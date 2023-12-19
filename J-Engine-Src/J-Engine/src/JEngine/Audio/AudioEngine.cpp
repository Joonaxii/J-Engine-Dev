#include <JEngine/Audio/AudioEngine.h>

namespace JEngine {

    AudioEngine::AudioEngine() : 
        _format{}, _xEngine{ nullptr }, _xMaster{nullptr}{
        auto ret = CoInitializeEx(NULL, COINITBASE_MULTITHREADED);
        _format.cbSize = 0;
        _format.nChannels = AUDIO_CHANNELS;
        _format.nSamplesPerSec = AUDIO_SAMPLE_RATE;
        _format.wFormatTag = AUDIO_FORMAT;
        _format.wBitsPerSample = AUDIO_BIT_DEPTH;
        _format.nAvgBytesPerSec = DWORD(AUDIO_BUFFER_SIZE);
        _format.nBlockAlign = (AUDIO_CHANNELS * AUDIO_BIT_DEPTH) / 8;
        
        ret = XAudio2Create(&_xEngine);
        _xEngine->CreateMasteringVoice(&_xMaster);



    }

    AudioEngine::~AudioEngine() {
        if (_xEngine) {
            _xEngine->Release();
            _xEngine = nullptr;
        }
        CoUninitialize();
    }

    void __stdcall AudioEngine::VoiceCallback::OnBufferStart(void* bufferContext) {
        AudioEngine::Voice* voice = (AudioEngine::Voice*)bufferContext;
        voice->mixAudioSource();
    }

    void __stdcall AudioEngine::VoiceCallback::OnBufferEnd(void* bufferContext) {
        AudioEngine::Voice* voice = (AudioEngine::Voice*)bufferContext;

    }

    template<size_t size, AudioSampleType type>
    static inline float getSampleF(const uint8_t* data) { return 0; }

    template<>
    inline float getSampleF<8, AudioSampleType::Signed>(const uint8_t* data) {
        return Audio::sampleI8ToF_T(*reinterpret_cast<const int8_t*>(data));
    }

    template<>
    inline float getSampleF<8, AudioSampleType::Unsigned>(const uint8_t* data) {
        return Audio::sampleU8ToF_T(*data);
    }


    template<>
    inline float getSampleF<16, AudioSampleType::Signed>(const uint8_t* data) {
        return Audio::sampleI16ToF_T(*reinterpret_cast<const int16_t*>(data));
    }

    template<>
    inline float getSampleF<16, AudioSampleType::Unsigned>(const uint8_t* data) {
        return Audio::sampleU16ToF_T(*reinterpret_cast<const uint16_t*>(data));
    }

    template<>
    inline float getSampleF<32, AudioSampleType::Signed>(const uint8_t* data) {
        return Audio::fromSampleI32F(*reinterpret_cast<const int32_t*>(data));
    }

    template<>
    inline float getSampleF<32, AudioSampleType::Unsigned>(const uint8_t* data) {
        return Audio::fromSampleU32F(*reinterpret_cast<const uint32_t*>(data));
    }

    typedef float (*GetAudioSample)(const uint8_t*);

    static void mixAudio(size_t* offsets, float* output, float* volumes, bool isMono, bool isLayered, uint8_t layers, const AudioData& audioData, GetAudioSample getSample) {     
        size_t bps = (audioData.depth >> 3);
        size_t bpsC = bps * audioData.channels;

        if (isMono) {
            if (isLayered) {
                for (size_t i = 0, j = 0; i < audioData.sampleRate; i++) {
                    for (size_t l = 0, lv = 0; l < layers; l++) {
                        float sample = getSample(audioData.data + offsets[l]);
                        output[j++] = volumes[lv++] * sample;
                        output[j++] = volumes[lv++] * sample;
                        offsets[l] += bpsC;
                    }
                }
                return;
            }
            for (size_t i = 0, j = 0, k = offsets[0]; i < audioData.sampleRate; i++, k += bpsC) {
                float sample = getSample(audioData.data + k);
                output[j++] = volumes[0] * sample;
                output[j++] = volumes[1] * sample;
            }
            return;
        }

        if (isLayered) {
            for (size_t i = 0, j = 0; i < audioData.sampleRate; i++) {
                for (size_t l = 0, lC = 0, lv = 0; l < layers; l++) {
                    float sampleL = getSample(audioData.data + offsets[lC + 0]);
                    float sampleR = getSample(audioData.data + offsets[lC + 1]);

                    output[j++] = volumes[lv++] * sampleL;
                    output[j++] = volumes[lv++] * sampleR;

                    offsets[lC++] += bpsC;
                    offsets[lC++] += bpsC;
                }
            }
            return;
        }
        for (size_t i = 0, j = 0, k = offsets[0]; i < audioData.sampleRate; i++, k += bpsC) {
            float sampleL = getSample(audioData.data + offsets[k + 0]);
            float sampleR = getSample(audioData.data + offsets[k + 1]);

            output[j++] = volumes[0] * sampleL;
            output[j++] = volumes[1] * sampleR;
        }
    }

