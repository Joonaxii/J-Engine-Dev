#pragma once
#include <cstdint>

namespace JEngine {
    struct JColor24;
    struct JColor32;
    struct JColor565;
    struct JColor555;
    struct JColor48;
    struct JColor64;
    struct JColor;
    struct JColor4444 {
    public:
        constexpr JColor4444() : _value(0) {}
        constexpr JColor4444(uint16_t gray) : JColor4444(gray, gray, gray, 0xF) {}
        constexpr JColor4444(uint16_t gray, uint16_t alpha) : JColor4444(gray, gray, gray, alpha) {}
        constexpr JColor4444(uint16_t r, uint16_t g, uint16_t b) : JColor4444(r, g, b, 0xF) {}
        constexpr JColor4444(uint16_t r, uint16_t g, uint16_t b, uint16_t a) : _value(r | (g << 4) | (b << 8) | (a << 12)) {}

        JColor4444(const JColor& rgba);

        JColor4444(const JColor24& rgb);
        JColor4444(const JColor32& rgba);      
        
        JColor4444(const JColor48& rgb);
        JColor4444(const JColor64& rgba);

        JColor4444(const JColor555& rgb);
        JColor4444(const JColor565& rgba);

        operator uint16_t() const { return _value; }

        uint8_t getR() const { return (_value & 0xF); }
        uint8_t getG() const { return ((_value >> 4) & 0xF); }
        uint8_t getB() const { return ((_value >> 8) & 0xF); }
        uint8_t getA() const { return ((_value >> 12) & 0xF); }

        void setR(uint8_t value) { _value = (_value & ~0xF) | value; }
        void setG(uint8_t value) { _value = (_value & ~0xF0) | (value << 4); }
        void setB(uint8_t value) { _value = (_value & ~0xF00) | (value << 8); }
        void setA(uint8_t value) { _value = (_value & ~0xF000) | (value << 12); }

        void set(uint8_t gray) {
            _value = (gray | (gray << 4) | (gray << 8) | 0xF000);
        }

        void set(uint8_t gray, uint8_t alpha) {
            _value = (gray | (gray << 4) | (gray << 8) | (alpha << 12));
        }  
        
        void set(uint8_t r, uint8_t g, uint8_t b) {
            _value = (r | (g << 4) | (b << 8) | 0xF000);
        }
           
        void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
            _value = (r | (g << 4) | (b << 8) | (a << 12));
        }

        void flipRB() {
            uint16_t red = (_value & 0xF) << 8;
            uint16_t blue = (_value & 0xF00) >> 8;
            _value = (_value & 0x0F0F) | red | blue;
        }
    private:
        uint16_t _value;
    };
}