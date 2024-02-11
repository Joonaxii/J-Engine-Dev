#pragma once 
#include <cstdint>
#include <JEngine/Math/Math.h>

static constexpr uint16_t RED_MASK_565 = 0x1F;
static constexpr uint16_t GREEN_MASK_565 = 0x7E0;
static constexpr uint16_t BLUE_MASK_565 = 0xF800;

namespace JEngine {
    struct JColor24;
    struct JColor555;
    struct JColor32;
    struct JColor565 {
        uint16_t data{ 0 };

        constexpr JColor565() : data(0) {}
        constexpr JColor565(uint16_t value) : data(value) {}
        constexpr JColor565(uint8_t r, uint8_t g, uint8_t b) :
            data(Math::remapBitsFromUI8<uint16_t, 5>(r) | 
                (Math::remapBitsFromUI8<uint16_t, 6>(g) << 5) | 
                (Math::remapBitsFromUI8<uint16_t, 5>(b) << 11)) {}

        JColor565(const JColor555& rgb);
        JColor565(const JColor24& rgb);
        JColor565(const JColor32& rgba);

        constexpr uint8_t getR_UI8() const { return Math::remapBitsToUI8<5>(data & 0x1F); }
        constexpr uint8_t getG_UI8() const { return Math::remapBitsToUI8<5>((data >> 5) & 0x3F); }
        constexpr uint8_t getB_UI8() const { return Math::remapBitsToUI8<5>((data >> 11) & 0x1F); }

        constexpr uint8_t getR() const { return (data & 0x1F); }
        constexpr uint8_t getG() const { return ((data >> 5) & 0x3F); }
        constexpr uint8_t getB() const { return ((data >> 11) & 0x1F); }

        constexpr bool operator <(const JColor565& other) const { return data < other.data; }
        constexpr bool operator >(const JColor565& other) const { return data > other.data; }

        void flipRB();
    };

    struct Colors565 {
        static constexpr JColor565 White{ 0xFF, 0xFF, 0xFF };
        static constexpr JColor565 Black{ 0x00, 0x00, 0x00 };
        static constexpr JColor565 Gray{ 0x7F, 0x7F, 0x7F };
        static constexpr JColor565 Red{ 0xFF, 0x00, 0x00 };
        static constexpr JColor565 Green{ 0x00, 0xFF, 0x00 };
        static constexpr JColor565 Blue{ 0x00, 0x00, 0xFF };
        static constexpr JColor565 Magenta{ 0xFF, 0x00, 0xFF };
        static constexpr JColor565 Yellow{ 0xFF, 0xFF, 0x00 };
        static constexpr JColor565 Cyan{ 0x00, 0xFF, 0xFF };
    };
}