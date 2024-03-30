#pragma once
#include <cstdint>

namespace JEngine {
    struct JColor64;
    struct JColor;
    struct JColor48 {
        uint16_t r;
        uint16_t g;
        uint16_t b;
 
        constexpr JColor48() : r(0), g(0), b(0) {}
        constexpr JColor48(uint16_t gray) : r(gray), g(gray), b(gray) {}
        constexpr JColor48(uint16_t r, uint16_t g, uint16_t b) : r(r), g(g), b(b) {}

        JColor48(const JColor64& rgb);
        JColor48(const JColor& rgba);

        operator uint64_t() const { return uint64_t(r | (g << 16) | (b << 32)); }

        void set(uint16_t gray);
        void set(uint16_t gray, uint16_t alpha);

        void set(uint16_t r, uint16_t g, uint16_t b);
        void set(uint16_t r, uint16_t g, uint16_t b, uint16_t a);

        void flipRB();
    };

    struct Colors48 {
        static constexpr JColor48 White{0xFFFF, 0xFFFF, 0xFFFF};
        static constexpr JColor48 Black{0x00, 0x00, 0x00};
        static constexpr JColor48 Gray{0x7FFF, 0x7FFF, 0x7FFF};
        static constexpr JColor48 Red{0xFFFF, 0x00, 0x00};
        static constexpr JColor48 Green{0x00, 0xFFFF, 0x00};
        static constexpr JColor48 Blue{0x00, 0x00, 0xFFFF};
        static constexpr JColor48 Magenta{0xFFFF, 0x00, 0xFFFF};
        static constexpr JColor48 Yellow{0xFFFF, 0xFFFF, 0x00};
        static constexpr JColor48 Cyan{0x00, 0xFFFF, 0xFFFF};
    };
}