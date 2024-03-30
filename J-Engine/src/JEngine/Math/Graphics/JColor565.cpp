#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>

namespace JEngine {
    JColor565::JColor565(const JColor555& rgb) : JColor565(rgb.getR_UI8(), rgb.getG_UI8(), rgb.getB_UI8()) {}

    JColor565::JColor565(const JColor24& rgb) : JColor565(rgb.r, rgb.g, rgb.b) {}
    JColor565::JColor565(const JColor32& rgba) : JColor565(rgba.r, rgba.g, rgba.b) {}

    void JColor565::flipRB() {
        uint16_t r = (data & RED_MASK_565);
        uint16_t b = (data & BLUE_MASK_565) >> 11;
        data = (data & GREEN_MASK_565) | (r << 11) | b;
    }
}