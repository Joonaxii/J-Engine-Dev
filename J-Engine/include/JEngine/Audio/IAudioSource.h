#pragma once
#include <cstdint>
#include <algorithm>
#include <JEngine/Audio/AudioClip.h>
#include <JEngine/Utility/Flags.h>

namespace JEngine {
    class IAudioSource {
    public:
        enum : int32_t {
            SECTION_NONE = -1,
        };
        
        IAudioSource() : _volume(1.0f), _pitch(1.0f), _layers{}{
            std::fill_n(_layers, AudioClip::MAX_AUDIO_LAYERS, LayerInfo());
        }

        virtual void play(int32_t section = SECTION_NONE, AudioPlayType playMode = AudioPlayType::Unspecified);
        virtual void pause();
        virtual void unPause();

        virtual bool isPlaying() const;
        virtual bool isPaused() const;

        int64_t getTimeSamples() const {
            return _timeSamples;
        }

        void setTimeSamples(int64_t samples) {
            if (_timeSamples == samples || _clip == nullptr) { return; }
            _timeSamples = samples < 0 ? 0 : samples >= _clip->getSampleLength() ? _clip->getSampleLength() : samples;
            _currentSection = _clip->timeSampleToSection(_timeSamples);
            _asFlags |= AS_FLAG_CHANGED_TIME;
        }

        float getTimeSeconds() const {
            return _clip ? _timeSamples / float(_clip->getSampleRate()) : 0.0f;
        }

        void setTimeSeconds(float seconds) {
            if (_clip == nullptr) { return; }
            setTimeSamples(int64_t(seconds * _clip->getSampleRate()));
        }

        int32_t getBeat() const {}
        uint8_t getLocalBeat() const { return _localBeat; }
        uint8_t getCurrentSection() const { return _currentSection; }

        void setAudioClip(AudioClip* clip) { _clip = clip; }
        AudioClip* getAudioClip() const { return _clip; }

        void setPriority(float priority) { _priority = priority; }
        float getPriority() const { return _priority; }

        void setPitch(float pitch) { 
            if (_pitch == pitch) { return; }
            _pitch = pitch < 0 ? 0 : pitch; 
            _asFlags |= AS_FLAG_CHANGED_PITCH;
        }
        float getPitch() const { return _pitch; }

        void setVolume(float volume) { _volume = volume > 1.0f ? 1.0f : volume < 0.0f ? 0.0f : volume; }
        float getVolume() const { return _volume; }  
        
        void setPan(float pan) { _pan = pan > 1.0f ? 1.0f : pan < -1.0f ? -1.0f : pan; }
        float getPan() const { return _pan; }

        void setLayerVolume(int32_t index, float volume) {
            _layers[index].volume = volume > 1.0f ? 1.0f : volume < 0.0f ? 0.0f : volume;
        }
        float getLayerVolume(int32_t index) const { return _layers[index].volume; }

        void setLayerPan(int32_t index, float pan) {
            _layers[index].pan = pan > 1.0f ? 1.0f : pan < -1.0f ? -1.0f : pan;
        }
        float getLayerPan(int32_t index) const { return _layers[index].pan; }

    private:
        friend class AudioEngine;
        static constexpr uint8_t AS_FLAG_CHANGED_TIME = 0x1;
        static constexpr uint8_t AS_FLAG_CHANGED_PITCH = 0x2;
        static constexpr uint8_t AS_FLAG_CHANGED_VOLUME = 0x4;
        static constexpr uint8_t AS_FLAG_IS_PLAYING = 0x10;

        struct LayerInfo {
            float volume{ 1.0f };
            float pan { 0.0f };
            LayerInfo() : volume(1.0f), pan(0.0f) {}
        };

        AudioClip* _clip{nullptr};

        UI8Flags _asFlags{};

        float _priority{};
        float _volume{1};
        float _pan{0.0f};
        float _pitch{1};

        LayerInfo _layers[AudioClip::MAX_AUDIO_LAYERS]{LayerInfo()};

        uint8_t _currentSection{};
        uint8_t _currentVoice{};
        int64_t _timeSamples{};

        int32_t _currentMeasure{};
        uint8_t _localBeat{};
        
        void sendBeat(int32_t curMeasure, uint8_t beat);
    };
}