#pragma once 
#include <JEngine/Assets/IAsset.h>
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/Assets/Graphics/Texture/TextureFormat.h>
#include <JEngine/Assets/Graphics/Texture/FilterMode.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Utility/ConstSpan.h>
#include <JEngine/Utility/Flags.h>

namespace JEngine {
    class Texture : public IAsset, public ISerializable<Texture> {
    public:

        Texture();
        ~Texture();

        virtual const bool create(const uint16_t width, const uint16_t height);
        virtual const bool create(const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter);
        virtual const bool create(const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter, uint8_t* pixelData, const bool keepData);

        void setPalette(const uint8_t* pixels, const size_t size, const TextureFormat format);

        const JVector2i getSize() const;

        virtual const bool serializeBinary(std::ostream& stream) const override = 0;
        virtual const bool deserializeBinary(std::istream& stream, const size_t size) override = 0;

        virtual const bool serializeJson(json& jsonF) const override = 0;
        virtual const bool deserializeJson(json& jsonF) override = 0;

        static const uint32_t getMaximumSize();
        uint8_t* getTextureData() const;

        const uint32_t getMainNativeHandle() const;
        const uint32_t getPaletteNativeHandle() const;

        void setTextureFormat(const TextureFormat& format);
        const TextureFormat getTextureFormat() const;

        void setFilterMode(const FilterMode& filter);
        const FilterMode getFilterMode() const;

        void setCompressionLevel(const int32_t level);
        const int32_t getCompressionLevel() const;

        static const uint32_t bindNull(const uint32_t slot = 0);

        virtual const uint32_t bind(const uint32_t slot = 0) const;
        virtual const uint32_t unbind(const uint32_t slot = 0) const;

        virtual void update(const uint8_t* pixels, const TextureFormat format, const uint32_t width, const uint32_t height, TextureFormat toFormat, const FilterMode filter, const bool keepData);
        virtual void update(Texture& texture);
        virtual void update(Texture& texture, const TextureFormat toFormat);
        virtual void update(Texture& texture, const TextureFormat toFormat, const FilterMode filter);

    protected:
        static constexpr uint16_t FLAG_IS_REPEATED = 0x0001U;
        static constexpr uint16_t FLAG_FLIPPED = 0x0002U;
        static constexpr uint16_t FLAG_KEEP_DATA = 0x0004U;

        static constexpr uint16_t VALUE_COMPRESSION = 0xF000U;

        UI16Flags _texFlags;
        JVector2<uint16_t> _size;

        TextureFormat _format;
        FilterMode _filter;
        uint32_t _texId;
        uint32_t _palId;

        uint8_t* _pixelData;

        static void readPixel(const ConstSpan<uint8_t>& data, const TextureFormat format, const int32_t index, JColor32& color);
        static const int32_t generatePaletteAndTexture(const uint8_t* inputData, const uint16_t width, const uint16_t height, const TextureFormat format, uint8_t* outputData, const bool validate = true);

        virtual void applyData(const uint8_t* pixels, const bool doFlush);
        virtual void applyPixels(const uint8_t* pixels, const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter);
        virtual void applyPalette(const uint8_t* palette);

        virtual void freeTexture();
        virtual void freePalette();
    };
}