#pragma once
#include <cstdint>
#include <ostream>

namespace JEngine {
    struct JColor;
    struct JColor32;
    struct JColor555;
    struct JColor565;
    struct JColor24 {
        uint8_t r;
        uint8_t g;
        uint8_t b;

        constexpr JColor24() : r(0), g(0), b(0) {}
        constexpr JColor24(uint8_t gray) : r(gray), g(gray), b(gray) {}
        constexpr JColor24(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
     
        JColor24(const JColor& rgba);
        JColor24(const JColor32& rgba);

        JColor24(const JColor555& rgb);
        JColor24(const JColor565& rgb);

        operator uint32_t() const;
        operator JColor32() const;
        operator JColor() const;

        void set(const JColor& rgba);
        void set(const JColor32& rgba);
        void set(uint8_t gray);
        void set(uint8_t r, uint8_t g, uint8_t b);

        bool operator==(JColor24 other) const;
        bool operator!=(JColor24 other) const;

        JColor24 operator-(JColor24 other) const;
        JColor24 operator+(JColor24 other) const;

        JColor24& operator-=(JColor24 other);
        JColor24& operator+=(JColor24 other);

        JColor24 operator/(float value) const;
        JColor24 operator*(float value) const;

        JColor24& operator/=(float value);
        JColor24& operator*=(float value);

        JColor24 operator*(JColor24 other) const;
        JColor24& operator*=(JColor24 other);

        void flipRB();
    };

    inline std::ostream& operator<<(std::ostream& os, const JColor24& rgb) {
        os << "RGB: (" << int32_t(rgb.r) << ", " << int32_t(rgb.g) << ", " << int32_t(rgb.b) << ")";
        return os;
    }

    struct Colors24 {
        static constexpr JColor24 White{0xFF, 0xFF, 0xFF};
        static constexpr JColor24 Black{0x00, 0x00, 0x00};
        static constexpr JColor24 Gray{0x7F, 0x7F, 0x7F};
        static constexpr JColor24 Red{0xFF, 0x00, 0x00};
        static constexpr JColor24 Green{0x00, 0xFF, 0x00};
        static constexpr JColor24 Blue{0x00, 0x00, 0xFF};
        static constexpr JColor24 Magenta{0xFF, 0x00, 0xFF};
        static constexpr JColor24 Yellow{0xFF, 0xFF, 0x00};
        static constexpr JColor24 Cyan{0x00, 0xFF, 0xFF};
    };
 }