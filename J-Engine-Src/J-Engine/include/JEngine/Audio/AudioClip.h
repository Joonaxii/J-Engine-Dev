#pragma once
#include <JEngine/Core.h>
#include <JEngine/IO/AudioUtils.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/IAssetSerializer.h>
#include <JEngine/Utility/EnumUtils.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
    enum class AudioPlayType : uint8_t {
        Unspecified,
        OneShot,
        Loop,
    };

JE_BEG_PACK
    struct AudioSection {
        enum : uint8_t {
            AUD_NULL_SECTION = 0xFF
        };

        uint8_t targetSection{ AUD_NULL_SECTION };
        uint32_t startSample{};
        uint32_t endSample{};

        uint32_t getSampleLength() const { 
            JE_CORE_ASSERT(startSample <= endSample, "End sample position must be greater or equal to start sample position!")
            return endSample - startSample; 
        }
    };
JE_END_PACK

    class AudioClip : public IAsset {
    public:
        static constexpr size_t MAX_AUDIO_LAYERS = 16;
        static constexpr size_t MAX_SECTION_COUNT = 16;

        ~AudioClip();
        bool unload() override;

        uint32_t getSampleRate() const {
            return _audioData.sampleRate;
        }

        template<typename P = float>
        inline float getLength() const {
            return P(_audioData.sampleCount) / std::max<P>(P(_audioData.sampleRate), P(1));
        }

        int64_t getSampleLength() const {
            return _audioData.sampleCount;
        }

        template<typename P = float>
        inline float getLength(int32_t section) const {
            return P(_sections[section & 0xF].getSampleLength()) / std::max<P>(P(_audioData.sampleRate), P(1));
        }

        int64_t getSampleLength(int32_t section) const {
            return _sections[section & 0xF].getSampleLength();
        }

        bool isMono() const {
            return _acFlags.isBitSet(AC_FLAG_IS_MONO);
        }

        const AudioData& getAudioData() const { return _audioData; }

        bool isLayered() const { return (_counts.data & 0xF) != 0; }
        bool isSectioned() const { return (_counts.data & 0xF0) != 0; }

        int32_t getLayerCount() const { return _counts.getLayerCount(); }
        int32_t getSectionCount() const { return _counts.getSectionCount(); }

        uint8_t timeSampleToSection(int64_t sample) const {
            uint8_t sectC = _counts.getSectionCount();
            uint8_t sectIDX = 0;
            for (uint8_t i = 0; i < sectC; i++) {
                auto& sect = _sections[i];
                if (sample >= sect.startSample && sample >= sect.endSample) {
                    sectIDX = i;
                }
            }
            return sectIDX;
        }

    protected:
        void destroyAsset() override {
            IAssetSerializer<AudioClip>::getAllocator().deallocate(this);
        }
    private:
        struct Counts {
            uint8_t data{};

            uint8_t getLayerCount() const { return (data & 0xF) + 1; }
            uint8_t getSectionCount() const { return ((data & 0xF) >> 4) + 1; }

            void setLayerCount(uint8_t value) { data = (data & 0xF0) | ((value - 1) & 0xF); }
            void setSectionCount(uint8_t value) { data = (data & 0xF) | (((value - 1) & 0xF) << 4);}
        };
        static constexpr uint8_t AC_FLAG_IS_MONO = 0x1;

        friend struct AudioClipSerializer;
        UI8Flags _acFlags{};
        Counts _counts{};
        AudioData _audioData{};
        AudioSection _sections[MAX_SECTION_COUNT]{};
    };
}
REGISTER_ASSET(JEngine::AudioClip);
