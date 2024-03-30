#pragma once
#include <cstdint>
#include <xaudio2.h>
#include <vector>
#include <JEngine/Audio/IAudioSource.h>
#include <JEngine/Collections/PoolAllocator.h>

namespace JEngine {
    class AudioEngine {
    public:
        static constexpr uint32_t AUDIO_SAMPLE_RATE = 48000;
        static constexpr uint32_t AUDIO_CHANNELS = 2;
        static constexpr uint32_t AUDIO_BIT_DEPTH = 32;
        static constexpr uint16_t AUDIO_FORMAT = WAVE_FORMAT_IEEE_FLOAT;
        static constexpr int32_t MAX_VOICES = 64;
        static constexpr size_t AUDIO_BUFFER_SIZE = (AUDIO_SAMPLE_RATE * AUDIO_CHANNELS * (AUDIO_BIT_DEPTH >> 3));

    private:

        struct VoiceBuffer {
            uint8_t buffer[AUDIO_BUFFER_SIZE]{};
        };

        typedef PoolAllocator<VoiceBuffer, 1> VoiceBufferAllocator;

        struct VoiceCallback : IXAudio2VoiceCallback { 
            void STDMETHODCALLTYPE OnBufferStart(void* bufferContext) override;
            void STDMETHODCALLTYPE OnBufferEnd(void* bufferContext) override;

            void STDMETHODCALLTYPE OnLoopEnd(void* bufferContext) override {};

            void STDMETHODCALLTYPE OnVoiceError(void* bufferContext, HRESULT error) override {};
            void STDMETHODCALLTYPE OnStreamEnd() override {};
            void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 samplesReq) override {};
            void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override {};
        };

        struct Voice {
        public:
            Voice() : _xBuffer{}, _dBuffer{} {
                static VoiceCallback vcb;
                _xBuffer.pContext = this;
                _xBuffer.PlayBegin = 0;
            }

            bool play(IAudioSource* aSource, VoiceBufferAllocator& allocator);
            bool update();

        private:
            friend struct VoiceCallback;

            VoiceBuffer* _dBuffer{nullptr};
            XAUDIO2_BUFFER _xBuffer{};
            IXAudio2SourceVoice* _source{ nullptr };
            IAudioSource* _aSource{nullptr};

            int64_t _currentSample{};
            int64_t _currentLength{};

            void mixAudioSource();
            void submitBuffer(float time);
        };

        WAVEFORMATEX _format;
        IXAudio2* _xEngine;
        IXAudio2MasteringVoice* _xMaster;

        Voice _allVoices[MAX_VOICES]{};
        std::vector<uint8_t> _idleVoices{};
        std::vector<uint8_t> _activeVoices{};
        VoiceBufferAllocator _buffers{};

        AudioEngine();
        ~AudioEngine();
    };
}