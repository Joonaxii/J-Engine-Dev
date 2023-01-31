#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Math.h>

namespace JEngine {

    JColor::JColor() : r(0), g(0), b(0), a(0) { }
    JColor::JColor(const float r, const float g, const float b) : r(r), g(g), b(b), a(1.0f) { }
    JColor::JColor(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a) { }

    JColor::JColor(const JColor24& rgb) : 
        r(Math::uintToScalar<uint8_t, float>(rgb.r)),
        g(Math::uintToScalar<uint8_t, float>(rgb.g)),
        b(Math::uintToScalar<uint8_t, float>(rgb.b)),
        a(1.0f) {
    }

    JColor::JColor(const JColor32& rgba) :
        r(Math::uintToScalar<uint8_t, float>(rgba.r)),
        g(Math::uintToScalar<uint8_t, float>(rgba.g)),
        b(Math::uintToScalar<uint8_t, float>(rgba.b)),
        a(Math::uintToScalar<uint8_t, float>(rgba.a)) {
    }

    const bool JColor::operator==(const JColor& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    const bool JColor::operator!=(const JColor& other) const {
        return !(*this == other);
    }

    JColor::operator JColor32() {
        return JColor32(
            Math::scalarToUInt<uint8_t, float>(r),
            Math::scalarToUInt<uint8_t, float>(g),
            Math::scalarToUInt<uint8_t, float>(b),
            Math::scalarToUInt<uint8_t, float>(a));
    }

    JColor::operator JColor24() {
        return JColor24(
            Math::scalarToUInt<uint8_t, float>(r),
            Math::scalarToUInt<uint8_t, float>(g),
            Math::scalarToUInt<uint8_t, float>(b));
    }

    JColor JColor::operator-(const JColor& other) const {
        return JColor(r - other.r, g - other.g, b - other.b, a - other.a);
    }

    JColor JColor::operator+(const JColor& other) const {
        return JColor(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    JColor JColor::operator/(const float value) const {
        const float div = 1.0f / value;
        return JColor(r * div, g * div, b * div, a * div);
    }

    JColor JColor::operator*(const float value) const {
        return JColor(r * value, g * value, b * value, a * value);
    }

    JColor& JColor::operator-=(const JColor& other) {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        return *this;
    }

    JColor& JColor::operator+=(const JColor& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }

    JColor& JColor::operator/=(const float value) {
        const float div = 1.0f / value;
        r *= div;
        g *= div;
        b *= div;
        a *= div;
        return *this;
    }

    JColor& JColor::operator*=(const float value) {
        r *= value;
        g *= value;
        b *= value;
        a *= value;
        return *this;
    }

    JColor JColor::operator/(const JColor& other) const {
        return JColor(r / other.r, g / other.g, b / other.b, a / other.a);
    }

    JColor JColor::operator*(const JColor& other) const {
        return JColor(r * other.r, g * other.g, b * other.b, a * other.a);
    }

    JColor& JColor::operator/=(const JColor& other) {
        r /= other.r;
        g /= other.g;
        b /= other.b;
        a /= other.a;
        return *this;
    }

    JColor& JColor::operator*=(const JColor& other) {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        a *= other.a;

        return *this;
    }
}