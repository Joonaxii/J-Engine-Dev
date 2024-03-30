#include <JEngine/Math/Graphics/JColor4444.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/IO/ImageUtils.h>

namespace JEngine {

    JColor4444::JColor4444(const JColor& rgba)  : 
        JColor4444(
            uint16_t(Math::clamp<float>(rgba.r * 15.0f, 0, 15.0f)), 
            uint16_t(Math::clamp<float>(rgba.g * 15.0f, 0, 15.0f)),
            uint16_t(Math::clamp<float>(rgba.b * 15.0f, 0, 15.0f)),
            uint16_t(Math::clamp<float>(rgba.a * 15.0f, 0, 15.0f))      
        ) { }

    JColor4444::JColor4444(const JColor24& rgb) : 
        JColor4444(rgb.r, rgb.g, rgb.b, 0xFF) {}
    JColor4444::JColor4444(const JColor32& rgba) :
        JColor4444(rgba.r, rgba.g, rgba.b, rgba.a) {}

    JColor4444::JColor4444(const JColor565& rgb) : 
        JColor4444(rgb.getR_UI8(), rgb.getG_UI8(), rgb.getB_UI8(), 0xFF) {}
    JColor4444::JColor4444(const JColor555& rgba) :
        JColor4444(rgba.getR_UI8(), rgba.getG_UI8(), rgba.getB_UI8(), rgba.getA_UI8()) {}

    void JColor4444::flipRB() {
        uint16_t red = (_value & 0xF) << 8;
        uint16_t blue = (_value & 0xF00) >> 8;
        _value = (_value & 0x0F0F) | red | blue;
    }
}