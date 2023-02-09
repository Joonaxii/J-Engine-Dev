#include <JEngine/Rendering/Buffers/VertexBuffer.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>

namespace JEngine {
    VertexBuffer::VertexBuffer() : _rendererID(0) { }    
    
    VertexBuffer::VertexBuffer(const void* data, const size_t size, const uint32_t drawMode) : _rendererID(0) {
        init(data, size, drawMode);
    }

    VertexBuffer::~VertexBuffer() { release(); }

    void VertexBuffer::init(const void* data, const size_t size, const uint32_t drawMode) {
        if (!_rendererID) {
            GLCall(glGenBuffers(1, &_rendererID));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
            GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, drawMode));
        }
    }

    void VertexBuffer::release() {
        if (_rendererID) {
            GLCall(glDeleteBuffers(1, &_rendererID));
            _rendererID = 0;
        }
    }

    void VertexBuffer::updateBuffer(const void* data, const size_t size) {
        if (bind()) {
            GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
        }
    }

    bool VertexBuffer::bind() const {
        if (_rendererID) {
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
        }
        return bool(_rendererID);
    }

    void VertexBuffer::unbind() const {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
}