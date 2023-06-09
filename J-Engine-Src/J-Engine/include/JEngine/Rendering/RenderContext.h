#pragma once

namespace JEngine {
    class RenderContext {
    public:

        virtual ~RenderContext() = default;
        virtual void init() = 0;
        virtual void swapBuffers() = 0;

        static RenderContext* create(void* window);
    };
}