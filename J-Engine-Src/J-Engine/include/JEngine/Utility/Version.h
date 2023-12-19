#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <string>
#include <sstream>
#include <iostream>

namespace JEngine {
#pragma pack(push, 1)
    template<typename TMaj, typename TMin, typename TRev>
    struct Version {
    public:
        constexpr Version() : _major(), _minor(), _revision() {}
        constexpr Version(const TMaj& major, const TMin& minor, const TRev& revision) : _major(major), _minor(minor), _revision(revision) {}

        constexpr TMaj getMajor() const { return _major; }
        constexpr TMin getMinor() const { return _minor; }
        constexpr TRev getRevision() const { return _revision; }

        std::string toString() const {
            std::stringstream sstrm;
            sstrm << int64_t(_major);
            sstrm << '.';
            sstrm << int64_t(_minor);
            sstrm << '.';
            sstrm << int64_t(_revision);
            return sstrm.str();
        }

    private:
        friend struct Serializable<Version<TMaj, TMin, TRev>>;
        friend struct YAML::convert<Version<TMaj, TMin, TRev>>;

        TMaj _major;
        TMin _minor;
        TRev _revision;
    };

    template<typename TMaj, typename TMin, typename TRev>
    inline std::ostream& operator<<(std::ostream& os, const Version<TMaj, TMin, TRev>& ver) {
        os << +ver.getMajor() << '.' << +ver.getMinor() << '.' << +ver.getRevision();
        return os;
    }

#pragma pack(pop)
}
typedef JEngine::Version<uint16_t, uint8_t, uint8_t> JVersion;

#pragma region Serialization
//YAML
namespace YAML {
    template<typename TMaj, typename TMin, typename TRev>
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::Version<TMaj, TMin, TRev>& itemRef) {
        yamlOut << YAML::Flow << YAML::BeginSeq;
        yamlOut << uint64_t(itemRef.getMajor()) << uint64_t(itemRef.getMinor()) << uint64_t(itemRef.getRevision());
        yamlOut << YAML::EndSeq;
        return yamlOut;
    }

    template<typename TMaj, typename TMin, typename TRev>
    struct convert<JEngine::Version<TMaj, TMin, TRev>> {
        static Node encode(const JEngine::Version<TMaj, TMin, TRev>& rhs) {
            Node node;
            node.push_back(uint64_t(rhs._major));
            node.push_back(uint64_t(rhs._minor));
            node.push_back(uint64_t(rhs._revision));
            node.SetStyle(YAML::Flow);
            return node;
        }

        static bool decode(const Node& node, JEngine::Version<TMaj, TMin, TRev>& rhs) {
            if (!node.IsSequence() || node.size() < 3) { return false; }
            rhs._major    = node.as<TMaj>();
            rhs._minor    = node.as<TMin>();
            rhs._revision = node.as<TRev>();
            return true;
        }
    };
}

//JSON and Binary
namespace JEngine {
    template<typename TMaj, typename TMin, typename TRev>
    struct Serializable<Version<TMaj, TMin, TRev>> {
        static bool deserializeJson(Version<TMaj, TMin, TRev>& itemRef, json& jsonF, const Version<TMaj, TMin, TRev>& defaultVal = {});
        static bool serializeJson(const Version<TMaj, TMin, TRev>& itemRef, json& jsonF);

        static bool deserializeBinary(Version<TMaj, TMin, TRev>& itemRef, const Stream& stream);
        static bool serializeBinary(const Version<TMaj, TMin, TRev>& itemRef, const Stream& stream);
    };

    template<typename TMaj, typename TMin, typename TRev>
    inline bool Serializable<Version<TMaj, TMin, TRev>>::deserializeJson(Version<TMaj, TMin, TRev>& itemRef, json& jsonF, const Version<TMaj, TMin, TRev>& defaultVal) {
        Serialization::deserialize(itemRef._major, jsonF["major"], defaultVal._major);
        Serialization::deserialize(itemRef._minor, jsonF["minor"], defaultVal._minor);
        Serialization::deserialize(itemRef._revision, jsonF["revision"], defaultVal._revision);
        return true;
    }

    template<typename TMaj, typename TMin, typename TRev>
    inline bool Serializable<Version<TMaj, TMin, TRev>>::serializeJson(const Version<TMaj, TMin, TRev>& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._major, jsonF["major"]);
        Serialization::serialize(itemRef._minor, jsonF["minor"]);
        Serialization::serialize(itemRef._revision, jsonF["revision"]);
        return true;
    }

    template<typename TMaj, typename TMin, typename TRev>
    inline bool Serializable<Version<TMaj, TMin, TRev>>::deserializeBinary(Version<TMaj, TMin, TRev>& itemRef, const Stream& stream) {
        stream.readValue(itemRef, false);
        return true;
    }

    template<typename TMaj, typename TMin, typename TRev>
    inline bool Serializable<Version<TMaj, TMin, TRev>>::serializeBinary(const Version<TMaj, TMin, TRev>& itemRef, const Stream& stream) {
        stream.writeValue(itemRef, 1, false);
        return true;
    }
}
#pragma endregion