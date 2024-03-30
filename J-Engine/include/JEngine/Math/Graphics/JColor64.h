#pragma once
#include <cstdint>

namespace JEngine {
    struct JColor48;
    struct JColor;
    struct JColor64 {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t a;

        constexpr JColor64() : r(0), g(0), b(0), a(0) {}
        constexpr JColor64(uint16_t gray) : r(gray), g(gray), b(gray), a(0xFFFF) {}
        constexpr JColor64(uint16_t gray, uint16_t alpha) : r(gray), g(gray), b(gray), a(alpha) {}
        constexpr JColor64(uint16_t r, uint16_t g, uint16_t b) : r(r), g(g), b(b), a(0xFFFF) {}
        constexpr JColor64(uint16_t r, uint16_t g, uint16_t b, uint16_t a) : r(r), g(g), b(b), a(a) {}

        JColor64(const JColor48& rgb);

        JColor64(const JColor& rgba);

        operator uint64_t() const { return *reinterpret_cast<const uint64_t*>(this); }

        void set(uint16_t gray);
        void set(uint16_t gray, uint16_t alpha);
        
        void set(uint16_t r, uint16_t g, uint16_t b);
        void set(uint16_t r, uint16_t g, uint16_t b, uint16_t a);

        void flipRB();
    };

    struct Colors64 {
        static constexpr JColor64 Clear{0x00, 0x00, 0x00, 0x00};
        static constexpr JColor64 White{0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
        static constexpr JColor64 Black{0x00, 0x00, 0x00, 0xFFFF};
        static constexpr JColor64 Gray{0x7FFF, 0x7FFF, 0x7FFF, 0xFFFF};
        static constexpr JColor64 Red{0xFFFF, 0x00, 0x00, 0xFFFF};
        static constexpr JColor64 Green{0x00, 0xFFFF, 0x00, 0xFFFF};
        static constexpr JColor64 Blue{0x00, 0x00, 0xFFFF, 0xFFFF};
        static constexpr JColor64 Magenta{0xFFFF, 0x00, 0xFFFF, 0xFFFF};
        static constexpr JColor64 Yellow{0xFFFF, 0xFFFF, 0x00, 0xFFFF};
        static constexpr JColor64 Cyan{0x00, 0xFFFF, 0xFFFF, 0xFFFF};
    };
}