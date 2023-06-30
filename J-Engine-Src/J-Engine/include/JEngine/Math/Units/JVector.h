#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
#pragma pack(push, 1)
    template<typename T, typename P> struct JVector3;
    template<typename T, typename P> struct JVector4;
    template<typename T, typename P = float>
    struct JVector2 {
        T x;
        T y;

        constexpr JVector2();
        constexpr JVector2(const T x, const T y);
        constexpr JVector2(const JVector3<T, P>& vec);
        constexpr JVector2(const JVector4<T, P>& vec);

        constexpr bool operator==(const JVector2<T, P>& other) const;
        constexpr bool operator!=(const JVector2<T, P>& other) const;

        constexpr bool operator==(const JVector3<T, P>& other) const;
        constexpr bool operator!=(const JVector3<T, P>& other) const;

        constexpr bool operator==(const JVector4<T, P>& other) const;
        constexpr bool operator!=(const JVector4<T, P>& other) const;

        constexpr JVector2<T, P> operator-() const;

        constexpr JVector2<T, P> operator-(const JVector2<T, P>& other) const;
        constexpr JVector2<T, P> operator+(const JVector2<T, P>& other) const;

        constexpr JVector2<T, P> operator-(const JVector3<T, P>& other) const;
        constexpr JVector2<T, P> operator+(const JVector3<T, P>& other) const;

        constexpr JVector2<T, P> operator-(const JVector4<T, P>& other) const;
        constexpr JVector2<T, P> operator+(const JVector4<T, P>& other) const;

        JVector2<T, P>& operator-=(const JVector2<T, P>& other);
        JVector2<T, P>& operator+=(const JVector2<T, P>& other);

        JVector2<T, P>& operator-=(const JVector3<T, P>& other);
        JVector2<T, P>& operator+=(const JVector3<T, P>& other);

        JVector2<T, P>& operator-=(const JVector4<T, P>& other);
        JVector2<T, P>& operator+=(const JVector4<T, P>& other);

        constexpr JVector2<T, P> operator/(const JVector2<T, P>& value) const;
        constexpr JVector2<T, P> operator*(const JVector2<T, P>& value) const;

        JVector2<T, P>& operator/=(const JVector2<T, P>& value);
        JVector2<T, P>& operator*=(const JVector2<T, P>& value);

        constexpr JVector2<T, P> operator/(const JVector3<T, P>& value) const;
        constexpr JVector2<T, P> operator*(const JVector3<T, P>& value) const;

        JVector2<T, P>& operator/=(const JVector3<T, P>& value);
        JVector2<T, P>& operator*=(const JVector3<T, P>& value);

        constexpr JVector2<T, P> operator/(const JVector4<T, P>& value) const;
        constexpr JVector2<T, P> operator*(const JVector4<T, P>& value) const;

        JVector2<T, P>& operator/=(const JVector4<T, P>& value);
        JVector2<T, P>& operator*=(const JVector4<T, P>& value);

        constexpr JVector2<T, P> operator/(const P& value) const;
        constexpr JVector2<T, P> operator*(const P& value) const;

        JVector2<T, P>& operator/=(const P& value);
        JVector2<T, P>& operator*=(const P& value);

        constexpr P sqrMagnitude() const;
        constexpr P magnitude() const;

        constexpr P dot(const JVector2<T, P>& other) const;
        constexpr P cross(const JVector2<T, P>& other) const;

        P angle(const JVector2<T, P>& other) const;
        P signedAngle(const JVector2<T, P>& other) const;

        constexpr JVector2<T, P> getNormalized() const;
        JVector2<T, P>& normalize();
    };

    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector2<T, P>& vec) {
        os << "XY: (" << vec.x << ", " << vec.y << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector2<int8_t, P>& vec) {
        os << "XY: (" << int16_t(vec.x) << ", " << int16_t(vec.y) << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector2<uint8_t, P>& vec) {
        os << "XY: (" << uint16_t(vec.x) << ", " << uint16_t(vec.y) << ")";
        return os;
    }

    template<typename T, typename P> struct JVector2;
    template<typename T, typename P> struct JVector4;
    template<typename T, typename P = float>
    struct JVector3 {
        T x;
        T y;
        T z;

        constexpr JVector3();
        constexpr JVector3(const T x, const T y);
        constexpr JVector3(const JVector2<T, P>& vec);
        constexpr JVector3(const JVector4<T, P>& vec);
        constexpr JVector3(const T x, const T y, const T z);

        constexpr bool operator==(const JVector2<T, P>& other) const;
        constexpr bool operator!=(const JVector2<T, P>& other) const;

        constexpr bool operator==(const JVector3<T, P>& other) const;
        constexpr bool operator!=(const JVector3<T, P>& other) const;

        constexpr bool operator==(const JVector4<T, P>& other) const;
        constexpr bool operator!=(const JVector4<T, P>& other) const;

        constexpr JVector3<T, P> operator-() const;

        constexpr JVector3<T, P> operator-(const JVector2<T, P>& other) const;
        constexpr JVector3<T, P> operator+(const JVector2<T, P>& other) const;

        constexpr JVector3<T, P> operator-(const JVector3<T, P>& other) const;
        constexpr JVector3<T, P> operator+(const JVector3<T, P>& other) const;

        constexpr JVector3<T, P> operator-(const JVector4<T, P>& other) const;
        constexpr JVector3<T, P> operator+(const JVector4<T, P>& other) const;

        JVector3<T, P>& operator-=(const JVector2<T, P>& other);
        JVector3<T, P>& operator+=(const JVector2<T, P>& other);

        JVector3<T, P>& operator-=(const JVector3<T, P>& other);
        JVector3<T, P>& operator+=(const JVector3<T, P>& other);

        JVector3<T, P>& operator-=(const JVector4<T, P>& other);
        JVector3<T, P>& operator+=(const JVector4<T, P>& other);

        constexpr JVector3<T, P> operator/(const JVector2<T, P>& value) const;
        constexpr JVector3<T, P> operator*(const JVector2<T, P>& value) const;

        JVector3<T, P>& operator/=(const JVector2<T, P>& value);
        JVector3<T, P>& operator*=(const JVector2<T, P>& value);

        constexpr JVector3<T, P> operator/(const JVector3<T, P>& value) const;
        constexpr JVector3<T, P> operator*(const JVector3<T, P>& value) const;

        JVector3<T, P>& operator/=(const JVector3<T, P>& value);
        JVector3<T, P>& operator*=(const JVector3<T, P>& value);

        constexpr JVector3<T, P> operator/(const JVector4<T, P>& value) const;
        constexpr JVector3<T, P> operator*(const JVector4<T, P>& value) const;

        JVector3<T, P>& operator/=(const JVector4<T, P>& value);
        JVector3<T, P>& operator*=(const JVector4<T, P>& value);

        constexpr JVector3<T, P> operator/(const P& value) const;
        constexpr JVector3<T, P> operator*(const P& value) const;

        JVector3<T, P>& operator/=(const P& value);
        JVector3<T, P>& operator*=(const P& value);

        constexpr P sqrMagnitude() const;
        constexpr P magnitude() const;

        constexpr P dot(const JVector3<T, P>& other) const;
        constexpr JVector3<T, P> cross(const JVector3<T, P>& other) const;

        P angle(const JVector3<T, P>& other) const;
        P signedAngle(const JVector3<T, P>& other) const;

        constexpr JVector3<T, P> getNormalized() const;
        JVector3<T, P>& normalize();
    };

    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector3<T, P>& vec) {
        os << "XYZ: (" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector3<int8_t, P>& vec) {
        os << "XYZ: (" << int16_t(vec.x) << ", " << int16_t(vec.y) << ", " << int16_t(vec.z) << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector3<uint8_t, P>& vec) {
        os << "XYZ: (" << uint16_t(vec.x) << ", " << uint16_t(vec.y) << ", " << uint16_t(vec.z) << ")";
        return os;
    }

    template<typename T, typename P> struct JVector2;
    template<typename T, typename P> struct JVector3;
    template<typename T, typename P = float>
    struct JVector4 {
        T x;
        T y;
        T z;
        T w;

        constexpr JVector4();
        constexpr JVector4(const T x, const T y);
        constexpr JVector4(const JVector2<T, P>& vec);
        constexpr JVector4(const JVector3<T, P>& vec);
        constexpr JVector4(const T x, const T y, const T z);
        constexpr JVector4(const T x, const T y, const T z, const T w);

        constexpr bool operator==(const JVector2<T, P>& other) const;
        constexpr bool operator!=(const JVector2<T, P>& other) const;

        constexpr bool operator==(const JVector3<T, P>& other) const;
        constexpr bool operator!=(const JVector3<T, P>& other) const;

        constexpr bool operator==(const JVector4<T, P>& other) const;
        constexpr bool operator!=(const JVector4<T, P>& other) const;

        constexpr JVector4<T, P> operator-() const;

        constexpr JVector4<T, P> operator-(const JVector2<T, P>& other) const;
        constexpr JVector4<T, P> operator+(const JVector2<T, P>& other) const;

        constexpr JVector4<T, P> operator-(const JVector3<T, P>& other) const;
        constexpr JVector4<T, P> operator+(const JVector3<T, P>& other) const;

        constexpr JVector4<T, P> operator-(const JVector4<T, P>& other) const;
        constexpr JVector4<T, P> operator+(const JVector4<T, P>& other) const;

        JVector4<T, P>& operator-=(const JVector2<T, P>& other);
        JVector4<T, P>& operator+=(const JVector2<T, P>& other);

        JVector4<T, P>& operator-=(const JVector3<T, P>& other);
        JVector4<T, P>& operator+=(const JVector3<T, P>& other);

        JVector4<T, P>& operator-=(const JVector4<T, P>& other);
        JVector4<T, P>& operator+=(const JVector4<T, P>& other);

        constexpr JVector4<T, P> operator/(const JVector2<T, P>& value) const;
        constexpr JVector4<T, P> operator*(const JVector2<T, P>& value) const;

        JVector4<T, P>& operator/=(const JVector2<T, P>& value);
        JVector4<T, P>& operator*=(const JVector2<T, P>& value);

        constexpr JVector4<T, P> operator/(const JVector3<T, P>& value) const;
        constexpr JVector4<T, P> operator*(const JVector3<T, P>& value) const;

        JVector4<T, P>& operator/=(const JVector3<T, P>& value);
        JVector4<T, P>& operator*=(const JVector3<T, P>& value);

        constexpr JVector4<T, P> operator/(const JVector4<T, P>& value) const;
        constexpr JVector4<T, P> operator*(const JVector4<T, P>& value) const;

        JVector4<T, P>& operator/=(const JVector4<T, P>& value);
        JVector4<T, P>& operator*=(const JVector4<T, P>& value);

        constexpr JVector4<T, P> operator/(const P& value) const;
        constexpr JVector4<T, P> operator*(const P& value) const;

        JVector4<T, P>& operator/=(const P& value);
        JVector4<T, P>& operator*=(const P& value);

        constexpr P sqrMagnitude() const;
        constexpr P magnitude() const;

        constexpr P dot(const JVector4<T, P>& other) const;

        constexpr JVector4<T, P> getNormalized() const;
        JVector4<T, P>& normalize();
    };
