#include <JEngine/Assets/Import/AudioImportSettings.h>

namespace JEngine {
    bool AudioImportSettings::deserialize(const yamlNode& node) {
        flags.setBit(F_CONVERT_SAMPLE_RATE, Serialization::deserialize<bool>(node["convertSamplerate"]));
        flags.setBit(F_IS_MONO, Serialization::deserialize<bool>(node["isMono"]));
        flags.setBit(F_USE_SECTIONS, Serialization::deserialize<bool>(node["useSections"]));
        flags.setBit(F_USE_BEATS, Serialization::deserialize<bool>(node["useBetas"]));
        return false;
    }

    bool AudioImportSettings::serialize(yamlEmit& node) const {

        return false;
    }

    bool AudioImportSettings::deserialize(const Stream& stream) {
        stream.readValue(flags, false);
        return false;
    }
    bool AudioImportSettings::serialize(const Stream& stream) const {
        return false;
    }
}