#pragma once
#include <cstdint>

namespace JEngine {
    class IndexBuffer {
    public:
        IndexBuffer();
        IndexBuffer(const uint32_t* data, const uint32_t count, const uint32_t drawMode);
        ~IndexBuffer();

        void init(const uint32_t* data, const uint32_t count, const uint32_t drawMode);
        void release();

        void bind() const;
        void unbind() const;

        void updateBuffer(const uint32_t* data, const uint32_t count);

        inline const uint32_t getCount() const {
            return _count;
        }

    private:
        uint32_t _rendererID;
        uint32_t _count;
    };
}