#pragma pack(pop)

    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector4<T, P>& vec) {
        os << "XYZW: (" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector4<int8_t, P>& vec) {
        os << "XYZW: (" << int16_t(vec.x) << ", " << int16_t(vec.y) << ", " << int16_t(vec.z) << ", " << int16_t(vec.w) << ")";
        return os;
    }

    template<typename P> inline std::ostream& operator<<(std::ostream& os, const JVector4<uint8_t, P>& vec) {
        os << "XYZW: (" << uint16_t(vec.x) << ", " << uint16_t(vec.y) << ", " << uint16_t(vec.z) << ", " << int16_t(vec.w) << ")";
        return os;
    }

#pragma region JVector2
    template<typename T, typename P> inline constexpr JEngine::JVector2<T, P>::JVector2() : x(), y() { }
    template<typename T, typename P> inline constexpr JEngine::JVector2<T, P>::JVector2(const T x, const T y) : x(x), y(y) { }
    template<typename T, typename P> inline constexpr JEngine::JVector2<T, P>::JVector2(const JEngine::JVector3<T, P>& vec) : x(vec.x), y(vec.y) { }
    template<typename T, typename P> inline constexpr JEngine::JVector2<T, P>::JVector2(const JEngine::JVector4<T, P>& vec) : x(vec.x), y(vec.y) { }

    template<typename T, typename P> inline constexpr bool JVector2<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline constexpr bool JVector2<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline constexpr bool JVector2<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline constexpr bool JVector2<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline constexpr bool JVector2<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline constexpr bool JVector2<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator-() const {
        return JVector2<T, P>(-x, -y);
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector2<T, P>(x + other.x, y + other.y);
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector2<T, P>(x + other.x, y + other.y);
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator+(const JVector4<T, P>& other) const {
        return JVector2<T, P>(x + other.x, y + other.y);
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator-=(const JVector2<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator+=(const JVector2<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator-=(const JVector3<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator+=(const JVector3<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator-=(const JVector4<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator+=(const JVector4<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> inline constexpr P JVector2<T, P>::dot(const JVector2<T, P>& other) const {
        return P(x * other.x + y * other.y);
    }
    template<typename T, typename P> inline constexpr P JVector2<T, P>::cross(const JVector2<T, P>& other) const {
        return P(x * other.y - y * other.x);
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::getNormalized() const {
        const P mag = magnitude();
        return mag == 0 ? JVector2<T, P>(0, 0) : JVector2<T, P>(T(x / mag), T(y / mag));
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::normalize() {
        const P mag = magnitude();
        if (mag == 0) { return *this; }
        x = T(x / mag);
        y = T(y / mag);
        return *this;
    }

    template<typename T, typename P> inline constexpr P JVector2<T, P>::sqrMagnitude() const {
        return P(x * x + y * y);
    }
    template<typename T, typename P> inline constexpr P JVector2<T, P>::magnitude() const {
        return P(Math::sqrt(x * x + y * y));
    }

    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator/(const P& value) const {
        return JVector2(T(x / value), T(y / value));
    }
    template<typename T, typename P> inline constexpr JVector2<T, P> JVector2<T, P>::operator*(const P& value) const {
        return JVector2(T(x * value), T(y * value));
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator/=(const P& value) {
        x = T(x / value);
        y = T(y / value);
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator*=(const P& value) {
        x = T(x * value);
        y = T(y * value);
        return *this;
    }

    template<typename T, typename P> inline P JVector2<T, P>::angle(const JVector2<T, P>& other) const {
        const float den = Math::sqrt(sqrMagnitude() * other.sqrMagnitude());
        if (den < 0.000001f) { return P(0); }

        const float dotP = Math::clamp(dot(other) / den, -1.0f, 1.0f);
        return P(acosf(dotP) * RAD_2_DEG);
    }
    template<typename T, typename P> inline P JVector2<T, P>::signedAngle(const JVector2<T, P>& other) const {
        return P(angle(other) * Math::sign<P>(cross(other)));
    }

    template<typename T, typename P>
    struct Math::InvLerp<JVector2<T, P>, P> {
        static inline constexpr P inverseLerp(const JVector2<T, P>& a, const JVector2<T, P>& b, const JVector2<T, P>& v) {
            constexpr JVector2<T, P> aB = b - a;
            constexpr JVector2<T, P> aV = v - a;
            constexpr P dotB = aB.sqrMagnitude();
            return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
        }
    };

    //template<typename T, typename P>
    //inline constexpr P Math::InvLerp<JVector2<T, P>, P>::inverseLerp(const JVector2<T, P>& a, const JVector2<T, P>& b, const JVector2<T, P>& v) {
    //    constexpr JVector2<T, P> aB = b - a;
    //    constexpr JVector2<T, P> aV = v - a;
    //    constexpr P dotB = aB.sqrMagnitude();
    //    return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
    //}

#pragma endregion

#pragma region JVector3
    template<typename T, typename P> inline constexpr JEngine::JVector3<T, P>::JVector3() : x(), y(), z() { }
    template<typename T, typename P> inline constexpr JEngine::JVector3<T, P>::JVector3(const JEngine::JVector2<T, P>& vec) : x(vec.x), y(vec.y), z() { }
    template<typename T, typename P> inline constexpr JEngine::JVector3<T, P>::JVector3(const JEngine::JVector4<T, P>& vec) : x(vec.x), y(vec.y), z(vec.z) { }
    template<typename T, typename P> inline constexpr JEngine::JVector3<T, P>::JVector3(const T x, const T y) : x(x), y(y), z() { }
    template<typename T, typename P> inline constexpr JEngine::JVector3<T, P>::JVector3(const T x, const T y, const T z) : x(x), y(y), z(z) { }

    template<typename T, typename P> inline constexpr bool JVector3<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline constexpr bool JVector3<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline constexpr bool JVector3<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> inline constexpr bool JVector3<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> inline constexpr bool JVector3<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> inline constexpr bool JVector3<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator-() const {
        return JVector3<T, P>(-x, -y, -z);
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z);
    }
    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector3<T, P>(x + other.x, y + other.y, z);
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z - other.z);
    }
    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector3<T, P>(x + other.x, y + other.y, z + other.z);
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z - other.z);
    }
    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator+(const JVector4<T, P>& other) const {
        return JVector3<T, P>(x + other.x, y + other.y, z + other.z);
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator-=(const JVector2<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator+=(const JVector2<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator-=(const JVector3<T, P>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator+=(const JVector3<T, P>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator-=(const JVector4<T, P>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator+=(const JVector4<T, P>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z);
    }
    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z);
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z / value.z);
    }
    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z * value.z);
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z / value.z);
    }
    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z * value.z);
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;
        return *this;
    }

    template<typename T, typename P> inline constexpr P JVector3<T, P>::dot(const JVector3<T, P>& other) const {
        return P(x * other.x + y * other.y + z * other.z);
    }
    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::cross(const JVector3<T, P>& other) const {
        return JVector3<T, P>(y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::getNormalized() const {
        const P mag = magnitude();
        return mag == 0 ? JVector3<T, P>(0, 0, 0) : JVector3<T, P>(T(x / mag), T(y / mag), T(z / mag));
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::normalize() {
        const P mag = magnitude();
        if (mag == 0) { return *this; }
        x = T(x / mag);
        y = T(y / mag);
        z = T(z / mag);
        return *this;
    }

    template<typename T, typename P> inline constexpr P JVector3<T, P>::sqrMagnitude() const {
        return P(x * x + y * y + z * z);
    }
    template<typename T, typename P> inline constexpr P JVector3<T, P>::magnitude() const {
        return P(std::sqrt(x * x + y * y + z * z));
    }

    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator/(const P& value) const {
        return JVector3(T(x / value), T(y / value), T(z / value));
    }
    template<typename T, typename P> inline constexpr JVector3<T, P> JVector3<T, P>::operator*(const P& value) const {
        return JVector3(T(x * value), T(y * value), T(z * value));
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator/=(const P& value) {
        x = T(x / value);
        y = T(y / value);
        z = T(z / value);
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator*=(const P& value) {
        x = T(x * value);
        y = T(y * value);
        z = T(z * value);
        return *this;
    }

    template<typename T, typename P> inline P JVector3<T, P>::angle(const JVector3<T, P>& other) const {
        const float den = std::sqrtf(sqrMagnitude() * other.sqrMagnitude());
        if (den < 0.000001f) { return P(0); }

        const float dotP = Math::clamp(dot(other) / den, -1.0f, 1.0f);
        return P(acosf(dotP) * RAD_2_DEG);
    }
    template<typename T, typename P> inline P JVector3<T, P>::signedAngle(const JVector3<T, P>& other) const {
        return P(angle(other) * Math::sign(cross(other)));
    }

    template<typename T, typename P>
    struct Math::InvLerp<JVector3<T, P>, P> {
        static inline constexpr P inverseLerp(const JVector3<T, P>& a, const JVector3<T, P>& b, const JVector3<T, P>& v) {
            constexpr JVector3<T, P> aB = b - a;
            constexpr JVector3<T, P> aV = v - a;
            constexpr P dotB = aB.sqrMagnitude();
            return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
        }
    };
#pragma endregion

#pragma region JVector4
    template<typename T, typename P> inline constexpr JEngine::JVector4<T, P>::JVector4() : x(), y(), z(), w() { }
    template<typename T, typename P> inline constexpr JEngine::JVector4<T, P>::JVector4(const JEngine::JVector2<T, P>& vec) : x(vec.x), y(vec.y), z(), w() { }
    template<typename T, typename P> inline constexpr JEngine::JVector4<T, P>::JVector4(const JEngine::JVector3<T, P>& vec) : x(vec.x), y(vec.y), z(vec.z), w() { }
    template<typename T, typename P> inline constexpr JEngine::JVector4<T, P>::JVector4(const T x, const T y) : x(x), y(y), z(), w() { }
    template<typename T, typename P> inline constexpr JEngine::JVector4<T, P>::JVector4(const T x, const T y, const T z) : x(x), y(y), z(z), w() { }
    template<typename T, typename P> inline constexpr JEngine::JVector4<T, P>::JVector4(const T x, const T y, const T z, const T w) : x(x), y(y), z(z), w(w) { }

    template<typename T, typename P> inline constexpr bool JVector4<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline constexpr bool JVector4<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline constexpr bool JVector4<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> inline constexpr bool JVector4<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> inline constexpr bool JVector4<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    template<typename T, typename P> inline constexpr bool JVector4<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z || w != other.w;
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator-() const {
        return JVector4<T, P>(-x, -y, -z, -w);
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z, w);
    }
    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector4<T, P>(x + other.x, y + other.y, z, w);
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z - other.z, w);
    }
    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector4<T, P>(x + other.x, y + other.y, z + other.z, w);
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z - other.z, w);
    }
    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator+(const JVector4<T, P>& other) const {
        return JVector4<T, P>(x + other.x, y + other.y, z + other.z, w);
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator-=(const JVector2<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator+=(const JVector2<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator-=(const JVector3<T, P>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator+=(const JVector3<T, P>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator-=(const JVector4<T, P>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator+=(const JVector4<T, P>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z, w);
    }
    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z, w);
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z / value.z, w);
    }
    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z * value.z, w);
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;
        return *this;
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z / value.z, w / value.w);
    }
    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z * value.z, w * value.w);
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;
        w /= value.w;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;
        w *= value.w;
        return *this;
    }

    template<typename T, typename P> inline constexpr P JVector4<T, P>::dot(const JVector4<T, P>& other) const {
        return P(x * other.x + y * other.y + z * other.z + w * other.w);
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::getNormalized() const {
        const P mag = magnitude();
        return mag == 0 ? JVector4<T, P>(0, 0, 0, 0) : JVector4<T, P>(T(x / mag), T(y / mag), T(z / mag), T(w / mag));
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::normalize() {
        const P mag = magnitude();
        if (mag == 0) { return *this; }
        x = T(x / mag);
        y = T(y / mag);
        z = T(z / mag);
        w = T(w / mag);
        return *this;
    }

    template<typename T, typename P> inline constexpr P JVector4<T, P>::sqrMagnitude() const {
        return P(x * x + y * y + z * z + w * w);
    }
    template<typename T, typename P> inline constexpr P JVector4<T, P>::magnitude() const {
        return P(Math::sqrt(x * x + y * y + z * z + w * w));
    }

    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator/(const P& value) const {
        return JVector4(T(x / value), T(y / value), T(z / value), T(w / value));
    }
    template<typename T, typename P> inline constexpr JVector4<T, P> JVector4<T, P>::operator*(const P& value) const {
        return JVector4(T(x * value), T(y * value), T(z * value), T(w / value));
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator/=(const P& value) {
        x = T(x / value);
        y = T(y / value);
        z = T(z / value);
        w = T(w / value);
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator*=(const P& value) {
        x = T(x * value);
        y = T(y * value);
        z = T(z * value);
        w = T(w * value);
        return *this;
    }

    template<typename T, typename P>
    struct Math::InvLerp<JVector4<T, P>, P> {
        static inline constexpr P inverseLerp(const JVector4<T, P>& a, const JVector4<T, P>& b, const JVector4<T, P>& v) {
            constexpr JVector4<T, P> aB = b - a;
            constexpr JVector4<T, P> aV = v - a;
            constexpr P dotB = aB.sqrMagnitude();
            return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
        }
    };
#pragma endregion
}

#pragma region JVector2 Serialization
//YAML
namespace YAML {
    template<typename T>
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JVector2<T>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << itemRef.x << itemRef.y << YAML::EndSeq;
        return yamlOut;
    }

    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JVector2<uint8_t>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << uint16_t(itemRef.x) << uint16_t(itemRef.y) << YAML::EndSeq;
        return yamlOut;
    }

    template<typename T>
    struct convert<JEngine::JVector2<T>> {
        static Node encode(const JEngine::JVector2<T>& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, JEngine::JVector2<T>& rhs) {
            if (!node.IsSequence() || node.size() < 2) { return false; }
            rhs.x = node[0].as<T>();
            rhs.y = node[1].as<T>();
            return true;
        }
    };
}

//JSON and Binary
namespace JEngine {
    template<typename T, typename P>
    struct Serializable<JVector2<T, P>> {
        static bool deserializeJson(JVector2<T, P>& itemRef, json& jsonF, const JVector2<T, P>& defaultVal);
        static bool serializeJson(const JVector2<T, P>& itemRef, json& jsonF);

        static bool deserializeBinary(JVector2<T, P>& itemRef, const Stream& stream);
        static bool serializeBinary(const JVector2<T, P>& itemRef, const Stream& stream);
    };

    template<typename T, typename P>
    inline bool Serializable<JVector2<T, P>>::deserializeJson(JVector2<T, P>& itemRef, json& jsonF, const JVector2<T, P>& defaultVal) {
        Serialization::deserialize(itemRef.x, jsonF["x"], defaultVal.x);
        Serialization::deserialize(itemRef.y, jsonF["y"], defaultVal.y);
        return true;
    }

    template<typename T, typename P>
    inline bool Serializable<JVector2<T, P>>::serializeJson(const JVector2<T, P>& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.x, jsonF["x"]);
        Serialization::serialize(itemRef.y, jsonF["y"]);
        return true;
    }

    template<typename T, typename P>
    inline bool Serializable<JVector2<T, P>>::deserializeBinary(JVector2<T, P>& itemRef, const Stream& stream) {
        stream.readValue(itemRef, false);
        return true;
    }
    template<typename T, typename P>
    inline bool Serializable<JVector2<T, P>>::serializeBinary(const JVector2<T, P>& itemRef, const Stream& stream) {
        stream.writeValue(itemRef, 1, false);
        return true;
    }
}
#pragma endregion


#pragma region JVector3 Serialization
//YAML
namespace YAML {
    template<typename T>
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JVector3<T>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << itemRef.x << itemRef.y << itemRef.z << YAML::EndSeq;
        return yamlOut;
    }

    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JVector3<uint8_t>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << uint16_t(itemRef.x) << uint16_t(itemRef.y) << uint16_t(itemRef.z) << YAML::EndSeq;
        return yamlOut;
    }

    template<typename T>
    struct convert<JEngine::JVector3<T>> {
        static Node encode(const JEngine::JVector3<T>& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, JEngine::JVector3<T>& rhs) {
            if (!node.IsSequence() || node.size() < 3) { return false; }
            rhs.x = node[0].as<T>();
            rhs.y = node[1].as<T>();
            rhs.z = node[2].as<T>();
            return true;
        }
    };
}

