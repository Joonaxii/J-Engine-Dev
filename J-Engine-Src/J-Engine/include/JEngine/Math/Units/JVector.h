#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>

namespace JEngine {
    template<typename T, typename P> struct JVector3;
    template<typename T, typename P> struct JVector4;
    template<typename T, typename P = float>
    struct JVector2 : public ISerializable<JVector2<T>, sizeof(T) * 2> {
        T x;
        T y;

        JVector2();
        JVector2(const T x, const T y);
        JVector2(const JVector3<T, P>& vec);
        JVector2(const JVector4<T, P>& vec);

        bool operator==(const JVector2<T, P>& other) const;
        bool operator!=(const JVector2<T, P>& other) const;

        bool operator==(const JVector3<T, P>& other) const;
        bool operator!=(const JVector3<T, P>& other) const;

        bool operator==(const JVector4<T, P>& other) const;
        bool operator!=(const JVector4<T, P>& other) const;

        JVector2<T, P> operator-() const;

        JVector2<T, P> operator-(const JVector2<T, P>& other) const;
        JVector2<T, P> operator+(const JVector2<T, P>& other) const;

        JVector2<T, P> operator-(const JVector3<T, P>& other) const;
        JVector2<T, P> operator+(const JVector3<T, P>& other) const;

        JVector2<T, P> operator-(const JVector4<T, P>& other) const;
        JVector2<T, P> operator+(const JVector4<T, P>& other) const;

        JVector2<T, P>& operator-=(const JVector2<T, P>& other);
        JVector2<T, P>& operator+=(const JVector2<T, P>& other);

        JVector2<T, P>& operator-=(const JVector3<T, P>& other);
        JVector2<T, P>& operator+=(const JVector3<T, P>& other);

        JVector2<T, P>& operator-=(const JVector4<T, P>& other);
        JVector2<T, P>& operator+=(const JVector4<T, P>& other);

        JVector2<T, P> operator/(const JVector2<T, P>& value) const;
        JVector2<T, P> operator*(const JVector2<T, P>& value) const;

        JVector2<T, P>& operator/=(const JVector2<T, P>& value);
        JVector2<T, P>& operator*=(const JVector2<T, P>& value);

        JVector2<T, P> operator/(const JVector3<T, P>& value) const;
        JVector2<T, P> operator*(const JVector3<T, P>& value) const;

        JVector2<T, P>& operator/=(const JVector3<T, P>& value);
        JVector2<T, P>& operator*=(const JVector3<T, P>& value);

        JVector2<T, P> operator/(const JVector4<T, P>& value) const;
        JVector2<T, P> operator*(const JVector4<T, P>& value) const;

        JVector2<T, P>& operator/=(const JVector4<T, P>& value);
        JVector2<T, P>& operator*=(const JVector4<T, P>& value);

        JVector2<T, P> operator/(const P& value) const;
        JVector2<T, P> operator*(const P& value) const;

        JVector2<T, P>& operator/=(const P& value);
        JVector2<T, P>& operator*=(const P& value);

        P sqrMagnitude() const;
        P magnitude() const;

        P dot(const JVector2<T, P>& other) const;
        P cross(const JVector2<T, P>& other) const;

        P angle(const JVector2<T, P>& other) const;
        P signedAngle(const JVector2<T, P>& other) const;

        JVector2<T, P> getNormalized() const;
        JVector2<T, P>& normalize();

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json& jsonF) const;
        static const bool jsonToBinary(json& jsonF, std::ostream& stream);

