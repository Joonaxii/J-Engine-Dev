#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/IO/ImageUtils.h>
#include <JEngine/Math/Math.h>
#include <algorithm>

namespace JEngine {
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

    JColor24::JColor24(const JColor555& rgb) :
        r(rgb.getR_UI8()), g(rgb.getG_UI8()), b(rgb.getB_UI8())
    {}

    JColor24::JColor24(const JColor565& rgb) :
        r(rgb.getR_UI8()), g(rgb.getG_UI8()), b(rgb.getB_UI8())
    {}

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

    void JColor32::set(uint8_t gray) {
        r = gray;
        g = gray;
        b = gray;
    }

    void JColor24::set(uint8_t r, uint8_t g, uint8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    bool JColor24::operator==(JColor24 other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    bool JColor24::operator!=(JColor24 other) const {
        return !(*this == other);
    }

    JColor24 JColor24::operator -(JColor24 other) const {
        const uint8_t rR = Math::max(int32_t(r) - other.r, 0x00);
        const uint8_t gR = Math::max(int32_t(g) - other.g, 0x00);
        const uint8_t bR = Math::max(int32_t(b) - other.b, 0x00);
        return JColor24(rR, gR, bR);
    }
    JColor24 JColor24::operator+(JColor24 other) const {
        const uint8_t rR = Math::min(int32_t(r) + other.r, 0xFF);
        const uint8_t gR = Math::min(int32_t(g) + other.g, 0xFF);
        const uint8_t bR = Math::min(int32_t(b) + other.b, 0xFF);
        return JColor24(rR, gR, bR);
    }

    JColor24& JColor24::operator-=(JColor24 other) {
        r = Math::max(int32_t(r) - other.r, 0x00);
        g = Math::max(int32_t(g) - other.g, 0x00);
        b = Math::max(int32_t(b) - other.b, 0x00);
        return *this;
    }
    JColor24& JColor24::operator+=(JColor24 other) {
        r = Math::min(int32_t(r) + other.r, 0xFF);
        g = Math::min(int32_t(g) + other.g, 0xFF);
        b = Math::min(int32_t(b) + other.b, 0xFF);
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

    JColor24& JColor24::operator/=(float value) {
        r = Math::clamp(int32_t(r / value), 0x00, 0xFF);
        g = Math::clamp(int32_t(g / value), 0x00, 0xFF);
        b = Math::clamp(int32_t(b / value), 0x00, 0xFF);
        return *this;
    }
    JColor24& JColor24::operator*=(float value) {
        r = Math::clamp(int32_t(r * value), 0x00, 0xFF);
        g = Math::clamp(int32_t(g * value), 0x00, 0xFF);
        b = Math::clamp(int32_t(b * value), 0x00, 0xFF);
        return *this;
    }

    JColor24 JColor24::operator*(JColor24 other) const {
        return JColor24(
            Math::multByte(r, other.r),
            Math::multByte(g, other.g),
            Math::multByte(b, other.b));
    }

    JColor24& JColor24::operator*=(JColor24 other) {
        r = Math::multByte(r, other.r);
        g = Math::multByte(g, other.g);
        b = Math::multByte(b, other.b);
        return *this;
    }

    void JColor24::flipRB() {
        std::swap(r, b);
    }
}