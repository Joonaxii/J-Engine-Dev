#include <JEngine/Math/Graphics/JColor4444.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor48.h>
#include <JEngine/Math/Graphics/JColor64.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/IO/ImageUtils.h>

namespace JEngine {

    JColor4444::JColor4444(const JColor& rgba)  : 
        JColor4444(
            uint16_t(std::clamp<float>(rgba.r * 15.0f, 0, 15.0f)), 
            uint16_t(std::clamp<float>(rgba.g * 15.0f, 0, 15.0f)),
            uint16_t(std::clamp<float>(rgba.b * 15.0f, 0, 15.0f)),
            uint16_t(std::clamp<float>(rgba.a * 15.0f, 0, 15.0f))      
        ) { }

    JColor4444::JColor4444(const JColor24& rgb) : 
        JColor4444(remapUI8ToUI4(rgb.r), remapUI8ToUI4(rgb.g), remapUI8ToUI4(rgb.b), 0xF) {}
    JColor4444::JColor4444(const JColor32& rgba) :
        JColor4444(remapUI8ToUI4(rgba.r), remapUI8ToUI4(rgba.g), remapUI8ToUI4(rgba.b), remapUI8ToUI4(rgba.a)) {}
    JColor4444::JColor4444(const JColor48& rgb) {

    }
    JColor4444::JColor4444(const JColor64& rgba) {

    }
}