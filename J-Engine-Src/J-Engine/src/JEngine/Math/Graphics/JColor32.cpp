#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Math.h>
#include <algorithm>

namespace JEngine {

    JColor32::JColor32() : r(0), g(0), b(0), a(0) { }
    JColor32::JColor32(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b), a(0xFF) { }
    JColor32::JColor32(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a) { }

    JColor32::JColor32(const JColor24& rgba) :
        r(r),
        g(g),
        b(b),
        a(0xFF) { }

    JColor32::JColor32(const JColor& rgba) :
        r(Math::scalarToUInt<uint8_t, float>(rgba.r)),
        g(Math::scalarToUInt<uint8_t, float>(rgba.g)),
        b(Math::scalarToUInt<uint8_t, float>(rgba.b)),
        a(Math::scalarToUInt<uint8_t, float>(rgba.a)) { }

    JColor32::operator JColor24() {
        return JColor24(
            r,
            g,
            b);
    }

    JColor32::operator JColor() {
        return JColor(
            Math::uintToScalar<uint8_t, float>(r), 
            Math::uintToScalar<uint8_t, float>(g), 
            Math::uintToScalar<uint8_t, float>(b),
            Math::uintToScalar<uint8_t, float>(a));
    }

    const bool JColor32::operator==(const JColor32& other) const {
        return *reinterpret_cast<const uint32_t*>(this) == *reinterpret_cast<const uint32_t*>(&other);
    }

    const bool JColor32::operator!=(const JColor32& other) const {
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
            Math::multiplyBytes(r, other.r),
            Math::multiplyBytes(g, other.g),
            Math::multiplyBytes(b, other.b),
            Math::multiplyBytes(a, other.a));
    }

    JColor32& JColor32::operator*=(const JColor32& other) {
        r = Math::multiplyBytes(r, other.r);
        g = Math::multiplyBytes(g, other.g);
        b = Math::multiplyBytes(b, other.b);
        a = Math::multiplyBytes(a, other.a);
        return *this;
    }

    const bool JColor32::deserializeJson(json& jsonF) {
        r = Math::clamp(jsonF.value("r", 0x00), 0x00, 0xFF);
        g = Math::clamp(jsonF.value("g", 0x00), 0x00, 0xFF);
        b = Math::clamp(jsonF.value("b", 0x00), 0x00, 0xFF);
        a = Math::clamp(jsonF.value("a", 0x00), 0x00, 0xFF);
        return true;
    }

    const bool JColor32::serializeJson(json& jsonF) const {
        jsonF["r"] = r;
        jsonF["g"] = g;
        jsonF["b"] = b;
        jsonF["a"] = a;
        return true;
    }

    const bool JColor32::jsonToBinary(json& jsonF, std::ostream& stream) {
        JColor32 temp;
        temp.deserializeJson(jsonF);
        return temp.serializeBinary(stream);
    }
}