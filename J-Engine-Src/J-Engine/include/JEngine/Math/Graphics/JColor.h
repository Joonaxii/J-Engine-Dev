#pragma once
#include <JEngine/IO/Serialization/Serialize.h>

namespace JEngine {
    struct JColor24;
    struct JColor32;
    struct JColor {
        float r;
        float g;
        float b;
        float a;

        constexpr JColor() : r(0), g(0), b(0), a(0) { }
        constexpr JColor(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) { }
        constexpr JColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) { }
        JColor(const JColor24& rgb);
        JColor(const JColor24& rgb, float alpha);
        JColor(const JColor32& rgba);

        bool operator==(const JColor& other) const;
        bool operator!=(const JColor& other) const;

        void set(const JColor32& rgba);
        void set(const JColor24& rgb);
        void set(float r, float g,float b);
        void set(float r, float g,float b, float a);

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
    };

    namespace Colors {
        static constexpr JColor White(1.0f, 1.0f, 1.0f, 1.0f);
        static constexpr JColor Black(0.0f, 0.0f, 0.0f, 1.0f);
        static constexpr JColor Gray(0.5f, 0.5f, 0.5f, 1.0f);
        static constexpr JColor Clear(0.0f, 0.0f, 0.0f, 0.0f);
        static constexpr JColor Red(1.0f, 0.0f, 0.0f, 1.0f);
        static constexpr JColor Green(0.0f, 1.0f, 0.0f, 1.0f);
        static constexpr JColor Blue(0.0f, 0.0f, 1.0f, 1.0f);
        static constexpr JColor Magenta(1.0f, 0.0f, 1.0f, 1.0f);
        static constexpr JColor Yellow(1.0f, 1.0f, 0.0f, 1.0f);
        static constexpr JColor Cyan(0.0f, 1.0f, 1.0f, 1.0f);
    }
}
DEFINE_STYLED_SERIALIZABLE_TYPE(JEngine::JColor, JEngine::ValueType::VTYPE_COLOR, JEngine::Gui::Styles::Color)
