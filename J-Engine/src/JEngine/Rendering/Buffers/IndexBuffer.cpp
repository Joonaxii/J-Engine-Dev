#include <JEngine/Rendering/Buffers/IndexBuffer.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>

namespace JEngine {
    IndexBuffer::IndexBuffer() : _rendererID(0), _count(0) {}
    IndexBuffer::IndexBuffer(const uint32_t* data, const uint32_t count, const uint32_t drawMode) : _count(count) {
        init(data, count, drawMode);
    }

    IndexBuffer::~IndexBuffer() { release(); }

    void IndexBuffer::init(const uint32_t* data, const uint32_t count, const uint32_t drawMode) {
        _count = count;
        if (!_rendererID) {
            GLCall(glGenBuffers(1, &_rendererID));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererID));
            GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), data, drawMode));
        }
    }

    void IndexBuffer::release()  {
        if (_rendererID) {
            GLCall(glDeleteBuffers(1, &_rendererID));
            _rendererID = 0;
        }
    }

    void IndexBuffer::bind() const {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));
    }

    void IndexBuffer::unbind() const {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::updateBuffer(const uint32_t* data, const uint32_t count) {
        _count = count;
        if (_rendererID) {
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID));
            GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data));
        }
    }

    uint32_t IndexBuffer::getCount() const { return _count; }
}