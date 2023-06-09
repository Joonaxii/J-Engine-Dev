#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>

namespace JEngine {
    const JColor565 JColor565::White(0xFF, 0xFF, 0xFF);
    const JColor565 JColor565::Black(0, 0, 0);
    const JColor565 JColor565::Gray(0x80, 0x80, 0x80);
    const JColor565 JColor565::Red(0xFF, 0, 0);
    const JColor565 JColor565::Green(0, 0xFF, 0);
    const JColor565 JColor565::Blue(0, 0, 0xFF);
    const JColor565 JColor565::Magenta(0xFF, 0, 0xFF);
    const JColor565 JColor565::Yellow(0xFF, 0xFF, 0);
    const JColor565 JColor565::Cyan(0, 0xFF, 0xFF);

    JColor565::JColor565(uint8_t r, uint8_t g, uint8_t b) {
        data = ((b >> 3) << 11) | ((g >> 2) << 5) | (r >> 3);
    }
    JColor565::JColor565(const JColor555& rgb) : data() {
        uint16_t g = (((rgb.data & GREEN_MASK_555) >> 5) * 255) / 31;
        data |= (rgb.data & RED_MASK_555) | (g << 5) | ((rgb.data & BLUE_MASK_555) << 1);
    }

    JColor565::JColor565(const JColor24& rgb) : JColor565(rgb.r, rgb.g, rgb.b) {}
    JColor565::JColor565(const JColor32& rgba) : JColor565(rgba.r, rgba.g, rgba.b) {}

    void JColor565::flipRB() {
        uint16_t r = (data & RED_MASK_565);
        uint16_t b = (data & BLUE_MASK_565) >> 11;
        data = (data & GREEN_MASK_565) | (r << 11) | b;
    }
}