        static const P invLerp(const JVector2<T, P>& a, const JVector2<T, P>& b, const JVector2<T, P>& v);
    };

    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector2<T, P>& vec) {
        os << "XY: (" << vec.x << ", " << vec.y << ")";
        return os;
    }

    template<typename T, typename P> struct JVector2;
    template<typename T, typename P> struct JVector4;
    template<typename T, typename P = float>
    struct JVector3 : public ISerializable<JVector3<T>, sizeof(T) * 3> {
        T x;
        T y;
        T z;

        JVector3();
        JVector3(const T x, const T y);
        JVector3(const JVector2<T, P>& vec);
        JVector3(const JVector4<T, P>& vec);
        JVector3(const T x, const T y, const T z);

        bool operator==(const JVector2<T, P>& other) const;
        bool operator!=(const JVector2<T, P>& other) const;

        bool operator==(const JVector3<T, P>& other) const;
        bool operator!=(const JVector3<T, P>& other) const;

        bool operator==(const JVector4<T, P>& other) const;
        bool operator!=(const JVector4<T, P>& other) const;

        const JVector3<T, P> operator-() const;

        const JVector3<T, P> operator-(const JVector2<T, P>& other) const;
        const JVector3<T, P> operator+(const JVector2<T, P>& other) const;

        const JVector3<T, P> operator-(const JVector3<T, P>& other) const;
        const JVector3<T, P> operator+(const JVector3<T, P>& other) const;

        const JVector3<T, P> operator-(const JVector4<T, P>& other) const;
        const JVector3<T, P> operator+(const JVector4<T, P>& other) const;

        JVector3<T, P>& operator-=(const JVector2<T, P>& other);
        JVector3<T, P>& operator+=(const JVector2<T, P>& other);

        JVector3<T, P>& operator-=(const JVector3<T, P>& other);
        JVector3<T, P>& operator+=(const JVector3<T, P>& other);

        JVector3<T, P>& operator-=(const JVector4<T, P>& other);
        JVector3<T, P>& operator+=(const JVector4<T, P>& other);

        const JVector3<T, P> operator/(const JVector2<T, P>& value) const;
        const JVector3<T, P> operator*(const JVector2<T, P>& value) const;

        JVector3<T, P>& operator/=(const JVector2<T, P>& value);
        JVector3<T, P>& operator*=(const JVector2<T, P>& value);

        JVector3<T, P> operator/(const JVector3<T, P>& value) const;
        JVector3<T, P> operator*(const JVector3<T, P>& value) const;

        JVector3<T, P>& operator/=(const JVector3<T, P>& value);
        JVector3<T, P>& operator*=(const JVector3<T, P>& value);

        const JVector3<T, P> operator/(const JVector4<T, P>& value) const;
        const JVector3<T, P> operator*(const JVector4<T, P>& value) const;

        JVector3<T, P>& operator/=(const JVector4<T, P>& value);
        JVector3<T, P>& operator*=(const JVector4<T, P>& value);

        const JVector3<T, P> operator/(const P& value) const;
        const JVector3<T, P> operator*(const P& value) const;

        JVector3<T, P>& operator/=(const P& value);
        JVector3<T, P>& operator*=(const P& value);

        const P sqrMagnitude() const;
        const P magnitude() const;

        const P dot(const JVector3<T, P>& other) const;
        const JVector3<T, P> cross(const JVector3<T, P>& other) const;

        const P angle(const JVector3<T, P>& other) const;
        const P signedAngle(const JVector3<T, P>& other) const;

        const JVector3<T, P> getNormalized() const;
        JVector3<T, P>& normalize();

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json& jsonF) const;
        static const bool jsonToBinary(json& jsonF, std::ostream& stream);

        static const P invLerp(const JVector3<T, P>& a, const JVector3<T, P>& b, const JVector3<T, P>& v);
    };

    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector3<T, P>& vec) {
        os << "XYZ: (" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

    template<typename T, typename P> struct JVector2;
    template<typename T, typename P> struct JVector3;
    template<typename T, typename P = float>
    struct JVector4 : public ISerializable<JVector4<T>, sizeof(T) * 4> {
        T x;
        T y;
        T z;
        T w;

        JVector4();
        JVector4(const T x, const T y);
        JVector4(const JVector2<T, P>& vec);
        JVector4(const JVector3<T, P>& vec);
        JVector4(const T x, const T y, const T z);
        JVector4(const T x, const T y, const T z, const T w);

        bool operator==(const JVector2<T, P>& other) const;
        bool operator!=(const JVector2<T, P>& other) const;

        bool operator==(const JVector3<T, P>& other) const;
        bool operator!=(const JVector3<T, P>& other) const;

        bool operator==(const JVector4<T, P>& other) const;
        bool operator!=(const JVector4<T, P>& other) const;

        const JVector4<T, P> operator-() const;

        const JVector4<T, P> operator-(const JVector2<T, P>& other) const;
        const JVector4<T, P> operator+(const JVector2<T, P>& other) const;

        const JVector4<T, P> operator-(const JVector3<T, P>& other) const;
        const JVector4<T, P> operator+(const JVector3<T, P>& other) const;

        const JVector4<T, P> operator-(const JVector4<T, P>& other) const;
        const JVector4<T, P> operator+(const JVector4<T, P>& other) const;

        JVector4<T, P>& operator-=(const JVector2<T, P>& other);
        JVector4<T, P>& operator+=(const JVector2<T, P>& other);

        JVector4<T, P>& operator-=(const JVector3<T, P>& other);
        JVector4<T, P>& operator+=(const JVector3<T, P>& other);

        JVector4<T, P>& operator-=(const JVector4<T, P>& other);
        JVector4<T, P>& operator+=(const JVector4<T, P>& other);

        const JVector4<T, P> operator/(const JVector2<T, P>& value) const;
        const JVector4<T, P> operator*(const JVector2<T, P>& value) const;

        JVector4<T, P>& operator/=(const JVector2<T, P>& value);
        JVector4<T, P>& operator*=(const JVector2<T, P>& value);

        JVector4<T, P> operator/(const JVector3<T, P>& value) const;
        JVector4<T, P> operator*(const JVector3<T, P>& value) const;

        JVector4<T, P>& operator/=(const JVector3<T, P>& value);
        JVector4<T, P>& operator*=(const JVector3<T, P>& value);

        const JVector4<T, P> operator/(const JVector4<T, P>& value) const;
        const JVector4<T, P> operator*(const JVector4<T, P>& value) const;

        JVector4<T, P>& operator/=(const JVector4<T, P>& value);
        JVector4<T, P>& operator*=(const JVector4<T, P>& value);

        const JVector4<T, P> operator/(const P& value) const;
        const JVector4<T, P> operator*(const P& value) const;

        JVector4<T, P>& operator/=(const P& value);
        JVector4<T, P>& operator*=(const P& value);

        const P sqrMagnitude() const;
        const P magnitude() const;

        const P dot(const JVector4<T, P>& other) const;

        const JVector4<T, P> getNormalized() const;
        JVector4<T, P>& normalize();

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json& jsonF) const;
        static const bool jsonToBinary(json& jsonF, std::ostream& stream);

        static const P invLerp(const JVector4<T, P>& a, const JVector4<T, P>& b, const JVector4<T, P>& v);
    };

    template<typename T, typename P> inline std::ostream& operator<<(std::ostream& os, const JVector4<T, P>& vec) {
        os << "XYZW: (" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return os;
    }

