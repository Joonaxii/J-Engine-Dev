#pragma once
#include <cstdint>
#include <JEngine/Math/Math.h>

namespace JEngine {
    struct JColor24;
    struct JColor32;
    struct JColor565;
    struct JColor555;
    struct JColor;
    struct JColor4444 {
    public:
        constexpr JColor4444() : _value(0) {}
        constexpr JColor4444(uint8_t gray) : JColor4444(gray, gray, gray, 0xFF) {}
        constexpr JColor4444(uint8_t gray, uint8_t alpha) : JColor4444(gray, gray, gray, alpha) {}
        constexpr JColor4444(uint8_t r, uint8_t g, uint8_t b) : JColor4444(r, g, b, 0xFF) {}
        constexpr JColor4444(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : 
            _value(Math::remapBitsFromUI8<uint16_t,4>(r) | 
                  (Math::remapBitsFromUI8<uint16_t, 4>(g) << 4) | 
                  (Math::remapBitsFromUI8<uint16_t, 4>(b) << 8) | 
                  (Math::remapBitsFromUI8<uint16_t, 4>(a) << 12)) {}

        JColor4444(const JColor& rgba);

        JColor4444(const JColor24& rgb);
        JColor4444(const JColor32& rgba);      

        JColor4444(const JColor555& rgb);
        JColor4444(const JColor565& rgba);

        constexpr operator uint16_t() const { return _value; }

        constexpr uint8_t getR() const { return (_value & 0xF); }
        constexpr uint8_t getG() const { return ((_value >> 4) & 0xF); }
        constexpr uint8_t getB() const { return ((_value >> 8) & 0xF); }
        constexpr uint8_t getA() const { return ((_value >> 12) & 0xF); }

        constexpr uint8_t getR_UI8() const { return Math::remapBitsToUI8<4>(_value & 0xF); }
        constexpr uint8_t getG_UI8() const { return Math::remapBitsToUI8<4>((_value >> 4) & 0xF); }
        constexpr uint8_t getB_UI8() const { return Math::remapBitsToUI8<4>((_value >> 8) & 0xF); }
        constexpr uint8_t getA_UI8() const { return Math::remapBitsToUI8<4>((_value >> 12) & 0xF); }

        void flipRB();
    private:
        uint16_t _value;
    };

    struct Colors4444 {
        static constexpr JColor4444 Claer{ 0x00, 0x00, 0x00, 0x00 };
        static constexpr JColor4444 White{ 0xFF, 0xFF, 0xFF };
        static constexpr JColor4444 Black{ 0x00, 0x00, 0x00 };
        static constexpr JColor4444 Gray{ 0x7F, 0x7F, 0x7F };
        static constexpr JColor4444 Red{ 0xFF, 0x00, 0x00 };
        static constexpr JColor4444 Green{ 0x00, 0xFF, 0x00 };
        static constexpr JColor4444 Blue{ 0x00, 0x00, 0xFF };
        static constexpr JColor4444 Magenta{ 0xFF, 0x00, 0xFF };
        static constexpr JColor4444 Yellow{ 0xFF, 0xFF, 0x00 };
        static constexpr JColor4444 Cyan{ 0x00, 0xFF, 0xFF };
    };
}