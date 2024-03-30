#pragma once
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>
#include <JEngine/Rendering/RenderTarget.h>

namespace JEngine {
    class RenderTexture : public Texture, public RenderTarget {
    public:
        RenderTexture();
        ~RenderTexture();

        bool create(const uint8_t* pixelData, TextureFormat format, int32_t paletteSize, int32_t width, int32_t height,  FilterMode filter, uint8_t flags = 0x00) override;
        bool update(const uint8_t* input, TextureFormat format, int32_t paletteSize, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t flags = TEX_UF_NONE) override;

        uint8_t getDepth() const;

        bool serializeBinary(const Stream& stream) const;
        bool deserializeBinary(const Stream& stream, const size_t size);

        bool serializeJson(json& jsonF) const;
        bool deserializeJson(json& jsonF);

    protected:
        void applyData(const uint8_t* pixels, const bool doFlush);
        void applyPixels(const uint8_t* pixels, const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter);
        void applyPalette(const uint8_t* palette);

    private:
        static constexpr uint16_t FLAG_KEEP_DEPTH_DATA = 0x0008U;

        uint8_t _texDepth;
        uint32_t _depthId;

        void applyDepthBuffer(const uint16_t width, const uint16_t height, const uint8_t depth);
        void freeDepthBuffer();
    };
}