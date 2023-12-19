#pragma once
#include <cstdint>
#include <algorithm>

namespace JEngine {
    struct JColor24;
    struct JColor32;
    struct JColor565;
    struct JColor555;
    struct JColor48;
    struct JColor4444;
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

        JColor64(const JColor24& rgb);
        JColor64(const JColor32& rgba);

        JColor64(const JColor& rgba);

        JColor64(const JColor555& rgb);
        JColor64(const JColor565& rgba);

        JColor64(const JColor4444& rgba);

        operator uint64_t() const { return *reinterpret_cast<const uint64_t*>(this); }

        void set(uint16_t gray) {
            r = g = b = gray;
            a = 0xFFFF;
        }

        void set(uint16_t gray, uint16_t alpha) {
            r = g = b = gray;
            a = alpha;
        }  
        
        void set(uint16_t r, uint16_t g, uint16_t b) {
            this->r = r;
            this->g = g;
            this->b = b;
            a = 0xFFFF;
        }  
        
        void set(uint16_t r, uint16_t g, uint16_t b, uint16_t a) {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        void flipRB() {
            std::swap(r, b);
        }
    };
}