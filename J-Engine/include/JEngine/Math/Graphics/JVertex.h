#pragma once
#include <JEngine/Core.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Units/JVector.h>

namespace JEngine {
JE_BEG_PACK
    struct JVertex {
        JVector3f position;
        JColor32 color;
        JVector2f uv;

        constexpr JVertex() : 
            position(), color(Colors32::White), uv(0.0f, 0.0f) {}
        constexpr JVertex(const JVector3f& position, JColor32 color, const JVector2f& uv) : 
            position(position), color(color), uv(uv) { }
    };
JE_END_PACK
}