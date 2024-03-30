#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/Math/Graphics/JColor4444.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/IO/ImageUtils.h>
#include <JEngine/Math/Math.h>
#include <algorithm>

namespace JEngine {

    JColor32::JColor32(const JColor24& rgb) :
        r(rgb.r),
        g(rgb.g),
        b(rgb.b),
        a(0xFF) { }

    JColor32::JColor32(const JColor24& rgb, uint8_t alpha) :
        r(rgb.r),
        g(rgb.g),
        b(rgb.b),
        a(alpha) { }

    JColor32::JColor32(const JColor& rgba) :
        r(Math::scalarToUInt<uint8_t, float>(rgba.r)),
        g(Math::scalarToUInt<uint8_t, float>(rgba.g)),
        b(Math::scalarToUInt<uint8_t, float>(rgba.b)),
        a(Math::scalarToUInt<uint8_t, float>(rgba.a)) { }


    JColor32::JColor32(const JColor555& rgba) : JColor32(rgba, 0xFF) { }
    JColor32::JColor32(const JColor555& rgba, uint8_t alpha) :
        r(rgba.getR_UI8()), g(rgba.getG_UI8()), b(rgba.getB_UI8()), a(alpha) {}

    JColor32::JColor32(const JColor565& rgb) : JColor32(rgb, 0xFF) { }
    JColor32::JColor32(const JColor565& rgb, uint8_t alpha) :
        r(rgb.getR_UI8()), g(rgb.getG_UI8()), b(rgb.getB_UI8()), a(alpha) {}
    
    JColor32::JColor32(const JColor4444& rgba) : JColor32(rgba, rgba.getA_UI8()) { }
    JColor32::JColor32(const JColor4444& rgba, uint8_t alpha) :
        r(rgba.getR_UI8()), g(rgba.getG_UI8()), b(rgba.getB_UI8()), a(alpha) {}


    JColor32::operator JColor24() const {
        return JColor24(
            r,
            g,
            b);
    }

    JColor32::operator JColor() const {
        return JColor(
            Math::uintToScalar<uint8_t, float>(r), 
            Math::uintToScalar<uint8_t, float>(g), 
            Math::uintToScalar<uint8_t, float>(b),
            Math::uintToScalar<uint8_t, float>(a));
    }

    void JColor32::set(const JColor& rgba) {
        r = Math::scalarToUInt<uint8_t, float>(rgba.r);
        g = Math::scalarToUInt<uint8_t, float>(rgba.g);
        b = Math::scalarToUInt<uint8_t, float>(rgba.b);
        a = Math::scalarToUInt<uint8_t, float>(rgba.a);
    }

    void JColor32::set(const JColor24& rgb) {
        r = rgb.r;
        g = rgb.g;
        b = rgb.b;
    }

    void JColor32::set(uint8_t gray) {
        r = gray;
        g = gray;
        b = gray;
    }

    void JColor32::set(uint8_t gray, uint8_t alpha) {
        r = gray;
        g = gray;
        b = gray;
        a = alpha;
    }

    void JColor32::set(uint8_t r, uint8_t g, uint8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void JColor32::set(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    bool JColor32::operator==(const JColor24 & other) const {
        return (*reinterpret_cast<const uint32_t*>(this) & 0xFFFFFFU) == uint32_t(other);
    };
    bool JColor32::operator!=(const JColor24& other) const {
        return (*reinterpret_cast<const uint32_t*>(this) & 0xFFFFFFU) != uint32_t(other);
    };

    bool JColor32::operator==(const JColor32& other) const {
        return *reinterpret_cast<const uint32_t*>(this) == *reinterpret_cast<const uint32_t*>(&other);
    }

    bool JColor32::operator!=(const JColor32& other) const {
        return *reinterpret_cast<const uint32_t*>(this) != *reinterpret_cast<const uint32_t*>(&other);
    }

    JColor32 JColor32::operator - (const JColor32& other) const {
        const uint8_t rR = std::max(int32_t(r) - other.r, 0x00);
        const uint8_t gR = std::max(int32_t(g) - other.g, 0x00);
        const uint8_t bR = std::max(int32_t(b) - other.b, 0x00);
        const uint8_t aR = std::max(int32_t(a) - other.a, 0x00);
        return JColor32(rR, gR, bR, aR);
    }
    JColor32 JColor32::operator + (const JColor32& other) const {
        const uint8_t rR = std::min(int32_t(r) + other.r, 0xFF);
        const uint8_t gR = std::min(int32_t(g) + other.g, 0xFF);
        const uint8_t bR = std::min(int32_t(b) + other.b, 0xFF);
        const uint8_t aR = std::min(int32_t(a) + other.a, 0xFF);
        return JColor32(rR, gR, bR, aR);
    }

    JColor32& JColor32::operator -= (const JColor32& other) {
        r = std::max(int32_t(r) - other.r, 0x00);
        g = std::max(int32_t(g) - other.g, 0x00);
        b = std::max(int32_t(b) - other.b, 0x00);
        a = std::max(int32_t(a) - other.a, 0x00);
        return *this;
    }
    JColor32& JColor32::operator += (const JColor32& other) {
        r = std::min(int32_t(r) + other.r, 0xFF);
        g = std::min(int32_t(g) + other.g, 0xFF);
        b = std::min(int32_t(b) + other.b, 0xFF);
        a = std::min(int32_t(a) + other.a, 0xFF);
        return *this;
    }

    JColor32 JColor32::operator /(const float value) const {
        const uint8_t rR = Math::clamp(int32_t(r / value), 0x00, 0xFF);
        const uint8_t gR = Math::clamp(int32_t(g / value), 0x00, 0xFF);
        const uint8_t bR = Math::clamp(int32_t(b / value), 0x00, 0xFF);
        const uint8_t aR = Math::clamp(int32_t(a / value), 0x00, 0xFF);
        return JColor32(rR, gR, bR, aR);
    }

    JColor32 JColor32::operator*(const float value) const {
        const uint8_t rR = Math::clamp(int32_t(r * value), 0x00, 0xFF);
        const uint8_t gR = Math::clamp(int32_t(g * value), 0x00, 0xFF);
        const uint8_t bR = Math::clamp(int32_t(b * value), 0x00, 0xFF);
        const uint8_t aR = Math::clamp(int32_t(a * value), 0x00, 0xFF);
        return JColor32(rR, gR, bR, aR);
    }

    JColor32& JColor32::operator /= (const float value) {
        r = Math::clamp(int32_t(r / value), 0x00, 0xFF);
        g = Math::clamp(int32_t(g / value), 0x00, 0xFF);
        b = Math::clamp(int32_t(b / value), 0x00, 0xFF);
        a = Math::clamp(int32_t(a / value), 0x00, 0xFF);
        return *this;
    }
    JColor32& JColor32::operator*=(const float value) {
        r = Math::clamp(int32_t(r * value), 0x00, 0xFF);
        g = Math::clamp(int32_t(g * value), 0x00, 0xFF);
        b = Math::clamp(int32_t(b * value), 0x00, 0xFF);
        a = Math::clamp(int32_t(a * value), 0x00, 0xFF);
        return *this;
    }

    JColor32 JColor32::operator*(const JColor32& other) const {
        return JColor32(
            Math::multByte(r, other.r),
            Math::multByte(g, other.g),
            Math::multByte(b, other.b),
            Math::multByte(a, other.a));
    }

    JColor32& JColor32::operator*=(const JColor32& other) {
        r = Math::multByte(r, other.r);
        g = Math::multByte(g, other.g);
        b = Math::multByte(b, other.b);
        a = Math::multByte(a, other.a);
        return *this;
    }
}