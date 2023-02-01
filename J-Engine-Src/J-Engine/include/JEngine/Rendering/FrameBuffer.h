#pragma once
#include <cstdint>
#include <JEngine/Collections/IRef.h>

namespace JEngine {

    struct FrameBufferSpecs {
        uint16_t width;
        uint16_t height;

        uint32_t samples = 1;

        const bool operator==(const FrameBufferSpecs& other) const {
            return width == other.width && height == other.height && samples == other.samples;
        }

        const bool operator!=(const FrameBufferSpecs& other) const {
            return !(*this == other);
        }
    };

    class FrameBuffer : public IRef {
    public:

        FrameBuffer();
        FrameBuffer(const uint32_t colorFormat);
        FrameBuffer(const FrameBufferSpecs& specs);
        FrameBuffer(const uint32_t colorFormat, const FrameBufferSpecs& specs);
        ~FrameBuffer();

        const uint32_t& getBufferId() const { return _bufferId; }
        const uint32_t getColorAttatchmentId() const { return _colorAttachId; }

        void invalidate(const FrameBufferSpecs& specs);
        void release();

        void releaseTexture();
        void releaseDepthBuffer();

        const uint32_t bindColorAttachment(const uint32_t slot) const;
        const uint32_t unbindColorAttachment(const uint32_t slot) const;

        const bool bind() const;
        void unbind() const;

    private:

        void invalidate(const bool rebuild);
        FrameBufferSpecs _specs;
        uint32_t _bufferId;

        uint32_t _colorFormat;
        uint32_t _colorAttachId;
        uint32_t _depthAttachId;
    };
}