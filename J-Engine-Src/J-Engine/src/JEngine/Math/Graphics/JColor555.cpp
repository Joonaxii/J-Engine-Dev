#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>

namespace JEngine {
    const JColor555 JColor555::Clear(0x00, 0x00, 0x00, 0x00);
    const JColor555 JColor555::White(0xFF, 0xFF, 0xFF);
    const JColor555 JColor555::Black(0, 0, 0);
    const JColor555 JColor555::Gray(0x80, 0x80, 0x80);
    const JColor555 JColor555::Red(0xFF, 0, 0);
    const JColor555 JColor555::Green(0, 0xFF, 0);
    const JColor555 JColor555::Blue(0, 0, 0xFF);
    const JColor555 JColor555::Magenta(0xFF, 0, 0xFF);
    const JColor555 JColor555::Yellow(0xFF, 0xFF, 0);
    const JColor555 JColor555::Cyan(0, 0xFF, 0xFF);


    JColor555::JColor555(uint8_t r, uint8_t g, uint8_t b) : JColor555(r, g, b, 0xFF) { }
    JColor555::JColor555(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) : data() {
        data |= (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10) | (alpha >= 127 ? ALPHA_MASK_555 : 0);
    }

    JColor555::JColor555(const JColor565& rgb) : data() {
        uint16_t g = (((rgb.data & GREEN_MASK_565) >> 5) * 255) / 63;
        data |= (rgb.data & RED_MASK_565) | (((g >> 3) << 5)) | ((rgb.data & BLUE_MASK_565) >> 1) | ALPHA_MASK_555;
    }

    JColor555::JColor555(const JColor24& rgb) : JColor555(rgb.r, rgb.g, rgb.b, 0xFF) {}
    JColor555::JColor555(const JColor32& rgba) : JColor555(rgba.r, rgba.g, rgba.b, rgba.a) {}

    void JColor555::flipRB() {
        uint16_t r = (data & RED_MASK_555);
        uint16_t b = (data & BLUE_MASK_555) >> 10;
        data = (data & (GREEN_MASK_555 | ALPHA_MASK_555)) | (r << 10) | b;
    }
}