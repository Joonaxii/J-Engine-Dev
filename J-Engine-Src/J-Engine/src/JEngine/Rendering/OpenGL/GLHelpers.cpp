#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <iostream>
#include <JEngine/Core/Log.h>

namespace JEngine {

    bool GlLogWrite(const char* type, const uint32_t error, const char* function, const char* file, int line) {
        if (error) {
            JENGINE_CORE_ERROR("{0} ({1}): {2} {3}:{4}", type, error, function, file, line);
            return false;
        }
        return true;
    }

    uint32_t GLClearError(uint32_t maxErrors) {
        uint32_t err = glGetError();
        while (err != GL_NO_ERROR && maxErrors--) { err = glGetError(); }
        return err;
    }

    bool GLLogCall(const char* function, const char* file, int line) {
        while (GLenum error = glGetError()) {
            GlLogWrite("[OpenGL Error]", error, function, file, line);
            return false;
        }
        return true;
    }
}