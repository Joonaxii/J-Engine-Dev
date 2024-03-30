#pragma once
#include <JEngine/Platform.h>
#include <string>
#include <sstream>
#include <iostream>

namespace JEngine {
JE_BEG_PACK
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
        TMaj _major;
        TMin _minor;
        TRev _revision;
    };

    template<typename TMaj, typename TMin, typename TRev>
    inline std::ostream& operator<<(std::ostream& os, const Version<TMaj, TMin, TRev>& ver) {
        os << +ver.getMajor() << '.' << +ver.getMinor() << '.' << +ver.getRevision();
        return os;
    }

JE_END_PACK
}
typedef JEngine::Version<uint16_t, uint8_t, uint8_t> JVersion;