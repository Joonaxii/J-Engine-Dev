#pragma once
#include <cstdint>
#include <limits>
#include <bitset>

static constexpr int32_t PAL_SIZE = 256;

constexpr float PI              = 3.14159265359f;
constexpr float DEG_2_RAD       = PI / 180.0f;
constexpr float RAD_2_DEG       = 180.0f / PI;
constexpr float UINT8_TO_FLOAT  = 1.0f / float(UINT8_MAX);
constexpr float UINT16_TO_FLOAT = 1.0f / float(UINT16_MAX);
constexpr float UINT32_TO_FLOAT = 1.0f / float(UINT32_MAX);
constexpr float UINT64_TO_FLOAT = 1.0f / float(UINT64_MAX);

namespace detail
{
    double constexpr sqrtNewtonRaphson(double x, double curr, double prev) {
        return curr == prev ? curr : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
    }

    float constexpr sqrtfNewtonRaphson(float x, float curr, float prev) {
        return curr == prev ? curr : sqrtfNewtonRaphson(x, 0.5f * (curr + x / curr), curr);
    }
};

namespace JEngine::Math {
    template<typename T, typename P = float>
    struct InvLerp {
        static constexpr P inverseLerp(const T& a, const T& b, const T& v);
    };

    template<typename T, typename P>
    inline constexpr P InvLerp<T, P>::inverseLerp(const T& a, const T& b, const T& v) {
        return a == b ? P() : P((v - a) / (b - a));
    }

#undef max
#undef min
    template<typename T, typename P = double>
    inline constexpr P normalize(T value) {
        return value < 0 ? (value / P(-std::numeric_limits<T>::min())) : (value / P(std::numeric_limits<T>::max()));
    }

    template<typename T>
    inline constexpr bool isPowerOf2(T value) { return (value & (value - 1)) == 0; }

    template<> inline constexpr bool isPowerOf2<double>(const double value) { return isPowerOf2<int64_t>(int64_t(value)); }
    template<> inline constexpr bool isPowerOf2<float>(const float value) { return isPowerOf2<int64_t>(int64_t(value)); }

    template<typename T>
    constexpr T sqrt(T x) {
        static_assert("Not Implemented for given type!");
        return T{};
    }

    template<>
    inline constexpr double sqrt<double>(double x) {
        return (x >= 0 && x < std::numeric_limits<double>::infinity()
            ? detail::sqrtNewtonRaphson(x, x, 0)
            : std::numeric_limits<double>::quiet_NaN());
    }

    template<>
    inline constexpr float sqrt<float>(float x) {
        return (x >= 0 && x < std::numeric_limits<float>::infinity()
            ? detail::sqrtfNewtonRaphson(x, x, 0)
            : std::numeric_limits<float>::quiet_NaN());
    }

    inline constexpr uint8_t multByte(uint8_t a, uint8_t b) {
        return uint8_t((uint32_t(a) * b * 0x10101U + 0x800000U) >> 24);
    }

    float easeInOutQuart(float t);
    float easeInOutCubic(float t);
    float easeOutCubic(float t);

    int32_t log2(uint64_t value);
    int32_t findFirstLSB(uint64_t value);

    template<typename T>
    size_t countBits(T value) {
        return std::bitset<sizeof(T) << 3>(value).count();
    }

    template<typename T, typename P = T>
    constexpr P sign(T value, T threshold) {
        return P(value > threshold ? 1 : value < -threshold ? -1 : 0);
    }

    constexpr inline int32_t bytesRequired(int32_t bits) {
        const int32_t bitsShft = bits >> 3;
        return ((bitsShft << 3) == bits) ? bitsShft : bitsShft + 1;
    }

    template<typename T, size_t size>
    inline constexpr T nextDivByPowOf2(T input) {
        static_assert(isPowerOf2(size) && "Size not a power of 2!");
        return (input + (size - 1)) & ~(size - 1);
    }

    template<typename T>
    int32_t potToIndex(T value) {
        return value != T(0) && isPowerOf2<T>(value) ? log2(uint64_t(value)) : -1;
    }

    template<> inline int32_t potToIndex<double>(double value) { return potToIndex<int64_t>(int64_t(value)); }
    template<> inline int32_t potToIndex<float>(float value) { return potToIndex<int64_t>(int64_t(value)); }

    template<typename T>
    inline constexpr T sign(T val) {
        return T(val > 0 ? 1 : val < 0 ? -1 : 0);
    }

    template<>
    inline constexpr int8_t sign(int8_t val) {
        return val & 0x7F;
    }

    template<>
    inline constexpr int16_t sign(int16_t val) {
        return val & 0x7FFF;
    }

    template<>
    inline constexpr int32_t sign(int32_t val) {
        return val & 0x7FFFFFFF;
    }

    template<>
    inline constexpr int64_t sign(int64_t val) {
        return val & 0x7FFFFFFFFFFFFFFFLL;
    }

    template<>
    inline constexpr uint8_t sign(uint8_t val) {
        return val;
    }

    template<>
    inline constexpr uint16_t sign(uint16_t val) {
        return val;
    }

    template<>
    inline constexpr uint32_t sign(uint32_t val) {
        return val;
    }

    template<>
    constexpr uint64_t sign(uint64_t val) {
        return val;
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

    template<typename T, typename P = float>
    constexpr T lerp(const T& a, const T& b, P t) {
        return T((P(1) - t) * a + b * t);
    }
    template<typename T, typename P = float>
    constexpr P inverseLerp(const T& a, const T& b, const T& v) {
        return InvLerp<T, P>::inverseLerp(a, b, v);
    }

    template<typename T, typename P = float>
    constexpr T remap(const T& a, const T& b, const T& aT, const T& bT, P v) {
        return lerp<T, P>(aT, bT, InvLerp<T, P>::inverseLerp(a, b, v));
    }

    template<typename T>
    constexpr inline T nextDivBy(T input, T div) {
        const int32_t divB = input % div;
        return divB ? input + (div - divB) : input;
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
    constexpr bool isInRangeExc(const T input, const T min, const T max) {
        return input >= min && input < max;
    }

    template<typename T>
    constexpr bool isInRangeInc(const T input, const T min, const T max) {
        return input >= min && input <= max;
    }
}