//JSON and Binary
namespace JEngine {
    template<typename T, typename P>
    struct Serializable<JVector3<T, P>> {
        static bool deserializeJson(JVector3<T, P>& itemRef, json& jsonF, const JVector3<T, P>& defaultVal);
        static bool serializeJson(const JVector3<T, P>& itemRef, json& jsonF);

        static bool deserializeBinary(JVector3<T, P>& itemRef, const Stream& stream);
        static bool serializeBinary(const JVector3<T, P>& itemRef, const Stream& stream);
    };

    template<typename T, typename P>
    inline bool Serializable<JVector3<T, P>>::deserializeJson(JVector3<T, P>& itemRef, json& jsonF, const JVector3<T, P>& defaultVal) {
        Serialization::deserialize(itemRef.x, jsonF["x"], defaultVal.x);
        Serialization::deserialize(itemRef.y, jsonF["y"], defaultVal.y);
        Serialization::deserialize(itemRef.z, jsonF["z"], defaultVal.z);
        return true;
    }

    template<typename T, typename P>
    inline bool Serializable<JVector3<T, P>>::serializeJson(const JVector3<T, P>& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.x, jsonF["x"]);
        Serialization::serialize(itemRef.y, jsonF["y"]);
        Serialization::serialize(itemRef.z, jsonF["z"]);
        return true;
    }

    template<typename T, typename P>
    inline bool Serializable<JVector3<T, P>>::deserializeBinary(JVector3<T, P>& itemRef, const Stream& stream) {
        stream.readValue(itemRef, false);
        return true;
    }
    template<typename T, typename P>
    inline bool Serializable<JVector3<T, P>>::serializeBinary(const JVector3<T, P>& itemRef, const Stream& stream) {
        stream.writeValue(itemRef, 1, false);
        return true;
    }
}
#pragma endregion

