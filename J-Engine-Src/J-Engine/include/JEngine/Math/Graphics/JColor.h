#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>

namespace JEngine {
    struct JColor24;
    struct JColor32;
    struct JColor : public ISerializable<JColor, sizeof(float) * 4> {
        static const JColor White;
        static const JColor Black;
        static const JColor Gray;
        static const JColor Clear;
        static const JColor Red;
        static const JColor Green;
        static const JColor Blue;
        static const JColor Magenta;
        static const JColor Yellow;
        static const JColor Cyan;
        
        float r;
        float g;
        float b;
        float a;

        JColor();
        JColor(const float r, const float g, const float b);
        JColor(const float r, const float g, const float b, const float a);
        JColor(const JColor24& rgb);
        JColor(const JColor32& rgba);

        const bool operator==(const JColor& other) const;
        const bool operator!=(const JColor& other) const;

        operator JColor32();
        operator JColor24();

        JColor operator-(const JColor& other) const;
        JColor operator+(const JColor& other) const;

        JColor& operator-=(const JColor& other);
        JColor& operator+=(const JColor& other);

        JColor operator/(const float value) const;
        JColor operator*(const float value) const;

        JColor& operator/=(const float value);
        JColor& operator*=(const float value);

        JColor operator/(const JColor& other) const;
        JColor operator*(const JColor& other) const;

        JColor& operator/=(const JColor& other);
        JColor& operator*=(const JColor& other);

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json & jsonF) const;
        static const bool jsonToBinary(json& jsonF, std::ostream& stream);
    };

    inline std::ostream& operator<<(std::ostream& os, const JColor& rgba) {
        os << "RGBA: (" << rgba.r << ", " << rgba.g << ", " << rgba.b << ", " << rgba.a << ")";
        return os;
    }

    inline const JColor JColor::Clear = JColor(0.0f, 0.0f, 0.0f, 0.0f);
    inline const JColor JColor::White = JColor(1.0f, 1.0f, 1.0f, 1.0f);
    inline const JColor JColor::Black = JColor(0.0f, 0.0f, 0.0f, 1.0f);
    inline const JColor JColor::Gray = JColor(0.5f, 0.5f, 0.5f, 1.0f);

    inline const JColor JColor::Red = JColor(1.0f, 0.0f, 0.0f, 1.0f);
    inline const JColor JColor::Green = JColor(0.0f, 1.0f, 0.0f, 1.0f);
    inline const JColor JColor::Blue = JColor(0.0f, 0.0f, 1.0f, 1.0f);
    inline const JColor JColor::Magenta = JColor(1.0f, 0.0f, 1.0f, 1.0f);
    inline const JColor JColor::Yellow = JColor(1.0f, 1.0f, 0.0f, 1.0f);
    inline const JColor JColor::Cyan = JColor(0.0f, 1.0f, 1.0f, 1.0f);

}