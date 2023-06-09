#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/IO/ImageUtils.h>
#include <JEngine/Math/Math.h>
#include <algorithm>

namespace JEngine {

    JColor24::JColor24() : r(0), g(0), b(0) { }
    JColor24::JColor24(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b) { }


    JColor24::JColor24(const JColor& rgba) :
        r(Math::scalarToUInt<uint8_t, float>(rgba.r)),
        g(Math::scalarToUInt<uint8_t, float>(rgba.g)),
        b(Math::scalarToUInt<uint8_t, float>(rgba.b)) {
    }

    JColor24::JColor24(const JColor32& rgba) :
        r(rgba.r),
        g(rgba.g),
        b(rgba.b) {
    }

    JColor24::JColor24(const JColor555& rgb) {
        unpackRGB555(rgb.data, r, g, b);
    }

    JColor24::JColor24(const JColor565& rgb) {
        unpackRGB565(rgb.data, r, g, b);
    }


    JColor24::operator uint32_t() const {
        return uint32_t(r) | (g << 8) | (b << 16);
    }

    JColor24::operator JColor32() const {
        return JColor32(r, g, b);
    }

    JColor24::operator JColor() const {
        return JColor(
            Math::uintToScalar<uint8_t, float>(r),
            Math::uintToScalar<uint8_t, float>(g),
            Math::uintToScalar<uint8_t, float>(b),
            1.0f);
    }

    void JColor24::set(const JColor& rgba) {
        r = Math::scalarToUInt<uint8_t, float>(rgba.r);
        g = Math::scalarToUInt<uint8_t, float>(rgba.g);
        b = Math::scalarToUInt<uint8_t, float>(rgba.b);
    }

    void JColor24::set(const JColor32& rgb) {
        r = rgb.r;
        g = rgb.g;
        b = rgb.b;
    }

    void JColor24::set(const uint8_t r, const uint8_t g, const uint8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    bool JColor24::operator==(const JColor24& other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    bool JColor24::operator!=(const JColor24& other) const {
        return !(*this == other);
    }

    JColor24 JColor24::operator -(const JColor24& other) const {
        const uint8_t rR = std::max(int32_t(r) - other.r, 0x00);
        const uint8_t gR = std::max(int32_t(g) - other.g, 0x00);
        const uint8_t bR = std::max(int32_t(b) - other.b, 0x00);
        return JColor24(rR, gR, bR);
    }
    JColor24 JColor24::operator +(const JColor24& other) const {
        const uint8_t rR = std::min(int32_t(r) + other.r, 0xFF);
        const uint8_t gR = std::min(int32_t(g) + other.g, 0xFF);
        const uint8_t bR = std::min(int32_t(b) + other.b, 0xFF);
        return JColor24(rR, gR, bR);
    }

    JColor24& JColor24::operator -=(const JColor24& other) {
        r = std::max(int32_t(r) - other.r, 0x00);
        g = std::max(int32_t(g) - other.g, 0x00);
        b = std::max(int32_t(b) - other.b, 0x00);
        return *this;
    }
    JColor24& JColor24::operator +=(const JColor24& other) {
        r = std::min(int32_t(r) + other.r, 0xFF);
        g = std::min(int32_t(g) + other.g, 0xFF);
        b = std::min(int32_t(b) + other.b, 0xFF);
        return *this;
    }

    JColor24 JColor24::operator /(const float value) const {
        const uint8_t rR = Math::clamp(int32_t(r / value), 0x00, 0xFF);
        const uint8_t gR = Math::clamp(int32_t(g / value), 0x00, 0xFF);
        const uint8_t bR = Math::clamp(int32_t(b / value), 0x00, 0xFF);
        return JColor24(rR, gR, bR);
    }

    JColor24 JColor24::operator*(const float value) const {
        const uint8_t rR = Math::clamp(int32_t(r * value), 0x00, 0xFF);
        const uint8_t gR = Math::clamp(int32_t(g * value), 0x00, 0xFF);
        const uint8_t bR = Math::clamp(int32_t(b * value), 0x00, 0xFF);
        return JColor24(rR, gR, bR);
    }

    JColor24& JColor24::operator /= (const float value) {
        r = Math::clamp(int32_t(r / value), 0x00, 0xFF);
        g = Math::clamp(int32_t(g / value), 0x00, 0xFF);
        b = Math::clamp(int32_t(b / value), 0x00, 0xFF);
        return *this;
    }
    JColor24& JColor24::operator*=(const float value) {
        r = Math::clamp(int32_t(r * value), 0x00, 0xFF);
        g = Math::clamp(int32_t(g * value), 0x00, 0xFF);
        b = Math::clamp(int32_t(b * value), 0x00, 0xFF);
        return *this;
    }

    JColor24 JColor24::operator*(const JColor24& other) const {
        return JColor24(
            Math::multByte(r, other.r),
            Math::multByte(g, other.g),
            Math::multByte(b, other.b));
    }

    JColor24& JColor24::operator*=(const JColor24& other) {
        r = Math::multByte(r, other.r);
        g = Math::multByte(g, other.g);
        b = Math::multByte(b, other.b);
        return *this;
    }
}