#pragma region JVector4 Serialization
//YAML
namespace YAML {
    template<typename T>
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JVector4<T>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << itemRef.x << itemRef.y << itemRef.z << itemRef.w << YAML::EndSeq;
        return yamlOut;
    }

    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JVector4<uint8_t>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << uint16_t(itemRef.x) << uint16_t(itemRef.y) << uint16_t(itemRef.z) << uint16_t(itemRef.w) << YAML::EndSeq;
        return yamlOut;
    }

    template<typename T>
    struct convert<JEngine::JVector4<T>> {
        static Node encode(const JEngine::JVector4<T>& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, JEngine::JVector4<T>& rhs) {
            if (!node.IsSequence() || node.size() < 4) { return false; }
            rhs.x = node[0].as<T>();
            rhs.y = node[1].as<T>();
            rhs.z = node[2].as<T>();
            rhs.w = node[3].as<T>();
            return true;
        }
    };
}

//JSON and Binary
namespace JEngine {
    template<typename T, typename P>
    struct Serializable<JVector4<T, P>> {
        static bool deserializeJson(JVector4<T, P>& itemRef, json& jsonF, const JVector4<T, P>& defaultVal);
        static bool serializeJson(const JVector4<T, P>& itemRef, json& jsonF);

        static bool deserializeBinary(JVector4<T, P>& itemRef, const Stream& stream);
        static bool serializeBinary(const JVector4<T, P>& itemRef, const Stream& stream);
    };

