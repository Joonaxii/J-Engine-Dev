#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <string>
#include <sstream>
#include <iostream>

namespace JEngine {
    template<typename TMaj, typename TMin, typename TRev>
    struct Version {
    public:
        Version() : _major(), _minor(), _revision() {}
        Version(const TMaj& major, const TMin& minor, const TRev& revision) : _major(major), _minor(minor), _revision(revision) {}

        TMaj& getMajor() { return _major; }
        TMin& getMinor() { return _minor; }
        TRev& getRevision() { return _revision; }

        const TMaj& getMajor() const { return _major; }
        const TMin& getMinor() const { return _minor; }
        const TRev& getRevision() const { return _revision; }

        const std::string toString() const {
            std::stringstream sstrm;
            sstrm << _major;
            sstrm << '.';
            sstrm << _minor;
            sstrm << '.';
            sstrm << _revision;
            return sstrm.str();
        }

    private:
        friend struct Serializable<Version<TMaj, TMin, TRev>>;

        const TMaj _major;
        const TMin _minor;
        const TRev _revision;
    };

    typedef JEngine::Version<uint16_t, uint8_t, uint8_t> JVersion;

    template<typename TMaj, typename TMin, typename TRev>
    inline std::ostream& operator<<(std::ostream& os, const Version<TMaj, TMin, TRev>& ver) {
        os << +ver.getMajor() << '.' << +ver.getMinor() << '.' << +ver.getRevision();
        return os;
    }

#pragma region Serialization
    template<typename TMaj, typename TMin, typename TRev>
    struct Serializable<Version<TMaj, TMin, TRev>> {
        static const bool deserializeJson(Version<TMaj, TMin, TRev>& itemRef, json& jsonF, const Version<TMaj, TMin, TRev>& defaultVal = {});
        static const bool serializeJson(const Version<TMaj, TMin, TRev>& itemRef, json& jsonF);

        static const bool deserializeBinary(Version<TMaj, TMin, TRev>& itemRef, std::istream& stream);
        static const bool serializeBinary(const Version<TMaj, TMin, TRev>& itemRef, std::ostream& stream);
    };

    template<typename TMaj, typename TMin, typename TRev>
    inline const bool Serializable<Version<TMaj, TMin, TRev>>::deserializeJson(Version<TMaj, TMin, TRev>& itemRef, json& jsonF, const Version<TMaj, TMin, TRev>& defaultVal) {
        Serialization::deserialize(itemRef._major   , jsonF["major"]   , defaultVal._major);
        Serialization::deserialize(itemRef._minor   , jsonF["minor"]   , defaultVal._minor);
        Serialization::deserialize(itemRef._revision, jsonF["revision"], defaultVal._revision);
        return true;
    }

    template<typename TMaj, typename TMin, typename TRev>
    inline const bool Serializable<Version<TMaj, TMin, TRev>>::serializeJson(const Version<TMaj, TMin, TRev>& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._major   , jsonF["major"]);
        Serialization::serialize(itemRef._minor   , jsonF["minor"]);
        Serialization::serialize(itemRef._revision, jsonF["revision"]);
        return true;
    }

    template<typename TMaj, typename TMin, typename TRev>
    inline const bool Serializable<Version<TMaj, TMin, TRev>>::deserializeBinary(Version<TMaj, TMin, TRev>& itemRef, std::istream& stream) {
        Serialization::deserialize(itemRef._major   , stream);
        Serialization::deserialize(itemRef._minor   , stream);
        Serialization::deserialize(itemRef._revision, stream);
        return true;
    }

    template<typename TMaj, typename TMin, typename TRev>
    inline const bool Serializable<Version<TMaj, TMin, TRev>>::serializeBinary(const Version<TMaj, TMin, TRev>& itemRef, std::ostream& stream) {
        Serialization::serialize(itemRef._major, stream);
        Serialization::serialize(itemRef._minor, stream);
        Serialization::serialize(itemRef._revision, stream);
        return true;
    }
#pragma endregion
}
