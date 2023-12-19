#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
    JColor::JColor(const JColor24& rgb) :
        r(Math::uintToScalar<uint8_t, float>(rgb.r)),
        g(Math::uintToScalar<uint8_t, float>(rgb.g)),
        b(Math::uintToScalar<uint8_t, float>(rgb.b)),
        a(1.0f) {
    }

    JColor::JColor(const JColor24& rgb, const float alpha) :
        r(Math::uintToScalar<uint8_t, float>(rgb.r)),
        g(Math::uintToScalar<uint8_t, float>(rgb.g)),
        b(Math::uintToScalar<uint8_t, float>(rgb.b)),
        a(alpha) { }


    JColor::JColor(const JColor32& rgba) :
        r(Math::uintToScalar<uint8_t, float>(rgba.r)),
        g(Math::uintToScalar<uint8_t, float>(rgba.g)),
        b(Math::uintToScalar<uint8_t, float>(rgba.b)),
        a(Math::uintToScalar<uint8_t, float>(rgba.a)) {
    }

    bool JColor::operator==(const JColor& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool JColor::operator!=(const JColor& other) const {
        return !(*this == other);
    }

    void JColor::set(const JColor32& rgba) {
        r = Math::uintToScalar<uint8_t, float>(rgba.r);
        g = Math::uintToScalar<uint8_t, float>(rgba.g);
        b = Math::uintToScalar<uint8_t, float>(rgba.b);
        a = Math::uintToScalar<uint8_t, float>(rgba.a);
    }

    void JColor::set(const JColor24& rgb) {
        r = Math::uintToScalar<uint8_t, float>(rgb.r);
        g = Math::uintToScalar<uint8_t, float>(rgb.g);
        b = Math::uintToScalar<uint8_t, float>(rgb.b);
    }

    void JColor::set(const float r, const float g, const float b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void JColor::set(const float r, const float g, const float b, const float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
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