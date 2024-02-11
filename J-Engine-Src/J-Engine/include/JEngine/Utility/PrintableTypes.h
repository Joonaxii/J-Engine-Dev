#pragma once
#include <ostream>
#include <JEngine/Math/Units/JVector.h>

namespace JEngine {

#pragma region Vectors
    // JVector2
    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector2<T, P>& vec) {
        os << "XY: (" << vec.x << ", " << vec.y << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector2<int8_t, P>& vec) {
        os << "XY: (" << int32_t(vec.x) << ", " << int32_t(vec.y) << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector2<uint8_t, P>& vec) {
        os << "XY: (" << int32_t(vec.x) << ", " << int32_t(vec.y) << ")";
        return os;
    }

    // JVector3
    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector3<T, P>& vec) {
        os << "XYZ: (" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector3<int8_t, P>& vec) {
        os << "XYZ: (" << int32_t(vec.x) << ", " << int32_t(vec.y) << ", " << int32_t(vec.z) << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector3<uint8_t, P>& vec) {
        os << "XYZ: (" << int32_t(vec.x) << ", " << int32_t(vec.y) << ", " << int32_t(vec.z) << ")";
        return os;
    }

    // JVector4
    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector4<T, P>& vec) {
        os << "XYZW: (" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector4<int8_t, P>& vec) {
        os << "XYZW: (" << int32_t(vec.x) << ", " << int32_t(vec.y) << ", " << int32_t(vec.z) << ", " << int32_t(vec.w) << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector4<uint8_t, P>& vec) {
        os << "XYZW: (" << int32_t(vec.x) << ", " << int32_t(vec.y) << ", " << int32_t(vec.z) << ", " << int32_t(vec.w) << ")";
        return os;
    }
#pragma endregion
}