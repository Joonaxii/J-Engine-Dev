#pragma once
#include <JEngine/Utility/Flags.h>
#include <JEngine/Assets/Import/ImportSettings.h>
#include <JEngine/Audio/BeatMap.h>
#include <JEngine/Audio/AudioClip.h>

namespace JEngine {
    struct BeatMapSettings {
        enum : uint8_t {
            F_NONE = 0x00,
            F_GENERATED = 0x01,
        };

        float baseBPM{};
        TimeSignature baseSignature{};
        UI8Flags flags{ F_NONE };
        std::vector<Beat> beats{};
    };

    struct AudioImportSettings : public ImportSettings<AudioImportSettings> {
        enum : uint8_t {
            F_NONE = 0x00,
            F_CONVERT_SAMPLE_RATE = 0x1,
            F_IS_MONO = 0x2,
            F_USE_SECTIONS = 0x4,
            F_USE_BEATS = 0x8,
        };

        UI8Flags flags{};
        AudioSection sections[AudioClip::MAX_SECTION_COUNT];

        bool deserialize(const yamlNode& node) override;
        bool serialize(yamlEmit& node) const override;

        bool deserialize(const Stream& stream) override;
        bool serialize(const Stream& stream) const override;

        void reset() override {
            flags = F_NONE;
        }

        void copyFrom(const AudioImportSettings& other) override { 
            flags = other.flags;
            memcpy(sections, other.sections, sizeof(sections));
        }
        bool hasChanged(const AudioImportSettings& other) const override { 
            return other.flags != flags;
        }
    };
}