#pragma once
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>
#include <JEngine/Rendering/RenderTarget.h>

namespace JEngine {
    class RenderTexture : public Texture, public RenderTarget {
    public:
        static constexpr char* JRTX_HEADER = "JRTX";

        RenderTexture();
        ~RenderTexture();

        bool create(const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter, uint8_t* pixelData, const bool keepData) override;
        void update(const uint8_t* pixels, const TextureFormat format, const uint32_t width, const uint32_t height, TextureFormat toFormat, const FilterMode filter, const bool keepData) override;

        uint8_t getDepth() const;

        bool serializeBinary(std::ostream& stream) const override;
        bool deserializeBinary(std::istream& stream, const size_t size) override;

        bool serializeJson(json& jsonF) const override;
        bool deserializeJson(json& jsonF) override;

    protected:
        void applyData(const uint8_t* pixels, const bool doFlush) override;
        void applyPixels(const uint8_t* pixels, const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter) override;
        void applyPalette(const uint8_t* palette) override;

        virtual bool jsonToBinaryImpl(json& jsonF, std::ostream& stream) const override { return false; }

    private:
        static constexpr uint16_t FLAG_KEEP_DEPTH_DATA = 0x0008U;

        uint8_t _texDepth;
        uint32_t _depthId;

        void applyDepthBuffer(const uint16_t width, const uint16_t height, const uint8_t depth);
        void freeDepthBuffer();
    };
}

REGISTER_ASSET(JEngine::RenderTexture);