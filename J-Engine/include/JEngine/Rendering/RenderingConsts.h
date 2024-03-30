#pragma once
#include <cstdint>

namespace JEngine::RenderingConsts {
    static constexpr int32_t DEF_WINDOW_WIDTH = 1280;
    static constexpr int32_t DEF_WINDOW_HEIGHT = 720;
    
    static constexpr float getAspect() { return DEF_WINDOW_WIDTH / float(DEF_WINDOW_HEIGHT); }
}