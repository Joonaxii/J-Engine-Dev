#pragma once
#include <JEngine/Platform.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
JE_BEG_PACK
    template<typename T, typename P> struct JVector3;
    template<typename T, typename P> struct JVector4;
    template<typename T, typename P = float>
    struct JVector2 {
        T x;
        T y;

        constexpr JVector2() noexcept : x{}, y{} {}
        constexpr JVector2(T x, T y) noexcept : x(x), y(y) { }
        constexpr JVector2(const JEngine::JVector3<T, P>& vec) noexcept;
        constexpr JVector2(const JEngine::JVector4<T, P>& vec) noexcept;

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


    template<typename T, typename P> struct JVector2;
    template<typename T, typename P> struct JVector4;
    template<typename T, typename P = float>
    struct JVector3 {
        T x;
        T y;
        T z;

        constexpr JVector3() noexcept = default;
        constexpr JVector3(T x, T y) noexcept : x(x), y(y), z() { }
        constexpr JVector3(T x, T y, T z) noexcept : x(x), y(y), z(z) { }
        constexpr JVector3(const JVector2<T, P>& vec);
        constexpr JVector3(const JVector4<T, P>& vec);

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

    template<typename T, typename P> struct JVector2;
    template<typename T, typename P> struct JVector3;
    template<typename T, typename P = float>
    struct JVector4 {
        T x;
        T y;
        T z;
        T w;


        constexpr JVector4() noexcept = default;
        constexpr JVector4(T x, T y) noexcept : x(x), y(y), z(), w() { }
        constexpr JVector4(T x, T y, T z) noexcept : x(x), y(y), z(z), w() { }
        constexpr JVector4(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) { }
        constexpr JVector4(const JVector2<T, P>& vec);
        constexpr JVector4(const JVector3<T, P>& vec);

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
JE_END_PACK

#pragma region JVector2
    template<typename T, typename P> FORCE_INLINE constexpr JEngine::JVector2<T, P>::JVector2(const JEngine::JVector3<T, P>& vec) noexcept : x(vec.x), y(vec.y) { }
    template<typename T, typename P> FORCE_INLINE constexpr JEngine::JVector2<T, P>::JVector2(const JEngine::JVector4<T, P>& vec) noexcept : x(vec.x), y(vec.y) { }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector2<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector2<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector2<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector2<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector2<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector2<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator-() const {
        return JVector2<T, P>(-x, -y);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector2<T, P>(x + other.x, y + other.y);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector2<T, P>(x + other.x, y + other.y);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator+(const JVector4<T, P>& other) const {
        return JVector2<T, P>(x + other.x, y + other.y);
    }

    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator-=(const JVector2<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator+=(const JVector2<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator-=(const JVector3<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator+=(const JVector3<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator-=(const JVector4<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator+=(const JVector4<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr P JVector2<T, P>::dot(const JVector2<T, P>& other) const {
        return P(x * other.x + y * other.y);
    }
    template<typename T, typename P> FORCE_INLINE constexpr P JVector2<T, P>::cross(const JVector2<T, P>& other) const {
        return P(x * other.y - y * other.x);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::getNormalized() const {
        const P mag = magnitude();
        return mag == 0 ? JVector2<T, P>(0, 0) : JVector2<T, P>(T(x / mag), T(y / mag));
    }
    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::normalize() {
        const P mag = magnitude();
        if (mag == 0) { return *this; }
        x = T(x / mag);
        y = T(y / mag);
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr P JVector2<T, P>::sqrMagnitude() const {
        return P(x * x + y * y);
    }
    template<typename T, typename P> FORCE_INLINE constexpr P JVector2<T, P>::magnitude() const {
        return P(Math::sqrt(x * x + y * y));
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator/(const P& value) const {
        return JVector2(T(x / value), T(y / value));
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector2<T, P> JVector2<T, P>::operator*(const P& value) const {
        return JVector2(T(x * value), T(y * value));
    }

    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator/=(const P& value) {
        x = T(x / value);
        y = T(y / value);
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector2<T, P>& JVector2<T, P>::operator*=(const P& value) {
        x = T(x * value);
        y = T(y * value);
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE P JVector2<T, P>::angle(const JVector2<T, P>& other) const {
        const float den = Math::sqrt(sqrMagnitude() * other.sqrMagnitude());
        if (den < 0.000001f) { return P(0); }

        const float dotP = Math::clamp(dot(other) / den, -1.0f, 1.0f);
        return P(acosf(dotP) * RAD_2_DEG);
    }
    template<typename T, typename P> FORCE_INLINE P JVector2<T, P>::signedAngle(const JVector2<T, P>& other) const {
        return P(angle(other) * Math::sign<P>(cross(other)));
    }

    template<typename T, typename P>
    struct Math::InvLerp<JVector2<T, P>, P> {
        static FORCE_INLINE constexpr P inverseLerp(const JVector2<T, P>& a, const JVector2<T, P>& b, const JVector2<T, P>& v) {
            constexpr JVector2<T, P> aB = b - a;
            constexpr JVector2<T, P> aV = v - a;
            constexpr P dotB = aB.sqrMagnitude();
            return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
        }
    };

#pragma endregion

#pragma region JVector3
    template<typename T, typename P> FORCE_INLINE constexpr JEngine::JVector3<T, P>::JVector3(const JEngine::JVector2<T, P>& vec) : x(vec.x), y(vec.y), z() { }
    template<typename T, typename P> FORCE_INLINE constexpr JEngine::JVector3<T, P>::JVector3(const JEngine::JVector4<T, P>& vec) : x(vec.x), y(vec.y), z(vec.z) { }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector3<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector3<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector3<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector3<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector3<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector3<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator-() const {
        return JVector3<T, P>(-x, -y, -z);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector3<T, P>(x + other.x, y + other.y, z);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z - other.z);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector3<T, P>(x + other.x, y + other.y, z + other.z);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z - other.z);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator+(const JVector4<T, P>& other) const {
        return JVector3<T, P>(x + other.x, y + other.y, z + other.z);
    }

    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator-=(const JVector2<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator+=(const JVector2<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator-=(const JVector3<T, P>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator+=(const JVector3<T, P>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator-=(const JVector4<T, P>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator+=(const JVector4<T, P>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z);
    }

    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z / value.z);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z * value.z);
    }

    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z / value.z);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z * value.z);
    }

    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr P JVector3<T, P>::dot(const JVector3<T, P>& other) const {
        return P(x * other.x + y * other.y + z * other.z);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::cross(const JVector3<T, P>& other) const {
        return JVector3<T, P>(y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::getNormalized() const {
        const P mag = magnitude();
        return mag == 0 ? JVector3<T, P>(0, 0, 0) : JVector3<T, P>(T(x / mag), T(y / mag), T(z / mag));
    }
    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::normalize() {
        const P mag = magnitude();
        if (mag == 0) { return *this; }
        x = T(x / mag);
        y = T(y / mag);
        z = T(z / mag);
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr P JVector3<T, P>::sqrMagnitude() const {
        return P(x * x + y * y + z * z);
    }
    template<typename T, typename P> FORCE_INLINE constexpr P JVector3<T, P>::magnitude() const {
        return P(std::sqrt(x * x + y * y + z * z));
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator/(const P& value) const {
        return JVector3(T(x / value), T(y / value), T(z / value));
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector3<T, P> JVector3<T, P>::operator*(const P& value) const {
        return JVector3(T(x * value), T(y * value), T(z * value));
    }

    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator/=(const P& value) {
        x = T(x / value);
        y = T(y / value);
        z = T(z / value);
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector3<T, P>& JVector3<T, P>::operator*=(const P& value) {
        x = T(x * value);
        y = T(y * value);
        z = T(z * value);
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE P JVector3<T, P>::angle(const JVector3<T, P>& other) const {
        const float den = std::sqrtf(sqrMagnitude() * other.sqrMagnitude());
        if (den < 0.000001f) { return P(0); }

        const float dotP = Math::clamp(dot(other) / den, -1.0f, 1.0f);
        return P(acosf(dotP) * RAD_2_DEG);
    }
    template<typename T, typename P> FORCE_INLINE P JVector3<T, P>::signedAngle(const JVector3<T, P>& other) const {
        return P(angle(other) * Math::sign(cross(other)));
    }

    template<typename T, typename P>
    struct Math::InvLerp<JVector3<T, P>, P> {
        static FORCE_INLINE constexpr P inverseLerp(const JVector3<T, P>& a, const JVector3<T, P>& b, const JVector3<T, P>& v) {
            constexpr JVector3<T, P> aB = b - a;
            constexpr JVector3<T, P> aV = v - a;
            constexpr P dotB = aB.sqrMagnitude();
            return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
        }
    };
#pragma endregion

#pragma region JVector4
    template<typename T, typename P> FORCE_INLINE constexpr JEngine::JVector4<T, P>::JVector4(const JEngine::JVector2<T, P>& vec) : x(vec.x), y(vec.y), z(), w() { }
    template<typename T, typename P> FORCE_INLINE constexpr JEngine::JVector4<T, P>::JVector4(const JEngine::JVector3<T, P>& vec) : x(vec.x), y(vec.y), z(vec.z), w() { }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector4<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector4<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector4<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector4<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> FORCE_INLINE constexpr bool JVector4<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    template<typename T, typename P> FORCE_INLINE constexpr bool JVector4<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z || w != other.w;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator-() const {
        return JVector4<T, P>(-x, -y, -z, -w);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z, w);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector4<T, P>(x + other.x, y + other.y, z, w);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z - other.z, w);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector4<T, P>(x + other.x, y + other.y, z + other.z, w);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z - other.z, w);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator+(const JVector4<T, P>& other) const {
        return JVector4<T, P>(x + other.x, y + other.y, z + other.z, w);
    }

    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator-=(const JVector2<T, P>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator+=(const JVector2<T, P>& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator-=(const JVector3<T, P>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator+=(const JVector3<T, P>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator-=(const JVector4<T, P>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator+=(const JVector4<T, P>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z, w);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z, w);
    }

    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= value.x;
        y /= value.y;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= value.x;
        y *= value.y;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z / value.z, w);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z * value.z, w);
    }

    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z / value.z, w / value.w);
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z * value.z, w * value.w);
    }

    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= value.x;
        y /= value.y;
        z /= value.z;
        w /= value.w;
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= value.x;
        y *= value.y;
        z *= value.z;
        w *= value.w;
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr P JVector4<T, P>::dot(const JVector4<T, P>& other) const {
        return P(x * other.x + y * other.y + z * other.z + w * other.w);
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::getNormalized() const {
        const P mag = magnitude();
        return mag == 0 ? JVector4<T, P>(0, 0, 0, 0) : JVector4<T, P>(T(x / mag), T(y / mag), T(z / mag), T(w / mag));
    }
    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::normalize() {
        const P mag = magnitude();
        if (mag == 0) { return *this; }
        x = T(x / mag);
        y = T(y / mag);
        z = T(z / mag);
        w = T(w / mag);
        return *this;
    }

    template<typename T, typename P> FORCE_INLINE constexpr P JVector4<T, P>::sqrMagnitude() const {
        return P(x * x + y * y + z * z + w * w);
    }
    template<typename T, typename P> FORCE_INLINE constexpr P JVector4<T, P>::magnitude() const {
        return P(Math::sqrt(x * x + y * y + z * z + w * w));
    }

    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator/(const P& value) const {
        return JVector4(T(x / value), T(y / value), T(z / value), T(w / value));
    }
    template<typename T, typename P> FORCE_INLINE constexpr JVector4<T, P> JVector4<T, P>::operator*(const P& value) const {
        return JVector4(T(x * value), T(y * value), T(z * value), T(w / value));
    }

    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator/=(const P& value) {
        x = T(x / value);
        y = T(y / value);
        z = T(z / value);
        w = T(w / value);
        return *this;
    }
    template<typename T, typename P> FORCE_INLINE JVector4<T, P>& JVector4<T, P>::operator*=(const P& value) {
        x = T(x * value);
        y = T(y * value);
        z = T(z * value);
        w = T(w * value);
        return *this;
    }

    template<typename T, typename P>
    struct Math::InvLerp<JVector4<T, P>, P> {
        static FORCE_INLINE constexpr P inverseLerp(const JVector4<T, P>& a, const JVector4<T, P>& b, const JVector4<T, P>& v) {
            constexpr JVector4<T, P> aB = b - a;
            constexpr JVector4<T, P> aV = v - a;
            constexpr P dotB = aB.sqrMagnitude();
            return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
        }
    };
#pragma endregion
}

typedef JEngine::JVector2<int32_t> JVector2i;
typedef JEngine::JVector3<int32_t> JVector3i;
typedef JEngine::JVector4<int32_t> JVector4i;

typedef JEngine::JVector2<uint32_t> JVector2u;
typedef JEngine::JVector3<uint32_t> JVector3u;
typedef JEngine::JVector4<uint32_t> JVector4u;

typedef JEngine::JVector2<float, float> JVector2f;
typedef JEngine::JVector3<float, float> JVector3f;
typedef JEngine::JVector4<float, float> JVector4f;

typedef JEngine::JVector2<double, double> JVector2d;
typedef JEngine::JVector3<double, double> JVector3d;
typedef JEngine::JVector4<double, double> JVector4d;