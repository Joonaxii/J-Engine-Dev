#include <JEngine/Rendering/Buffers/VertexArray.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <JEngine/Rendering/Buffers/VertexBuffer.h>
#include <JEngine/Rendering/Buffers/BufferLayout.h>

namespace JEngine {
    VertexArray::VertexArray() : _rendererID(0) { }
    VertexArray::~VertexArray() {
        release();
    }

    void VertexArray::init() {
        if (!_rendererID) {
            GLCall(glGenVertexArrays(1, &_rendererID));
        }
    }

    void VertexArray::release() {
        if (_rendererID) {
            GLCall(glDeleteVertexArrays(1, &_rendererID));
            _rendererID = 0;
        }
    }

    void VertexArray::addBuffer(const VertexBuffer& vb, const BufferLayout& layout) {
        if (vb.bind() && _rendererID) {
            bind();

            const auto& elements = layout.getElements();
            uint64_t offset = 0;
            for (uint32_t i = 0; i < elements.size(); i++) {
                const auto& element = elements[i];
                GLCall(glEnableVertexAttribArray(i));
                GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)(offset)));

                offset += uint64_t(element.count) * BufferElement::getSizeOfType(element.type);
            }
        }
    }

    void VertexArray::bind() const {
        GLCall(glBindVertexArray(_rendererID));
    }
    void VertexArray::unbind() const {
        GLCall(glBindVertexArray(0));
    }
}