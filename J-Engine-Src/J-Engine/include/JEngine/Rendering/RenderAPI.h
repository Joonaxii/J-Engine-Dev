#pragma once
#include <cstdint>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Rendering/Buffers/VertexArray.h>

namespace JEngine {
    class RenderAPI {
    public:
        enum API {
            None = 0, OpenGL = 1, 
        };

        virtual ~RenderAPI() = default;

        virtual void init() = 0;
        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void setClearColor(const JColor32& color) = 0;

        virtual void clear() = 0;

        virtual void drawIndexed(const VertexArray& vertexArray, uint32_t indexCount = 0) = 0;
        virtual void drawLines(const VertexArray& vertexArray, uint32_t vertexCount = 0) = 0;

        virtual void setLineWidth(float width) = 0;

        static API getAPI() { return _api; }
        static RenderAPI* create();

    private:

        static API _api;
    };
}