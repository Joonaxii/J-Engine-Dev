#pragma once
#include <cstdint>
#include <ostream>

namespace JEngine {
    struct JColor;
    struct JColor24;
    struct JColor4444;
    struct JColor555;
    struct JColor565;
    struct JColor32 {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        constexpr JColor32() : r(0), g(0), b(0), a(0) { }
        constexpr JColor32(uint8_t gray) : r(gray), g(gray), b(gray), a(0xFF) { }
        constexpr JColor32(uint8_t gray, uint8_t alpha) : r(gray), g(gray), b(gray), a(alpha) { }
        constexpr JColor32(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(0xFF) { }
        constexpr JColor32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) { }

        JColor32(const JColor& rgba);
        JColor32(const JColor24& rgba);
        JColor32(const JColor24& rgb, uint8_t alpha);

        JColor32(const JColor555& rgba);
        JColor32(const JColor555& rgba, uint8_t alpha);
        JColor32(const JColor565& rgb);
        JColor32(const JColor565& rgb, uint8_t alpha);
        JColor32(const JColor4444& rgb);
        JColor32(const JColor4444& rgb, uint8_t alpha);

        operator uint32_t() const { return *reinterpret_cast<const uint32_t*>(this); }

        operator JColor24() const;
        operator JColor() const;

        void set(const JColor& rgba);
        void set(const JColor24& rgb);
        void set(uint8_t gray);
        void set(uint8_t gray, uint8_t alpha);
        void set(uint8_t r, uint8_t g, uint8_t b);
        void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        bool operator==(const JColor32& other) const;
        bool operator!=(const JColor32& other) const;

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

        bool operator==(const JColor24& other) const;
        bool operator!=(const JColor24& other) const;

        bool operator<(const JColor32& other) const {
            return *reinterpret_cast<const uint32_t*>(this) < *reinterpret_cast<const uint32_t*>(&other);
        };
        bool operator>(const JColor32& other) const {
            return *reinterpret_cast<const uint32_t*>(this) > *reinterpret_cast<const uint32_t*>(&other);
        };
        void flipRB();
    };

    inline std::ostream& operator<<(std::ostream& os, const JColor32& rgba) {
        os << "RGBA: (" << int32_t(rgba.r) << ", " << int32_t(rgba.g) << ", " << int32_t(rgba.b) << ", " << int32_t(rgba.a) << ")";
        return os;
    }

    struct Colors32 {
        static constexpr JColor32 Clear{0x00, 0x00, 0x00, 0x00};
        static constexpr JColor32 White{0xFF, 0xFF, 0xFF, 0xFF};
        static constexpr JColor32 Black{0x00, 0x00, 0x00, 0xFF};
        static constexpr JColor32 Gray{0x7F, 0x7F, 0x7F, 0xFF};
        static constexpr JColor32 Red{0xFF, 0x00, 0x00, 0xFF};
        static constexpr JColor32 Green{0x00, 0xFF, 0x00, 0xFF};
        static constexpr JColor32 Blue{0x00, 0x00, 0xFF, 0xFF};
        static constexpr JColor32 Magenta{0xFF, 0x00, 0xFF, 0xFF};
        static constexpr JColor32 Yellow{0xFF, 0xFF, 0x00, 0xFF};
        static constexpr JColor32 Cyan{0x00, 0xFF, 0xFF, 0xFF};
    };
}