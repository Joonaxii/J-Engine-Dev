#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <JEngine/Assets/Graphics/Texture/FilterMode.h>
#include <JEngine/Assets/Graphics/Texture/TextureFormat.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) ASSERT(JEngine::GlLogWrite("[OpenGL Hang Error]", JEngine::GLClearError(), #x, __FILE__, __LINE__))\
    x;\
    ASSERT(JEngine::GLLogCall(#x, __FILE__, __LINE__))

namespace JEngine {
    const bool GlLogWrite(const char* type, const uint32_t error, const char* function, const char* file, int line);
    const uint32_t GLClearError(uint32_t maxErrors = 1000);
    const bool GLLogCall(const char* function, const char* file, int line);

    const uint32_t filterModeToGLFilter(const FilterMode filter);

    constexpr uint32_t textureFormatToGLFormat(const TextureFormat& fmt, const bool isMain = true) {
        switch (fmt)
        {
            default:                    return isMain ? GL_RGBA8 : GL_RGBA;
            case TextureFormat::RGB24:  return isMain ? GL_RGB8 : GL_RGB;
            case TextureFormat::Indexed:
            case TextureFormat::R8:     return isMain ? GL_R8 : GL_RED;
        }
    }

    constexpr uint32_t getGLPixelAlignment(const TextureFormat& fmt) {
        switch (fmt)
        {
            default:                    return 4;

            case TextureFormat::Indexed:
            case TextureFormat::R8:
            case TextureFormat::RGB24:  return 1;

        }
    }
}