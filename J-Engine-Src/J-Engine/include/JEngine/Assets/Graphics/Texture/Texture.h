#pragma once 
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/IO/ImageUtils.h>

namespace JEngine {
    class Texture : public IAsset {
    public:
        Texture();
        Texture(Texture&& other) noexcept;
        ~Texture() noexcept;

        TextureFormat getFormat() const { return _format; }
        FilterMode getFilter() const { return _filter; }

        bool create(const uint8_t* input, TextureFormat format, int32_t paletteSize, int32_t width, int32_t height, FilterMode filter, uint8_t flags);
        bool isValid() const { return bool(_textureId) && _valid; }

        int32_t getWidth() const { return _width; }
        int32_t getHeight() const { return _height; }

        uint32_t getTextureId() const { return _textureId; }
        uint32_t getPaletteId() const { return _paletteId; }

        int32_t getPaletteSize() const { return _paletteSize; }

        uint8_t* getPixels() const;
        uint8_t* getPixels(uint8_t* buffer) const;

        uint32_t bind(uint32_t slot) const;
        uint32_t unBind(uint32_t slot) const;

        uint32_t getHash() const { return _crcTex; }
        uint8_t getFlags() const { return _flags; }

        virtual bool serializeBinary(const Stream& stream) const override;
        virtual bool deserializeBinary(const Stream& stream, const size_t size);

        static uint32_t bindNull(uint32_t slot);

        void invalidate() {
            _valid = false;
        }
        void release();
    protected:
        static constexpr uint8_t VALUE_COMPRESSION = 0xF;
        uint32_t _textureId;
        uint32_t _paletteId;

        uint32_t _crcTex;

        int32_t _paletteSize;
        int32_t _width;
        int32_t _height;
        TextureFormat _format;
        FilterMode _filter;
        UI8Flags _flags;
        bool _valid{ false };

        void releaseTexture();
        void releasePalette();
    };

    enum : uint8_t {
        TEX_GEN_IDLE,
        TEX_GEN_WAIT,
        TEX_GEN_PROCESSING,
    };

    struct TextureGenState {
        std::shared_ptr<Texture>* texture{};
        ImageData data{};
        uint8_t state{ TEX_GEN_IDLE };
    };


    bool waitForTexGen();

    void setupTexGen(std::shared_ptr<Texture>& texture, const ImageData& data);
    void updateTexGen();
}