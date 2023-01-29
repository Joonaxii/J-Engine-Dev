#pragma once
#include <JEngine/Math/Units/JVector.h>

constexpr double PI = 3.14159265359;
constexpr double DEG_2_RAD = PI / 180.0;
constexpr double RAD_2_DEG = 180.0 / PI;
constexpr double UINT8_TO_FLOAT = 1.0 / double(UINT8_MAX);
constexpr double UINT16_TO_FLOAT = 1.0 / double(UINT16_MAX);
constexpr double UINT32_TO_FLOAT = 1.0 / double(UINT32_MAX);
constexpr double UINT64_TO_FLOAT = 1.0 / double(UINT64_MAX);

namespace JEngine::Math {
    namespace priv {

        template<typename T>
        struct value_limits {
            static const T MAX;
            static const T MIN;
        };

        template<> inline const uint8_t value_limits<uint8_t>::MAX = UINT8_MAX;
        template<> inline const uint8_t value_limits<uint8_t>::MIN = 0;

        template<> inline const int8_t value_limits<int8_t>::MAX = INT8_MAX;
        template<> inline const int8_t value_limits<int8_t>::MIN = INT8_MIN;


        template<> inline const uint16_t value_limits<uint16_t>::MAX = UINT16_MAX;
        template<> inline const uint16_t value_limits<uint16_t>::MIN = 0;

        template<> inline const int16_t value_limits<int16_t>::MAX = INT16_MAX;
        template<> inline const int16_t value_limits<int16_t>::MIN = INT16_MIN;


        template<> inline const uint32_t value_limits<uint32_t>::MAX = UINT32_MAX;
        template<> inline const uint32_t value_limits<uint32_t>::MIN = 0;

        template<> inline const int32_t value_limits<int32_t>::MAX = INT32_MAX;
        template<> inline const int32_t value_limits<int32_t>::MIN = INT32_MIN;


        template<> inline const uint64_t value_limits<uint64_t>::MAX = UINT64_MAX;
        template<> inline const uint64_t value_limits<uint64_t>::MIN = 0;

        template<> inline const int64_t value_limits<int64_t>::MAX = INT64_MAX;
        template<> inline const int64_t value_limits<int64_t>::MIN = INT64_MIN;


        template<typename T, typename P>
        struct value_recision_impl {
            static constexpr P normalize(const T value) { return P(0); }
        };

        template<typename P>
        struct value_recision_impl<int8_t, P> {
            static constexpr P normalize(const int8_t value) { return value < 0 ? (value / P(-INT8_MIN)) : (value / P(INT8_MAX)); }
        };

        template<typename P>
        struct value_recision_impl<uint8_t, P> {
            static constexpr P normalize(const uint8_t value) { return value / P(UINT8_MAX); }
        };

        template<typename P>
        struct value_recision_impl<int16_t, P> {
            static constexpr P normalize(const int16_t value) { return value < 0 ? (value / P(-INT16_MIN)) : (value / P(INT16_MAX)); }
        };

        template<typename P>
        struct value_recision_impl<uint16_t, P> {
            static constexpr P normalize(const uint16_t value) { return value / P(UINT16_MAX); }
        };

        template<typename P>
        struct value_recision_impl<int32_t, P> {
            static constexpr P normalize(const int32_t value) { return value < 0 ? (value / P(-INT32_MIN)) : (value / P(INT32_MAX)); }
        };

        template<typename P>
        struct value_recision_impl<uint32_t, P> {
            static constexpr P normalize(const uint32_t value) { return value / P(UINT32_MAX); }
        };

        template<typename P>
        struct value_recision_impl<int64_t, P> {
            static constexpr P normalize(const int64_t value) { return value < 0 ? (value / P(-INT64_MIN)) : (value / P(INT64_MAX)); }
        };

        template<typename P>
        struct value_recision_impl<uint64_t, P> {
            static constexpr P normalize(const uint64_t value) { return value / P(UINT64_MAX); }
        };
    }


    inline constexpr uint8_t multiplyBytes(const uint8_t a, const uint8_t b) {
        return static_cast<uint8_t>((static_cast<uint16_t>(a) * static_cast<uint16_t>(b) + 0xFFU) >> 8);
    }

    const int32_t log2(uint64_t value);
    const int32_t findFirstLSB(const uint64_t value);

    template<typename T, typename P = T>
    const P sign(const T value, const T threshold) {
        return P(value > threshold ? 1 : value < -threshold ? -1 : 0);
    }

    template<typename T, typename P = double>
    constexpr P normalize(const T value) {
        return priv::value_recision_impl<T, P>::normalize(value);
    }

    template<typename T>
    const bool isPowerOf2(const T value) {
        return (value & (value - 1)) == 0;
    }

    template<>
    inline const bool isPowerOf2<double>(const double value) {
        return isPowerOf2<int64_t>(int64_t(value));
    }

    template<>
    inline const bool isPowerOf2<float>(const float value) {
        return isPowerOf2<int64_t>(int64_t(value));
    }


