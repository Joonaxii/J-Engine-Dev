#pragma once
#include <cstdint>
#include <JEngine/Math/Math.h>

namespace JEngine {

    struct JColor565;
    struct JColor24;
    struct JColor32;
    struct JColor555 {
        static const JColor555 Clear;
        static const JColor555 White;
        static const JColor555 Black;
        static const JColor555 Gray;
        static const JColor555 Red;
        static const JColor555 Green;
        static const JColor555 Blue;
        static const JColor555 Magenta;
        static const JColor555 Yellow;
        static const JColor555 Cyan;

        uint16_t data{ 0 };

        constexpr JColor555() : data(0) {}
        constexpr JColor555(uint16_t value) : data(value) {}
        constexpr JColor555(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : 
            data(Math::remapBitsFromUI8<uint16_t, 5>(r) | 
                (Math::remapBitsFromUI8<uint16_t, 5>(g) << 5) | 
                (Math::remapBitsFromUI8<uint16_t, 5>(b) << 10) | 
                (Math::remapBitsFromUI8<uint16_t, 1>(a))) {}
        constexpr JColor555(uint8_t r, uint8_t g, uint8_t b) :
            data(Math::remapBitsFromUI8<uint16_t, 5>(r) | 
                (Math::remapBitsFromUI8<uint16_t, 5>(g) << 5) | 
                (Math::remapBitsFromUI8<uint16_t, 5>(b) << 10) | 0x8000) {}

        JColor555(const JColor565& rgb);
        JColor555(const JColor24& rgb);
        JColor555(const JColor32& rgba);

        constexpr uint8_t getR_UI8() const { return Math::remapBitsToUI8<5>(data & 0x1F); }
        constexpr uint8_t getG_UI8() const { return Math::remapBitsToUI8<5>((data >> 5) & 0x1F); }
        constexpr uint8_t getB_UI8() const { return Math::remapBitsToUI8<5>((data >> 10) & 0x1F); }
        constexpr uint8_t getA_UI8() const { return Math::remapBitsToUI8<1>((data >> 15) & 0x1); }

        constexpr uint8_t getR() const { return (data & 0x1F); }
        constexpr uint8_t getG() const { return ((data >> 5) & 0x1F); }
        constexpr uint8_t getB() const { return ((data >> 10) & 0x1F); }
        constexpr uint8_t getA() const { return ((data >> 15) & 0x1); }

        constexpr bool operator <(const JColor555& other) const { return data < other.data; }
        constexpr bool operator >(const JColor555& other) const { return data > other.data; }

        void flipRB();
    };

    struct Colors555 {
        static constexpr JColor555 Clear{0x00, 0x00, 0x00, 0x00};
        static constexpr JColor555 White{0xFF, 0xFF, 0xFF};
        static constexpr JColor555 Black{0x00, 0x00, 0x00};
        static constexpr JColor555 Gray{0x7F, 0x7F, 0x7F};
        static constexpr JColor555 Red{0xFF, 0x00, 0x00};
        static constexpr JColor555 Green{0x00, 0xFF, 0x00};
        static constexpr JColor555 Blue{0x00, 0x00, 0xFF};
        static constexpr JColor555 Magenta{0xFF, 0x00, 0xFF};
        static constexpr JColor555 Yellow{0xFF, 0xFF, 0x00};
        static constexpr JColor555 Cyan{0x00, 0xFF, 0xFF};
    };
}