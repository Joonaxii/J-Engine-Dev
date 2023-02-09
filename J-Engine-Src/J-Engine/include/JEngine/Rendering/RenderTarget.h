#pragma once
#include <cstdint>

namespace JEngine {
    class RenderTarget {
    public:
        RenderTarget();
        ~RenderTarget();

        void init();
        void release();

        virtual void invalidate() = 0;

        bool bindAsRenderTarget() const;
        void unbindAsRenderTarget() const;

    private:
        uint32_t _frameBufferID;
    };
}