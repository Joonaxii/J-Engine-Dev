#pragma once
#include <cstdint>
#include <vector>
#include <JEngine/Math/Math.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Audio/AudioClip.h>
#include <JEngine/Assets/IAsset.h>

namespace JEngine {
JE_BEG_PACK
    struct Beat {
        enum : uint8_t {
            F_NONE = 0x00,
            F_OMITTED = 0x01,
            F_STRONG = 0x02,
        };
        UI8Flags flags{};
        uint32_t samplePos{};
    };

    struct TimeSignature {
    public:
        static constexpr uint16_t MIN_BEATS = 1;
        static constexpr uint16_t MAX_BEATS = 16;

        constexpr TimeSignature() noexcept : _signature(), _flags(), _offsets{} {}
        constexpr TimeSignature(int32_t numOfBeats, int32_t noteValue) noexcept :
            _signature(
                uint8_t((Math::clamp<int32_t>(numOfBeats, MIN_BEATS, MAX_BEATS) - MIN_BEATS)) |
                uint8_t((Math::clamp<int32_t>(noteValue, MIN_BEATS, MAX_BEATS) - MIN_BEATS) << 4)
            ), _flags(0x1), _offsets{ 0x00 } {
        }

        bool operator==(const TimeSignature& other) const {
            return memcmp(this, &other, sizeof(TimeSignature)) == 0;
        }

        bool operator!=(const TimeSignature& other) const {
            return memcmp(this, &other, sizeof(TimeSignature)) != 0;
        }

        float getOffset(int32_t beat) const { return _offsets[beat]; }

        uint8_t getNumOfBeats() const { return (_signature & 0xF) + 1; }
        uint8_t getNoteValue() const { return (_signature >> 4) + 1; }

        bool isStressed(int32_t beat) const { return (_flags & (1U << beat)) != 0; }
        bool isOmitted(int32_t beat) const { return (_flags & (1U << (beat + MAX_BEATS))) != 0; }

        uint8_t getSignature() const { return _signature; }
        uint32_t getFlags() const { return _flags; }

        void addBeats(std::vector<Beat>& beats, uint32_t& samplePos, uint32_t samplesPerBeat) const {
            int32_t beatCount = getNumOfBeats();
            uint32_t noteValue = getNoteValue();
            uint32_t beatLength = samplesPerBeat * noteValue;
            uint32_t samplesPerNote = uint16_t(beatLength / beatCount);

            uint32_t lPos = samplePos;
            for (int32_t i = 0; i < beatCount; i++) {
                Beat& beat = beats.emplace_back();
                beat.samplePos = lPos + uint32_t(Math::normalize_T(_offsets[i]) * samplesPerNote);
                beat.flags =
                    (isStressed(i) ? Beat::F_STRONG : Beat::F_NONE) |
                    (isOmitted(i) ? Beat::F_OMITTED : Beat::F_NONE);
                lPos += samplesPerNote;
            }
            samplePos += beatLength;
        }

    private:
        friend struct Serializable<TimeSignature>;
        friend struct YAML::convert<TimeSignature>;

        uint8_t _signature{};
        uint32_t _flags{};
        uint16_t _offsets[MAX_BEATS]{ 0 };
    };
JE_END_PACK

    class BeatMap : public IAsset {
    public:
        ~BeatMap();
        bool unload() override;

        const std::vector<Beat>& getBeats() const { return _beats; }
    private:
        std::vector<Beat> _beats{};
    };

    namespace TimeSignatures {
        static constexpr TimeSignature Zero{ 0, 0 };
        static constexpr TimeSignature CutTime{ 2, 2 };
        static constexpr TimeSignature TwoFour{ 2, 4 };
        static constexpr TimeSignature ThreeFour{ 3, 4 };
        static constexpr TimeSignature CommonTime{ 4, 4 };
        static constexpr TimeSignature SixEight{ 6, 8 };
        static constexpr TimeSignature NineEight{ 9, 8 };
        static constexpr TimeSignature TwelveEight{ 12, 8 };
    }
}



