#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <iostream>

namespace JEngine {

    const bool GlLogWrite(const char* type, const uint32_t error, const char* function, const char* file, int line) {
        if (error) {
            std::cout << type << " (" << error << "): " << function << " " << file << ":" << line << std::endl;
            return false;
        }
        return true;
    }

    const uint32_t GLClearError(uint32_t maxErrors) {
        uint32_t err = glGetError();
        while (err != GL_NO_ERROR && maxErrors--) { err = glGetError(); }
        return err;
    }

    const bool GLLogCall(const char* function, const char* file, int line) {
        while (GLenum error = glGetError()) {
            GlLogWrite("[OpenGL Error]", error, function, file, line);
            return false;
        }
        return true;
    }

    const uint32_t filterModeToGLFilter(const FilterMode filter) {
        switch (filter)
        {
            default: return GL_NEAREST;
            case FilterMode::Linear: return GL_LINEAR;
        }
    }

}