    template<typename T>
    const int32_t potToIndex(const T value) {
        return value != T(0) && isPowerOf2<T>(value) ? log2(uint64_t(value)) : -1;
    }

    template<>
    inline const int32_t potToIndex<double>(const double value) {
        return potToIndex<int64_t>(int64_t(value));
    }

    template<>
    inline const int32_t potToIndex<float>(const float value) {
        return potToIndex<int64_t>(int64_t(value));
    }

    template<typename T>
    constexpr T sign(const T val) {
        return T(val > 0 ? 1 : val < 0 ? -1 : 0);
    }

    template<typename T>
    constexpr T clamp01(const T value) {
        return T(value < 0 ? 0 : value > 1 ? 1 : value);
    }

    template<typename T>
    const JVector2<T> clamp01(const JVector2<T>& value) {
        return JVector2<T>(clamp01<T>(value.x), clamp01<T>(value.y));
    }

    template<typename T>
    constexpr T clamp(const T value, const T min, const T max) {
        return T(value < min ? min : value > max ? max : value);
    }

    template<typename T>
    const T abs(const T value) {
        return T(value < 0 ? -value : value);
    }

    template<typename T>
    const T repeat(const T t, const T length) {
        static_assert("Not Implemented!");
    }

    template<>
    inline const double repeat(const double t, const double length) {
        return clamp<double>(t - floor(t / length) * length, 0.0, length);
    }

    template<>
    inline const float repeat(const float t, const float length) {
        return clamp<float>(t - floor(t / length) * length, 0.0f, length);
    }

    const float dot(const JVector2f& lhs, const JVector2f& rhs);

    const float sqrMagnitude(const JVector2f& vec);
    const float magnitude(const JVector2f& vec);

    const JVector2f smoothDamp(const JVector2f& current, const JVector2f& target, JVector2f& velocity, const float smoothTime, const float delta);

    const JVector2f rotateDeg(const JVector2f& lhs, const float deg);
    const JVector2f rotateRad(const JVector2f& lhs, const float rad);

    const float angle(const JVector2f& lhs, const JVector2f& rhs);
    const float signedAngle(const JVector2f& lhs, const JVector2f& rhs);

    template<typename T>
    const T lerp(const T& a, const T& b, const double t) {
        return static_cast<T>((1.0 - t) * a + b * t);
    }

    template<>
    inline const float lerp(const float& a, const float& b, const double t) {
        return float((1.0 - t) * a + b * t);
    }

    template<>
    inline const double lerp(const double& a, const double& b, const double t) {
        return (1.0 - t) * a + b * t;
    }

    template<typename T, typename R = float>
    const R invLerp(const T a, const T b, const T v) {
        return R((v - a) / (b - a));
    }

    template<typename T>
    constexpr T deNormalize(const double value) {
        return T(0);
    }

    template<>
    inline constexpr uint8_t deNormalize<uint8_t>(const double val) {
        return uint8_t(clamp01(val) * UINT8_MAX);
    }

    template<>
    inline constexpr int8_t deNormalize<int8_t>(const double val) {
        const double temp = clamp(val, -1.0, 1.0);
        return int8_t(temp < 0 ? (temp * -INT8_MIN) : (temp * INT8_MAX));
    }

    template<>
    inline constexpr uint16_t deNormalize<uint16_t>(const double val) {
        return uint16_t(clamp01(val) * UINT16_MAX);
    }

    template<>
    inline constexpr int16_t deNormalize<int16_t>(const double val) {
        const double temp = clamp(val, -1.0, 1.0);
        return int16_t(temp < 0 ? (temp * -INT16_MIN) : (temp * INT16_MAX));
    }

    template<>
    inline constexpr uint32_t deNormalize<uint32_t>(const double val) {
        return uint32_t(clamp01(val) * UINT32_MAX);
    }

    template<>
    inline constexpr int32_t deNormalize<int32_t>(const double val) {
        const double temp = clamp(val, -1.0, 1.0);
        return int32_t(temp < 0 ? (temp * -INT32_MIN) : (temp * INT32_MAX));
    }

    template<>
    inline constexpr uint64_t deNormalize<uint64_t>(const double val) {
        return uint64_t(clamp01(val) * UINT64_MAX);
    }

    template<>
    inline constexpr int64_t deNormalize<int64_t>(const double val) {
        const double temp = clamp(val, -1.0, 1.0);
        return int64_t(temp < 0 ? (temp * -INT64_MIN) : (temp * INT64_MAX));
    }

    const float invLerp(const  JVector2f& a, const JVector2f& b, const  JVector2f& v);

    template<typename T, typename P = double>
    inline constexpr T scalarToUInt(const P input) {
        return T(clamp01<P>(input) * priv::value_limits<T>::MAX);
    }

    template<typename T, typename P = double>
    inline constexpr P uintToScalar(const T input) {
        return P(input * (P(1.0) / P(priv::value_limits<T>::MAX)));
    }
}