    void AudioEngine::Voice::mixAudioSource() {
        if (!_aSource || !_aSource->_clip) { return; }
        auto clip = _aSource->_clip;

        if (clip->getSampleRate() > AUDIO_SAMPLE_RATE) {
            JENGINE_CORE_WARN("[Audio Engine] Warning: Audio sample rate must be less or equal to '{0}' but is '{1}'!", AUDIO_SAMPLE_RATE, clip->getSampleRate());
            return;
        }

        float* samples = reinterpret_cast<float*>(_dBuffer);
        memset(_dBuffer, 0, AUDIO_BUFFER_SIZE);

        uint8_t layers = clip->getLayerCount();
        bool isLayered = clip->isLayered();
        auto& clipData = clip->getAudioData();

        size_t bps = (clipData.depth >> 3);
        size_t bpsC = bps * clipData.channels;

        float lVol = _aSource->_volume * Audio::getPanL(_aSource->_pan);
        float rVol = _aSource->_volume * Audio::getPanR(_aSource->_pan);

        size_t currentPos = _aSource->_timeSamples * bpsC;

        size_t offsets[AudioClip::MAX_AUDIO_LAYERS * 2]{};
        float volumes[AudioClip::MAX_AUDIO_LAYERS * 2] {};

        GetAudioSample getSample{nullptr};
        switch (clipData.depth) {
            default:
                JENGINE_CORE_WARN("[Audio Engine] Warning: Audio bit depth '{0}' not supported!", int32_t(clipData.depth));
                return;
            case 8:
                getSample = clipData.sampleType == AudioSampleType::Signed ? getSampleF<8, AudioSampleType::Signed> : getSampleF<8, AudioSampleType::Unsigned>;
                break;  
            case 16:
                getSample = clipData.sampleType == AudioSampleType::Signed ? getSampleF<16, AudioSampleType::Signed> : getSampleF<16, AudioSampleType::Unsigned>;
                break;
            case 32:
                getSample = clipData.sampleType == AudioSampleType::Signed ? getSampleF<32, AudioSampleType::Signed> : getSampleF<32, AudioSampleType::Unsigned>;
                break;
        }

        if (clip->isMono()) {
            for (size_t i = 0, j = 0; i < layers; i++ , j += bps) {
                offsets[i] = j;
            }
        }
        else {
            for (size_t i = 0, j = 0, k = 0; i < layers; i++) {
                offsets[k++] = j;
                j += bps;
                offsets[k++] = j;
                j += bps;
            }
        }

        if (isLayered) {
            for (size_t i = 0, j = 0; i < layers; i++) {
                auto& layer = _aSource->_layers[i];
                volumes[j++] = lVol * Audio::getPanL(layer.pan) * layer.volume;
                volumes[j++] = rVol * Audio::getPanR(layer.pan) * layer.volume;
            }
        }
        else {
            volumes[0] = lVol;
            volumes[1] = rVol;
        }

        mixAudio(offsets, samples, volumes, clip->isMono(), isLayered, layers, clipData, getSample);
        _xBuffer.AudioBytes = clip->getSampleRate();
    }

    bool AudioEngine::Voice::play(IAudioSource* aSource, VoiceBufferAllocator& allocator) {
        _aSource = aSource;
        _source->Stop(0);
        _source->FlushSourceBuffers();
        if (!_aSource || !_aSource->_clip) { 
            return false;
        }
        _dBuffer = _dBuffer ? _dBuffer : allocator.allocate();
        
        if (!_dBuffer) {
            JENGINE_CORE_ERROR("[Audio Engine] Error: Failed to allocate/retrieve audio buffer!");
            return false;
        }

        //_currentSample = _aSource->;

        mixAudioSource();
        _xBuffer.pAudioData = _dBuffer->buffer;
        _source->SetSourceSampleRate(_aSource->_clip->getSampleRate());
        _source->SetFrequencyRatio(_aSource->_pitch);
        _source->SubmitSourceBuffer(&_xBuffer);
        _source->Start(0);
        return true;
    }

    bool AudioEngine::Voice::update() {
        if (!_aSource) { return false; }

        XAUDIO2_VOICE_STATE state{};
        _source->GetState(&state, 0);

        if (_aSource->_asFlags.isBitSet(IAudioSource::AS_FLAG_CHANGED_VOLUME)) {
            mixAudioSource();
        }

        if (_aSource->_asFlags.isBitSet(IAudioSource::AS_FLAG_CHANGED_PITCH)) {
            _source->SetFrequencyRatio(_aSource->_pitch);
        }

        if (_aSource->_asFlags.isBitSet(IAudioSource::AS_FLAG_CHANGED_TIME)) {
            
        }
        else {
            _aSource->_timeSamples = _currentSample + state.SamplesPlayed;
        } 

    
        return true;
    }

}