#pragma once
#include <cstdint>
#include <JEngine/Math/Graphics/JVertex.h>

namespace JEngine {
    class IRenderable {
    public:
        virtual int32_t getIndexCount() const = 0;
        virtual const uint32_t* getIndices() const = 0;

        virtual int32_t getVertexCount() const = 0;
        virtual const JVertex* getVertices(const uint8_t flip = 0) const = 0;
    };
}