#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::TimeSignature& itemRef) {
        yamlOut << YAML::BeginMap;
        yamlOut << YAML::Key << "signature" << YAML::Value << YAML::Hex << uint16_t(itemRef.getSignature());
        yamlOut << YAML::Key << "flags" << YAML::Value << YAML::Hex << itemRef.getFlags();

        yamlOut << YAML::Dec;
        yamlOut << YAML::Key << "offsets" << YAML::Value << YAML::BeginSeq;

        for (uint16_t i = 0; i < JEngine::TimeSignature::MAX_BEATS; i++) {
            yamlOut << itemRef.getOffset(i);
        }

        yamlOut << YAML::EndSeq << YAML::EndMap;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::TimeSignature> {
        static Node encode(const JEngine::TimeSignature& rhs) {
            Node node{};
            node["signature"] = uint16_t(rhs._signature);
            node["flags"] = rhs._flags;
            auto& offs = node["offsets"] = Node();
            for (size_t i = 0; i < 16; i++) {
                offs.push_back(rhs._offsets[i]);
            }
            return node;
        }

        static bool decode(const Node& node, JEngine::TimeSignature& rhs) {
            if (node.size() < (2ULL + JEngine::TimeSignature::MAX_BEATS)) { return false; }
            rhs._signature = uint8_t(node["signature"].as<uint16_t>());
            rhs._flags = node["flags"].as<uint32_t>();

            auto& arr = node["offests"];
            for (size_t i = 0; i < JEngine::TimeSignature::MAX_BEATS; i++) {
                rhs._offsets[i] = arr[i].as<uint16_t>();
            }
            return true;
        }
    };

    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::Beat& itemRef) {
        yamlOut << YAML::BeginMap;
        yamlOut << YAML::Key << "flags" << YAML::Value << YAML::Hex << uint8_t(itemRef.flags);
        yamlOut << YAML::Key << "samplePos" << YAML::Value << YAML::Hex << itemRef.samplePos;
        yamlOut << YAML::EndMap;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::Beat> {
        static Node encode(const JEngine::Beat& rhs) {
            Node node;
            node["samplePos"] = rhs.samplePos;
            node["flags"] = rhs.flags;

            return node;
        }

        static bool decode(const Node& node, JEngine::Beat& rhs) {
            if (node.size() < 2) { return false; }
            rhs.samplePos = node["samplePos"].as<uint32_t>();
            rhs.flags = node["flags"].as<UI8Flags>();
            return true;
        }
    };
}
REGISTER_ASSET(JEngine::BeatMap);

//JSON and Binary
namespace JEngine {
    template<>
    struct Serializable<TimeSignature> {
        static bool deserializeJson(TimeSignature& itemRef, const json& jsonF, const TimeSignature& defaultValue);
        static bool serializeJson(const TimeSignature& itemRef, json& jsonF);

        static bool deserializeBinary(TimeSignature& itemRef, const Stream& stream);
        static bool serializeBinary(const TimeSignature& itemRef, const Stream& stream);
    };

    inline bool Serializable<TimeSignature>::deserializeJson(TimeSignature& itemRef, const json& jsonF, const TimeSignature& defaultValue) {
        Serialization::deserialize(itemRef._signature, jsonF["signature"], defaultValue._signature);
        Serialization::deserialize(itemRef._flags, jsonF["flags"], defaultValue._flags);

        auto& offs = jsonF["offsets"];
        for (size_t i = 0; i < std::min(offs.size(), 16ULL); i++) {
            itemRef._offsets[i] = offs[i].get<uint16_t>();
        }

        Serialization::deserialize(itemRef._offsets, jsonF["offsets"], defaultValue._offsets);
        return true;
    }

    inline bool Serializable<TimeSignature>::serializeJson(const TimeSignature& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._signature, jsonF["signature"]);
        Serialization::serialize(itemRef._flags, jsonF["flags"]);
        jsonF["offsets"] = itemRef._offsets;
        return true;
    }

    inline bool Serializable<TimeSignature>::deserializeBinary(TimeSignature& itemRef, const Stream& stream) {
        Serialization::deserialize(itemRef._signature, stream);
        Serialization::deserialize(itemRef._flags, stream);
        Serialization::deserialize(itemRef._offsets, stream);
        return true;
    }

    inline bool Serializable<TimeSignature>::serializeBinary(const TimeSignature& itemRef, const Stream& stream) {
        Serialization::serialize(itemRef._signature, stream);
        Serialization::serialize(itemRef._flags, stream);
        Serialization::serialize(itemRef._offsets, stream);
        return true;
    }

    template<>
    struct Serializable<Beat> {
        static bool deserializeJson(Beat& itemRef, const json& jsonF, const Beat& defaultValue);
        static bool serializeJson(const Beat& itemRef, json& jsonF);

        static bool deserializeBinary(Beat& itemRef, const Stream& stream);
        static bool serializeBinary(const Beat& itemRef, const Stream& stream);
    };

    inline bool Serializable<Beat>::deserializeJson(Beat& itemRef, const json& jsonF, const Beat& defaultValue) {
        Serialization::deserialize(itemRef.samplePos, jsonF["samplePos"], defaultValue.samplePos);
        Serialization::deserialize(itemRef.flags, jsonF["flags"], defaultValue.flags);
        return true;
    }

    inline bool Serializable<Beat>::serializeJson(const Beat& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.samplePos, jsonF["samplePos"]);
        Serialization::serialize(itemRef.flags, jsonF["flags"]);
        return true;
    }

    inline bool Serializable<Beat>::deserializeBinary(Beat& itemRef, const Stream& stream) {
        Serialization::deserialize(itemRef.samplePos, stream);
        Serialization::deserialize(itemRef.flags, stream);
        return true;
    }

    inline bool Serializable<Beat>::serializeBinary(const Beat& itemRef, const Stream& stream) {
        Serialization::serialize(itemRef.samplePos, stream);
        Serialization::serialize(itemRef.flags, stream);
        return true;
    }
}
#pragma endregion
