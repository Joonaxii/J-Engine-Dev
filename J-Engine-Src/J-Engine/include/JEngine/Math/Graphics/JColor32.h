#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>
#include <cstdint>

namespace JEngine {
    struct JColor;
    struct JColor24;
    struct JColor32 : public ISerializable<JColor32, sizeof(uint8_t) * 4> {
        static const JColor32 White;
        static const JColor32 Black;
        static const JColor32 Gray;
        static const JColor32 Clear;
        static const JColor32 Red;
        static const JColor32 Green;
        static const JColor32 Blue;
        static const JColor32 Magenta;
        static const JColor32 Yellow;
        static const JColor32 Cyan;

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        JColor32();
        JColor32(const uint8_t r, const uint8_t g, const uint8_t b);
        JColor32(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

        JColor32(const JColor& rgba);
        JColor32(const JColor24& rgba);

        operator JColor24();
        operator JColor();

        const bool operator==(const JColor32& other) const;
        const bool operator!=(const JColor32& other) const;

        JColor32 operator-(const JColor32& other) const;
        JColor32 operator+(const JColor32& other) const;

        JColor32& operator-=(const JColor32& other);
        JColor32& operator+=(const JColor32& other);

        JColor32 operator/(const float value) const;
        JColor32 operator*(const float value) const;

        JColor32& operator/=(const float value);
        JColor32& operator*=(const float value);

        JColor32 operator*(const JColor32& other) const;
        JColor32& operator*=(const JColor32& other);

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json& jsonF) const;
        static const bool jsonToBinary(json& jsonF, std::ostream& stream);
    };

    inline std::ostream& operator<<(std::ostream& os, const JColor32& rgba) {
        os << "RGBA: (" << int32_t(rgba.r) << ", " << int32_t(rgba.g) << ", " << int32_t(rgba.b) << ", " << int32_t(rgba.a) << ")";
        return os;
    }

    inline const JColor32 JColor32::Clear = JColor32(0x00, 0x00, 0x00, 0x00);
    inline const JColor32 JColor32::White = JColor32(0xFF, 0xFF, 0xFF, 0xFF);
    inline const JColor32 JColor32::Black = JColor32(0x00, 0x00, 0x00, 0xFF);
    inline const JColor32 JColor32::Gray = JColor32(0x7F, 0x7F, 0x7F, 0xFF);

    inline const JColor32 JColor32::Red = JColor32(0xFF, 0x00, 0x00, 0xFF);
    inline const JColor32 JColor32::Green = JColor32(0x00, 0xFF, 0x00, 0xFF);
    inline const JColor32 JColor32::Blue = JColor32(0x00, 0x00, 0xFF, 0xFF);
    inline const JColor32 JColor32::Magenta = JColor32(0xFF, 0x00, 0xFF, 0xFF);
    inline const JColor32 JColor32::Yellow = JColor32(0xFF, 0xFF, 0x00, 0xFF);
    inline const JColor32 JColor32::Cyan = JColor32(0x00, 0xFF, 0xFF, 0xFF);

}