    template<typename T, typename P>
    inline bool Serializable<JVector4<T, P>>::deserializeJson(JVector4<T, P>& itemRef, json& jsonF, const JVector4<T, P>& defaultVal) {
        Serialization::deserialize(itemRef.x, jsonF["x"], defaultVal.x);
        Serialization::deserialize(itemRef.y, jsonF["y"], defaultVal.y);
        Serialization::deserialize(itemRef.z, jsonF["z"], defaultVal.z);
        Serialization::deserialize(itemRef.w, jsonF["w"], defaultVal.w);
        return true;
    }

    template<typename T, typename P>
    inline bool Serializable<JVector4<T, P>>::serializeJson(const JVector4<T, P>& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.x, jsonF["x"]);
        Serialization::serialize(itemRef.y, jsonF["y"]);
        Serialization::serialize(itemRef.z, jsonF["z"]);
        Serialization::serialize(itemRef.w, jsonF["w"]);
        return true;
    }

    template<typename T, typename P>
    inline bool Serializable<JVector4<T, P>>::deserializeBinary(JVector4<T, P>& itemRef, const Stream& stream) {
        stream.readValue(itemRef, false);
        return true;
    }
    template<typename T, typename P>
    inline bool Serializable<JVector4<T, P>>::serializeBinary(const JVector4<T, P>& itemRef, const Stream& stream) {
        stream.writeValue(itemRef, 1, false);
        return true;
    }
}
#pragma endregion

typedef JEngine::JVector2<int32_t> JVector2i;
typedef JEngine::JVector3<int32_t> JVector3i;
typedef JEngine::JVector4<int32_t> JVector4i;

typedef JEngine::JVector2<uint32_t> JVector2u;
typedef JEngine::JVector3<uint32_t> JVector3u;
typedef JEngine::JVector4<uint32_t> JVector4u;

typedef JEngine::JVector2<float> JVector2f;
typedef JEngine::JVector3<float> JVector3f;
typedef JEngine::JVector4<float> JVector4f;

typedef JEngine::JVector2<double> JVector2d;
typedef JEngine::JVector3<double> JVector3d;
typedef JEngine::JVector4<double> JVector4d;