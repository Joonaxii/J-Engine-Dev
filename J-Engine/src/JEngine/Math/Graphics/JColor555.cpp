#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>

namespace JEngine {
    static constexpr uint16_t RED_MASK_555 = 0x1F;
    static constexpr uint16_t GREEN_MASK_555 = 0x3E0;
    static constexpr uint16_t BLUE_MASK_555 = 0x7C00;
    static constexpr uint16_t ALPHA_MASK_555 = 0x8000;

    JColor555::JColor555(const JColor565& rgb) : JColor555(rgb.getR_UI8(), rgb.getG_UI8(), rgb.getB_UI8()) {}
    JColor555::JColor555(const JColor24& rgb) : JColor555(rgb.r, rgb.g, rgb.b, 0xFF) {}
    JColor555::JColor555(const JColor32& rgba) : JColor555(rgba.r, rgba.g, rgba.b, rgba.a) {}

    void JColor555::flipRB() {
        uint16_t r = (data & RED_MASK_555);
        uint16_t b = (data & BLUE_MASK_555) >> 10;
        data = (data & (GREEN_MASK_555 | ALPHA_MASK_555)) | (r << 10) | b;
    }
}