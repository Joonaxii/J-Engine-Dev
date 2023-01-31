#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <cstdint>

namespace JEngine {
    struct JColor;
    struct JColor32;
    struct JColor24 {
        static const JColor24 White;
        static const JColor24 Black;
        static const JColor24 Gray;
        static const JColor24 Red;
        static const JColor24 Green;
        static const JColor24 Blue;
        static const JColor24 Magenta;
        static const JColor24 Yellow;
        static const JColor24 Cyan;

        uint8_t r;
        uint8_t g;
        uint8_t b;

        JColor24();
        JColor24(const uint8_t r, const uint8_t g, const uint8_t b);
     
        JColor24(const JColor& rgba);
        JColor24(const JColor32& rgba);

        operator JColor32();
        operator JColor();

        const bool operator==(const JColor24& other) const;
        const bool operator!=(const JColor24& other) const;

        JColor24 operator-(const JColor24& other) const;
        JColor24 operator+(const JColor24& other) const;

        JColor24& operator-=(const JColor24& other);
        JColor24& operator+=(const JColor24& other);

        JColor24 operator/(const float value) const;
        JColor24 operator*(const float value) const;

        JColor24& operator/=(const float value);
        JColor24& operator*=(const float value);

        JColor24 operator*(const JColor24& other) const;
        JColor24& operator*=(const JColor24& other);

    };

    inline std::ostream& operator<<(std::ostream& os, const JColor24& rgb) {
        os << "RGB: (" << int32_t(rgb.r) << ", " << int32_t(rgb.g) << ", " << int32_t(rgb.b) << ")";
        return os;
    }

    inline const JColor24 JColor24::White = JColor24(0xFF, 0xFF, 0xFF);
    inline const JColor24 JColor24::Black = JColor24(0x00, 0x00, 0x00);
    inline const JColor24 JColor24::Gray = JColor24(0x7F, 0x7F, 0x7F);

    inline const JColor24 JColor24::Red = JColor24(0xFF, 0x00, 0x00);
    inline const JColor24 JColor24::Green = JColor24(0x00, 0xFF, 0x00);
    inline const JColor24 JColor24::Blue = JColor24(0x00, 0x00, 0xFF);
    inline const JColor24 JColor24::Magenta = JColor24(0xFF, 0x00, 0xFF);
    inline const JColor24 JColor24::Yellow = JColor24(0xFF, 0xFF, 0x00);
    inline const JColor24 JColor24::Cyan = JColor24(0x00, 0xFF, 0xFF);

#pragma region Serialization
    template<>
    inline const bool Serializable<JColor24>::deserializeJson(JColor24& itemRef, json& jsonF, const JColor24& defaultValue) {
        Serialization::deserialize(itemRef.r, jsonF["r"], defaultValue.r);
        Serialization::deserialize(itemRef.g, jsonF["g"], defaultValue.g);
        Serialization::deserialize(itemRef.b, jsonF["b"], defaultValue.b);
        return true;
    }

    template<>
    inline const bool Serializable<JColor24>::serializeJson(const JColor24& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.r, jsonF["r"]);
        Serialization::serialize(itemRef.g, jsonF["g"]);
        Serialization::serialize(itemRef.b, jsonF["b"]);
        return true;
    }
#pragma endregion
}