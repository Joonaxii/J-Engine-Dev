#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/IO/Serialization/Serialization.h>
#include <string>
#include <sstream>
#include <iostream>

namespace JEngine {
    template<typename TMaj, typename TMin, typename TRev>
    class Version : public ISerializable<Version<TMaj, TMin, TRev>, sizeof(TMaj) + sizeof(TMin) + sizeof(TRev)> {
    public:
        Version() : _major(), _minor(), _revision() {}
        Version(const TMaj& major, const TMin& minor, const TRev& revision) : _major(major), _minor(minor), _revision(revision) {}

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

        const bool deserializeJson(json& jsonF) {
            _major = jsonF.value("major", TMaj());
            _minor = jsonF.value("minor", TMin());
            _revision = jsonF.value("revision", TRev());
            return true;
        }

        const bool serializeJson(json& jsonF) {
            jsonF["major"] = _major;
            jsonF["minor"] = _minor;
            jsonF["revision"] = _revision;
            return true;
        }

    private:
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
}