#pragma region JVector2
    template<typename T, typename P> inline JEngine::JVector2<T, P>::JVector2() : x(), y() { }
    template<typename T, typename P> inline JEngine::JVector2<T, P>::JVector2(const T x, const T y) : x(x), y(y) { }
    template<typename T, typename P> inline JEngine::JVector2<T, P>::JVector2(const JEngine::JVector3<T, P>& vec) : x(vec.x), y(vec.y) { }
    template<typename T, typename P> inline JEngine::JVector2<T, P>::JVector2(const JEngine::JVector4<T, P>& vec) : x(vec.x), y(vec.y) { }

    template<typename T, typename P> inline bool JVector2<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline bool JVector2<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline bool JVector2<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline bool JVector2<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline bool JVector2<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline bool JVector2<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator-() const {
        return JVector2<T, P>(-x, -y);
    }

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector2<T, P>(x + other.x, y + other.y);
    }

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector2<T, P>(x + other.x, y + other.y);
    }

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector2<T, P>(x - other.x, y - other.y);
    }
    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator+(const JVector4<T, P>& other) const {
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

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector2<T, P>(x / value.x, y / value.y);
    }
    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector2<T, P>(x * value.x, y * value.y);
    }

    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector2<T, P>& JVector2<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template<typename T, typename P> inline P JVector2<T, P>::dot(const JVector2<T, P>& other) const {
        return P(x * other.x + y * other.y);
    }
    template<typename T, typename P> inline P JVector2<T, P>::cross(const JVector2<T, P>& other) const {
        return P(x * other.y - y * other.x);
    }

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::getNormalized() const {
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

    template<typename T, typename P> inline P JVector2<T, P>::sqrMagnitude() const {
        return P(x * x + y * y);
    }
    template<typename T, typename P> inline P JVector2<T, P>::magnitude() const {
        return P(std::sqrt(x * x + y * y));
    }

    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator/(const P& value) const {
        return JVector2(T(x / value), T(y / value));
    }
    template<typename T, typename P> inline JVector2<T, P> JVector2<T, P>::operator*(const P& value) const {
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
        const float den = std::sqrtf(sqrMagnitude() * other.sqrMagnitude());
        if (den < 0.000001f) { return P(0); }

        const float dotP = Math::clamp(dot(other) / den, -1.0f, 1.0f);
        return P(acosf(dotP) * RAD_2_DEG);
    }
    template<typename T, typename P> inline P JVector2<T, P>::signedAngle(const JVector2<T, P>& other) const {
        return P(angle(other) * Math::sign(cross(other)));
    }

    template<typename T, typename P> inline  const bool JVector2<T, P>::deserializeJson(json& jsonF) {
        x = jsonF.value("x", T());
        y = jsonF.value("y", T());
        return true;
    }
    template<typename T, typename P> inline const bool JVector2<T, P>::serializeJson(json& jsonF) const {
        jsonF["x"] = x;
        jsonF["y"] = y;
        return true;
    }
    template<typename T, typename P> inline const bool JEngine::JVector2<T, P>::jsonToBinary(json& jsonF, std::ostream& stream) {
        JVector2<T, P> temp;
        temp.deserializeJson(jsonF);
        return temp.serializeBinary(jsonF);
    }

    template<typename T, typename P> inline const P JEngine::JVector2<T, P>::invLerp(const JVector2<T, P>& a, const JVector2<T, P>& b, const JVector2<T, P>& v) {
        const JVector2<T, P> aB = b - a;
        const JVector2<T, P> aV = v - a;

        const P dotB = aB.sqrMagnitude();
        return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
    }
#pragma endregion

#pragma region JVector3
    template<typename T, typename P> inline JEngine::JVector3<T, P>::JVector3() : x(), y(), z() { }
    template<typename T, typename P> inline JEngine::JVector3<T, P>::JVector3(const JEngine::JVector2<T, P>& vec) : x(vec.x), y(vec.y), z() { }
    template<typename T, typename P> inline JEngine::JVector3<T, P>::JVector3(const JEngine::JVector4<T, P>& vec) : x(vec.x), y(vec.y), z(vec.z) { }
    template<typename T, typename P> inline JEngine::JVector3<T, P>::JVector3(const T x, const T y) : x(x), y(y), z() { }
    template<typename T, typename P> inline JEngine::JVector3<T, P>::JVector3(const T x, const T y, const T z) : x(x), y(y), z(z) { }

    template<typename T, typename P> inline bool JVector3<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline bool JVector3<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline bool JVector3<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> inline bool JVector3<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> inline bool JVector3<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> inline bool JVector3<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator-() const {
        return JVector3<T, P>(-x, -y, -z);
    }

    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z);
    }
    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector3<T, P>(x + other.x, y + other.y, z);
    }

    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z - other.z);
    }
    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector3<T, P>(x + other.x, y + other.y, z + other.z);
    }

    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector3<T, P>(x - other.x, y - other.y, z - other.z);
    }
    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator+(const JVector4<T, P>& other) const {
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

    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z);
    }
    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z);
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template<typename T, typename P> inline JVector3<T, P> JVector3<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z / value.z);
    }
    template<typename T, typename P> inline JVector3<T, P> JVector3<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z * value.z);
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector3<T, P>(x / value.x, y / value.y, z / value.z);
    }
    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector3<T, P>(x * value.x, y * value.y, z * value.z);
    }

    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    template<typename T, typename P> inline JVector3<T, P>& JVector3<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    template<typename T, typename P> inline const P JVector3<T, P>::dot(const JVector3<T, P>& other) const {
        return P(x * other.x + y * other.y + z * other.z);
    }
    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::cross(const JVector3<T, P>& other) const {
        return JVector3<T, P>(y * other.z - z * other.y,
                              z * other.x - x * other.z,
                              x * other.y - y * other.x);
    }

    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::getNormalized() const {
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

    template<typename T, typename P> inline const P JVector3<T, P>::sqrMagnitude() const {
        return P(x * x + y * y + z * z);
    }
    template<typename T, typename P> inline const P JVector3<T, P>::magnitude() const {
        return P(std::sqrt(x * x + y * y + z * z));
    }

    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator/(const P& value) const {
        return JVector3(T(x / value), T(y / value), T(z / value));
    }
    template<typename T, typename P> inline const JVector3<T, P> JVector3<T, P>::operator*(const P& value) const {
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

    template<typename T, typename P> inline const P JVector3<T, P>::angle(const JVector3<T, P>& other) const {
        const float den = std::sqrtf(sqrMagnitude() * other.sqrMagnitude());
        if (den < 0.000001f) { return P(0); }

        const float dotP = Math::clamp(dot(other) / den, -1.0f, 1.0f);
        return P(acosf(dotP) * RAD_2_DEG);
    }
    template<typename T, typename P> inline const P JVector3<T, P>::signedAngle(const JVector3<T, P>& other) const {
        return P(angle(other) * Math::sign(cross(other)));
    }

    template<typename T, typename P> inline const bool JVector3<T, P>::deserializeJson(json& jsonF) {
        x = jsonF.value("x", T());
        y = jsonF.value("y", T());
        z = jsonF.value("z", T());
        return true;
    }
    template<typename T, typename P> inline const bool JVector3<T, P>::serializeJson(json& jsonF) const {
        jsonF["x"] = x;
        jsonF["y"] = y;
        jsonF["z"] = z;
        return true;
    }
    template<typename T, typename P> inline const bool JEngine::JVector3<T, P>::jsonToBinary(json& jsonF, std::ostream& stream) {
        JVector3<T, P> temp;
        temp.deserializeJson(jsonF);
        return temp.serializeBinary(jsonF);
    }

    template<typename T, typename P> inline const P JEngine::JVector3<T, P>::invLerp(const JVector3<T, P>& a, const JVector3<T, P>& b, const JVector3<T, P>& v) {
        const JVector3<T, P> aB = b - a;
        const JVector3<T, P> aV = v - a;
                     
        const P dotB = aB.sqrMagnitude();
        return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
    }
#pragma endregion

#pragma region JVector4
    template<typename T, typename P> inline JEngine::JVector4<T, P>::JVector4() : x(), y(), z(), w() { }
    template<typename T, typename P> inline JEngine::JVector4<T, P>::JVector4(const JEngine::JVector2<T, P>& vec) : x(vec.x), y(vec.y), z(), w() { }
    template<typename T, typename P> inline JEngine::JVector4<T, P>::JVector4(const JEngine::JVector3<T, P>& vec) : x(vec.x), y(vec.y), z(vec.z), w() { }
    template<typename T, typename P> inline JEngine::JVector4<T, P>::JVector4(const T x, const T y) : x(x), y(y), z(), w() { }
    template<typename T, typename P> inline JEngine::JVector4<T, P>::JVector4(const T x, const T y, const T z) : x(x), y(y), z(z), w() { }
    template<typename T, typename P> inline JEngine::JVector4<T, P>::JVector4(const T x, const T y, const T z, const T w) : x(x), y(y), z(z), w(w) { }

    template<typename T, typename P> inline bool JVector4<T, P>::operator==(const JVector2<T, P>& other) const {
        return x == other.x && y == other.y;
    }
    template<typename T, typename P> inline bool JVector4<T, P>::operator!=(const JVector2<T, P>& other) const {
        return x != other.x || y != other.y;
    }

    template<typename T, typename P> inline bool JVector4<T, P>::operator==(const JVector3<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    template<typename T, typename P> inline bool JVector4<T, P>::operator!=(const JVector3<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template<typename T, typename P> inline bool JVector4<T, P>::operator==(const JVector4<T, P>& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    template<typename T, typename P> inline bool JVector4<T, P>::operator!=(const JVector4<T, P>& other) const {
        return x != other.x || y != other.y || z != other.z || w != other.w;
    }

    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator-() const {
        return JVector4<T, P>(-x, -y, -z, -w);
    }

    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator-(const JVector2<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z, w);
    }
    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator+(const JVector2<T, P>& other) const {
        return JVector4<T, P>(x + other.x, y + other.y, z, w);
    }

    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator-(const JVector3<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z - other.z, w);
    }
    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator+(const JVector3<T, P>& other) const {
        return JVector4<T, P>(x + other.x, y + other.y, z + other.z, w);
    }

    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator-(const JVector4<T, P>& other) const {
        return JVector4<T, P>(x - other.x, y - other.y, z - other.z, w);
    }
    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator+(const JVector4<T, P>& other) const {
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

    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator/(const JVector2<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z, w);
    }
    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator*(const JVector2<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z, w);
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator/=(const JVector2<T, P>& value) {
        x /= other.x;
        y /= other.y;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator*=(const JVector2<T, P>& value) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template<typename T, typename P> inline JVector4<T, P> JVector4<T, P>::operator/(const JVector3<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z / value.z, w);
    }
    template<typename T, typename P> inline JVector4<T, P> JVector4<T, P>::operator*(const JVector3<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z * value.z, w);
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator/=(const JVector3<T, P>& value) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator*=(const JVector3<T, P>& value) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator/(const JVector4<T, P>& value) const {
        return JVector4<T, P>(x / value.x, y / value.y, z / value.z, w / value.w);
    }
    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator*(const JVector4<T, P>& value) const {
        return JVector4<T, P>(x * value.x, y * value.y, z * value.z, w * value.w);
    }

    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator/=(const JVector4<T, P>& value) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }
    template<typename T, typename P> inline JVector4<T, P>& JVector4<T, P>::operator*=(const JVector4<T, P>& value) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    template<typename T, typename P> inline const P JVector4<T, P>::dot(const JVector4<T, P>& other) const {
        return P(x * other.x + y * other.y + z * other.z + w * other.w);
    }

    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::getNormalized() const {
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

    template<typename T, typename P> inline const P JVector4<T, P>::sqrMagnitude() const {
        return P(x * x + y * y + z * z + w * w);
    }
    template<typename T, typename P> inline const P JVector4<T, P>::magnitude() const {
        return P(std::sqrt(x * x + y * y + z * z + w * w));
    }

    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator/(const P& value) const {
        return JVector4(T(x / value), T(y / value), T(z / value), T(w / value));
    }
    template<typename T, typename P> inline const JVector4<T, P> JVector4<T, P>::operator*(const P& value) const {
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

    template<typename T, typename P> inline  const bool JVector4<T, P>::deserializeJson(json& jsonF) {
        x = jsonF.value("x", T());
        y = jsonF.value("y", T());
        z = jsonF.value("z", T());
        w = jsonF.value("w", T());
        return true;
    }
    template<typename T, typename P> inline const bool JVector4<T, P>::serializeJson(json& jsonF) const {
        jsonF["x"] = x;
        jsonF["y"] = y;
        jsonF["z"] = z;
        jsonF["w"] = w;
        return true;
    }
    template<typename T, typename P> inline const bool JEngine::JVector4<T, P>::jsonToBinary(json& jsonF, std::ostream& stream) {
        JVector4<T, P> temp;
        temp.deserializeJson(jsonF);
        return temp.serializeBinary(jsonF);
    }

    template<typename T, typename P> inline const P JEngine::JVector4<T, P>::invLerp(const JVector4<T, P>& a, const JVector4<T, P>& b, const JVector4<T, P>& v) {
        const JVector4<T, P> aB = b - a;
        const JVector4<T, P> aV = v - a;

        const P dotB = aB.sqrMagnitude();
        return P(dotB == 0 ? 0 : aV.dot(aB) / dotB);
    }
#pragma endregion
}

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