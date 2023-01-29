#include <JEngine/Rendering/RenderTarget.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>


namespace JEngine {
    RenderTarget::RenderTarget() : _frameBufferID(0) {}
    RenderTarget::~RenderTarget() {
        release();
    }

    void RenderTarget::init() {
        if (_frameBufferID) { return; }
        GLCall(glGenFramebuffers(1, &_frameBufferID));

    }
    void RenderTarget::release() {
        if (!_frameBufferID) { return; }
        GLCall(glDeleteFramebuffers(1, &_frameBufferID));
        _frameBufferID = 0;
    }

    const bool RenderTarget::bindAsRenderTarget() const  {
        if (_frameBufferID) {
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferID));
            return true;
        }
        return false;
    }

    void RenderTarget::unbindAsRenderTarget() const {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}