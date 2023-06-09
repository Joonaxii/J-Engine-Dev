#pragma once
#include <cstdint>
#include <limits>

static constexpr int32_t PAL_SIZE = 256;

constexpr float PI              = 3.14159265359f;
constexpr float DEG_2_RAD       = PI / 180.0f;
constexpr float RAD_2_DEG       = 180.0f / PI;
constexpr float UINT8_TO_FLOAT  = 1.0f / float(UINT8_MAX);
constexpr float UINT16_TO_FLOAT = 1.0f / float(UINT16_MAX);
constexpr float UINT32_TO_FLOAT = 1.0f / float(UINT32_MAX);
constexpr float UINT64_TO_FLOAT = 1.0f / float(UINT64_MAX);

namespace JEngine::Math {
    inline constexpr uint8_t multByte(uint8_t a, uint8_t b) {
        return uint8_t((uint32_t(a) * b * 0x10101U + 0x800000U) >> 24);
    }

    float easeInOutQuart(float t);
    float easeInOutCubic(float t);
    float easeOutCubic(float t);

    int32_t log2(uint64_t value);
    int32_t findFirstLSB(const uint64_t value);

    template<typename T, typename P = T>
    P sign(const T value, const T threshold) {
        return P(value > threshold ? 1 : value < -threshold ? -1 : 0);
    }

    constexpr int32_t bytesRequired(const int32_t bits) {
        const int32_t bitsShft = bits >> 3;
        return ((bitsShft << 3) == bits) ? bitsShft : bitsShft + 1;
    }

    template<typename T, size_t size>
    constexpr inline T nextDivByPowOf2(const T input) {
        return (input + (size - 1)) & ~(size - 1);
    }

#undef max
#undef min
    template<typename T, typename P = double>
    constexpr P normalize(const T value) {
        return value < 0 ? (value / P(-std::numeric_limits<T>::min())) : (value / P(std::numeric_limits<T>::max()));
    }

    template<typename T>
    bool isPowerOf2(const T value) { return (value & (value - 1)) == 0; }

    template<> inline bool isPowerOf2<double>(const double value) { return isPowerOf2<int64_t>(int64_t(value)); }
    template<> inline bool isPowerOf2<float>(const float value) { return isPowerOf2<int64_t>(int64_t(value)); }

    template<typename T>
    int32_t potToIndex(const T value) {
        return value != T(0) && isPowerOf2<T>(value) ? log2(uint64_t(value)) : -1;
    }

    template<> inline int32_t potToIndex<double>(const double value) { return potToIndex<int64_t>(int64_t(value)); }
    template<> inline int32_t potToIndex<float>(const float value) { return potToIndex<int64_t>(int64_t(value)); }

    template<typename T>
    constexpr T sign(const T val) {
        return T(val > 0 ? 1 : val < 0 ? -1 : 0);
    }

    template<typename T>
    constexpr T clamp01(const T value) {
        return T(value < 0 ? 0 : value > 1 ? 1 : value);
    }

    template<typename T>
    constexpr T clamp(const T value, const T min, const T max) {
        return T(value < min ? min : value > max ? max : value);
    }

    template<typename T>
    constexpr T abs(const T value) {
        return T(value < 0 ? -value : value);
    }

    template<typename T>
    T repeat(const T t, const T length) {
        static_assert("Not Implemented!");
    }

    template<>
    inline double repeat(const double t, const double length) {
        return clamp<double>(t - floor(t / length) * length, 0.0, length);
    }

    template<>
    inline float repeat(const float t, const float length) {
        return clamp<float>(t - floorf(t / length) * length, 0.0f, length);
    }

    template<typename T>
    T lerp(const T& a, const T& b, const double t) {
        return static_cast<T>((1.0 - t) * a + b * t);
    }

    template<>
    inline float lerp(const float& a, const float& b, const double t) {
        return float((1.0 - t) * a + b * t);
    }

    template<>
    inline double lerp(const double& a, const double& b, const double t) {
        return (1.0 - t) * a + b * t;
    }

    template<typename T, typename R = float>
    R invLerp(const T a, const T b, const T v) {
        return R((v - a) / (b - a));
    }

    template<typename T, typename P = float>
    constexpr T deNormalize(const P value) {
        return T(value < 0 ? (value * -std::numeric_limits<T>::min()) : (value * std::numeric_limits<T>::max()));
    }

    template<typename T, typename P = double>
    inline constexpr T scalarToUInt(const P input) {
        return T(clamp01<P>(input) * std::numeric_limits<T>::max());
    }

    template<typename T, typename P = double>
    inline constexpr P uintToScalar(const T input) {
        return P(input) * (P(1.0) / P(std::numeric_limits<T>::max()));
    }

    static constexpr int32_t alignToPalette(const int32_t size) {
        return size <= PAL_SIZE ? size : size >= PAL_SIZE * PAL_SIZE ? PAL_SIZE * PAL_SIZE : (size + PAL_SIZE - 1) & -PAL_SIZE;
    }

    static constexpr bool isAlignedToPalette(const int32_t size) {
        return size <= PAL_SIZE || (size & (PAL_SIZE - 1)) == 0;
    }

    template<typename T>
    bool isInRangeExc(const T input, const T min, const T max) {
        return input >= min && input < max;
    }

    template<typename T>
    bool isInRangeInc(const T input, const T min, const T max) {
        return input >= min && input <= max;
    }
}
