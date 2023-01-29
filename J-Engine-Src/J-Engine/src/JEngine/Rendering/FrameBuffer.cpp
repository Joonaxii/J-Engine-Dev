#include <JEngine/Rendering/FrameBuffer.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <cassert>

namespace JEngine {
    FrameBuffer::FrameBuffer() : _specs(), _bufferId(0), _colorAttachId(0), _depthAttachId(0) { }
    FrameBuffer::FrameBuffer(const FrameBufferSpecs& specs) : _specs(specs), _bufferId(0), _colorAttachId(0), _depthAttachId(0) { }
    FrameBuffer::~FrameBuffer() { release(); releaseTexture(); releaseDepthBuffer(); }

    void FrameBuffer::invalidate(const FrameBufferSpecs& specs) {
        const bool rebuild = specs != _specs || !_bufferId;
        _specs = specs;
        invalidate(rebuild);
    }

    void FrameBuffer::invalidate(const bool rebuild) {
        if (!rebuild) { return; }
        
        const bool createdBuf = !_bufferId;
        const bool createdTex = !_colorAttachId;
        const bool createdDep = !_depthAttachId;

        if (createdBuf) {
            GLCall(glCreateFramebuffers(1, &_bufferId));
        }
  
        if (createdTex || createdBuf || createdDep) {
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _bufferId));
        }

        if (createdTex) {
            GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &_colorAttachId));
        }

        GLCall(glBindTexture(GL_TEXTURE_2D, _colorAttachId));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _specs.width, _specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        if (createdTex || createdBuf) {
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttachId, 0));
        }

        if (createdDep) {
            GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &_depthAttachId));
        }
        GLCall(glBindTexture(GL_TEXTURE_2D, _depthAttachId));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _specs.width, _specs.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));

        if (createdDep || createdBuf) {
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAttachId, 0));
        }

        if (createdDep || createdTex || createdDep) {
            GLCall(GLenum ret = glCheckFramebufferStatus(GL_FRAMEBUFFER));
            assert(ret == GL_FRAMEBUFFER_COMPLETE && "Framebuffer not complete!");
        }
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void FrameBuffer::release() {
        if (_bufferId) {
            GLCall(glDeleteFramebuffers(1, &_bufferId));
            _bufferId = 0;
        }
    }

    void FrameBuffer::releaseTexture() {
        if (_colorAttachId) {
            GLCall(glDeleteFramebuffers(1, &_colorAttachId));
            _colorAttachId = 0;
        }
    }

    void FrameBuffer::releaseDepthBuffer() {
        if (_depthAttachId) {
            GLCall(glDeleteFramebuffers(1, &_depthAttachId));
            _depthAttachId = 0;
        }
    }

    const uint32_t FrameBuffer::bindColorAttachment(const uint32_t slot) const {
        uint32_t bindP = slot;

        GLCall(glActiveTexture(GL_TEXTURE0 + bindP++));
        GLCall(glBindTexture(GL_TEXTURE_2D, _colorAttachId));
        return slot;
    }

    const uint32_t FrameBuffer::unbindColorAttachment(const uint32_t slot) const {
        uint32_t bindP = slot;

        GLCall(glActiveTexture(GL_TEXTURE0 + bindP++));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

        return bindP;
    }

    const bool FrameBuffer::bind() const {
        if (_bufferId) {
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, _bufferId));
            return true;
        }
        return false;
    }

    void FrameBuffer::unbind() const {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}