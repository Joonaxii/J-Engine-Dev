#include <JEngine/Assets/Graphics/Texture/Texture.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Core/Log.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <JEngine/IO/Compression/ZLib.h>
#include <JEngine/Utility/DataFormatUtils.h>
#include <JEngine/IO/MemoryStream.h>

namespace JEngine {
    Texture::Texture() : IAsset(), _width(0), _height(0), _textureId(0), _paletteId(0), _paletteSize(0), _crcTex(0), _format(TextureFormat::Unknown) { }
    Texture::Texture(Texture&& other) noexcept :
        IAsset(other), _width(other._width), _height(other._height),
        _paletteSize(other._paletteSize),
        _format(other._format),
        _crcTex(other._crcTex),
        _textureId(std::exchange(other._textureId, 0)),
        _paletteId(std::exchange(other._paletteId, 0))
    {
    }
    Texture::~Texture() noexcept {
        release();
    }

    bool Texture::create(const ImageData& img) {
        return create(img.data, img.format, img.paletteSize, img.width, img.height, img.filter, img.flags);
    }

    bool Texture::create(const ImageData& img, FilterMode filter) {
        return create(img.data, img.format, img.paletteSize, img.width, img.height, filter, img.flags);
    }

    bool Texture::create(const uint8_t* input, TextureFormat format, int32_t paletteSize, int32_t width, int32_t height, FilterMode filter, uint8_t flags) {
        _valid = false;
        const bool isIndexed = format == TextureFormat::Indexed8 || format == TextureFormat::Indexed16;
        if (isIndexed && !Math::isAlignedToPalette(paletteSize)) {
            JENGINE_CORE_WARN("Couldn't create texture, palette is not aligned to palette scan! ({0}, should be {1})", paletteSize, Math::alignToPalette(paletteSize));
            return false;
        }

        if (format == TextureFormat::Unknown || width == 0 || height == 0) {
            JENGINE_CORE_WARN("Couldn't create texture, given data was invalid! ({0}, {1}x{2})", getTextureFormatName(format), width, height);
            return false;
        }

        _flags = flags;
        _filter = filter;
        if (format == TextureFormat::Indexed8) { paletteSize = 256; }
        size_t extra = isIndexed ? paletteSize * 4 : 0;
        const uint8_t* pixels = input + extra;

        _crcTex = 0xFFFFFFFFU;
        _crcTex = Data::updateCRC(_crcTex, input, std::min<size_t>((width * height * (getBitsPerPixel(format) >> 3)) + extra, 1024)) ^ 0xFFFFFFFFU;

        _paletteSize = paletteSize;
        if (!_textureId) {
            glGenTextures(1, &_textureId);
        }

        _format = format;
        _width = uint16_t(width);
        _height = uint16_t(height);


        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterModeToGLFilter(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterModeToGLFilter(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT, getGLPixelAlignment(format));
        glTexImage2D(GL_TEXTURE_2D, 0, textureFormatToGLFormat(format, true), width, height, 0, textureFormatToGLFormat(format, false), GL_UNSIGNED_BYTE, input ? pixels : nullptr);

        if (isIndexed) {
            if (!_paletteId) {
                glGenTextures(1, &_paletteId);
            }

            uint32_t texType = format == TextureFormat::Indexed16 ? GL_TEXTURE_2D : GL_TEXTURE_1D;

            glBindTexture(texType, _paletteId);
            glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

            if (format == TextureFormat::Indexed16) {
                uint32_t palSize = paletteSize >> 8;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, palSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, input);
            }
            else {
                glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, paletteSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, input);
            }

        }
        else {
            releasePalette();
        }

        _valid = input != nullptr;
        return true;
    }

    bool Texture::update(const ImageData& img, int32_t x, int32_t y, uint8_t flags) {
        return update(img.data, img.format, img.paletteSize, x, y, img.width, img.height, flags);
    }

    bool Texture::update(const uint8_t* input, TextureFormat format, int32_t paletteSize, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t flags) {
        if (_textureId == 0) {
            JENGINE_CORE_WARN("Couldn't update texture, texture not created!");
            return false;
        }

        if (format == TextureFormat::Unknown || width == 0 || height == 0) {
            JENGINE_CORE_WARN("Couldn't update texture, given data was invalid! ({0}, {1}x{2})", getTextureFormatName(format), width, height);
            return false;
        }

        int32_t leftX = _width - x;
        int32_t leftY = _height - y;

        if (leftX < width || leftY < height) {
            JENGINE_CORE_WARN("Couldn't update texture, given texture block was invalid! ({0}, {1}, {2}x{3} | {4}x{5})", x, y, width, height, _width, _height);
            return false;
        }

        switch (_format) {
            default:
                JENGINE_CORE_WARN("Couldn't update texture, unsupported format '{0}'!", getTextureFormatName(format));
                return false;

            case TextureFormat::Indexed8:
            case TextureFormat::Indexed16:
                if (_format != format) {
                    JENGINE_CORE_WARN("Couldn't update texture, cannot convert from format '{0}' to '{1}'!", getTextureFormatName(format), getTextureFormatName(_format));
                    return false;
                }
                break;
            case TextureFormat::R8:
            case TextureFormat::RGB24:
            case TextureFormat::RGBA32:
                break;
        }

        //If the formats are identical, we can just copy the data directly
        if (_format == format) {



            return true;
        }
        return true;
    }

    uint32_t Texture::bind(uint32_t slot) const {
        glActiveTexture(GL_TEXTURE0 + slot++);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        if (_format == TextureFormat::Indexed8) {
            glActiveTexture(GL_TEXTURE0 + slot++);
            glBindTexture(GL_TEXTURE_1D, _paletteId);
        }
        return slot;
    }

    uint32_t Texture::unBind(uint32_t slot) const {
        glActiveTexture(GL_TEXTURE0 + slot++);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (_format == TextureFormat::Indexed8) {
            glActiveTexture(GL_TEXTURE0 + slot++);
            glBindTexture(GL_TEXTURE_1D, 0);;
        }
        return slot;
    }

    uint8_t* Texture::getPixels() const {
        size_t size = _width * _height * getBitsPerPixel(_format);
        if (_format == TextureFormat::Indexed8) {
            size += 1024;
        }
        else if (_format == TextureFormat::Indexed16) {
            size += _paletteSize * 4;
        }
        uint8_t* pixels = reinterpret_cast<uint8_t*>(malloc(size));
        return getPixels(pixels);
    }
    uint8_t* Texture::getPixels(uint8_t* buffer) const {
        if (!_textureId || !buffer) { return nullptr; }

        size_t bpp = getBitsPerPixel(_format);
        size_t offset = 0;
        size_t size = _width * _height * bpp;
        if (_format == TextureFormat::Indexed8) {
            size += offset = 1024;
        }
        else if (_format == TextureFormat::Indexed16) {
            size += offset = _paletteSize * 4;
        }

        glBindTexture(GL_TEXTURE_2D, _textureId);
        glPixelStorei(GL_PACK_ALIGNMENT, getGLPixelAlignment(_format));
        glGetTexImage(GL_TEXTURE_2D, 0, textureFormatToGLFormat(_format, false), GL_UNSIGNED_BYTE, buffer + offset);
        glBindTexture(GL_TEXTURE_2D, 0);
        glPixelStorei(GL_PACK_ALIGNMENT, 4);

        switch (_format)
        {
            case TextureFormat::Indexed8:
                glBindTexture(GL_TEXTURE_1D, _paletteId);
                glGetTexImage(GL_TEXTURE_1D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                glBindTexture(GL_TEXTURE_1D, 0);
                break;
            case TextureFormat::Indexed16:
                glBindTexture(GL_TEXTURE_2D, _paletteId);
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
                glBindTexture(GL_TEXTURE_2D, 0);
                break;
        }
        return buffer;
    }

    bool Texture::serializeBinary(const Stream& stream) const {
        stream.writeValue(DataHeader<FMT_JTEX>::Value);

        Serialization::serialize(_flags, stream);
        Serialization::serialize(_width, stream);
        Serialization::serialize(_height, stream);

        Serialization::serialize(_format, stream);
        Serialization::serialize(_filter, stream);
        Serialization::serialize(_paletteSize, stream);

        const int32_t bitsPP = getBitsPerPixel(_format);
        const int32_t bytesPP = bitsPP >> 3;

        const size_t offset = _format == TextureFormat::Indexed16 ? _paletteSize * 4 : _format == TextureFormat::Indexed8 ? 256 * 4 : 0;
        const size_t dataSize = (_width * _height * bytesPP) + offset;
        uint16_t compression = _flags.select(VALUE_COMPRESSION);

        auto pixels = getPixels();
        if (compression) {
            //Texture data is compressed

            int64_t pos = stream.tell();
            int32_t size = 0;
            stream.writeValue(size);

            uint8_t buffer[8192]{ 0 };

            ZLib::ZLibContext context{};
            ZLib::deflateBegin(context, compression == 10 ? -1 : compression, buffer, sizeof(buffer));
            ZLib::deflateSegment(context, pixels, dataSize, stream, buffer, sizeof(buffer));
            auto res = ZLib::deflateEnd(context, size, stream, buffer, sizeof(buffer));
            if (res != Z_OK) {
                std::cout << "ZLib compression failed! " << std::endl;
                std::cout << ZLib::zerr(res) << std::endl;
            }

            stream.flush();

            int64_t cPos = stream.tell();
            size = int32_t(cPos - (pos + 4));

            stream.seek(pos, SEEK_SET);
            stream.writeValue(size);
            stream.seek(cPos, SEEK_SET);
        }
        else {
            //Texture data is uncompressed
            stream.write(pixels, dataSize, false);
        }

        if (pixels) {
            free(pixels);
        }
        return false;
    }
    bool Texture::deserializeBinary(const Stream& stream, const size_t size) {
        uint32_t header = 0;
        stream.readValue(header, false);

        if (header == DataHeader<FMT_JTEX>::Value) {
            Serialization::deserialize(_flags, stream);
            Serialization::deserialize(_width, stream);
            Serialization::deserialize(_height, stream);

            Serialization::deserialize(_format, stream);
            Serialization::deserialize(_filter, stream);
            Serialization::deserialize(_paletteSize, stream);

            if (_format == TextureFormat::Indexed8) {
                _paletteSize = 256;
            }

            const uint16_t compression = _flags.select(VALUE_COMPRESSION);

            bool indexed = _format == TextureFormat::Indexed16 || _format == TextureFormat::Indexed8;
            const size_t offset = indexed ? _paletteSize * 4 : 0;
            const size_t dataSize = _width * _height * (getBitsPerPixel(_format) >> 3) + offset;
            uint8_t* pixels = reinterpret_cast<uint8_t*>(malloc(dataSize));

            if (compression) {
                //Data is compressed
                size_t compressedSize = 0;
                stream.read(&compressedSize, 4, false);

                uint8_t* ptrComp = nullptr;
                ptrComp = reinterpret_cast<uint8_t*>(malloc(compressedSize));
                stream.read(ptrComp, compressedSize, false);

                MemoryStream cStrm(ptrComp, compressedSize, compressedSize);
                MemoryStream oStrm(pixels, 0, dataSize);
                cStrm.seek(0, SEEK_SET);
                oStrm.seek(0, SEEK_SET);
                auto ret = ZLib::inflateData(cStrm, oStrm);
                if (ret != Z_OK) {
                    std::cout << ZLib::zerr(ret) << std::endl;
                }
                oStrm.flush();
                free(ptrComp);
            }
            else {
                //Data not compressed
                stream.read(pixels, dataSize, false);
            }

            create(pixels, _format, _paletteSize, _width, _height, _filter, _flags);
            free(pixels);
            return true;
        }
        return false;
    }

    uint32_t Texture::bindNull(uint32_t slot) {
        glActiveTexture(GL_TEXTURE0 + slot++);
        glBindTexture(GL_TEXTURE_2D, 0);
        return slot;
    }

    void Texture::release() {
        releaseTexture();
        releasePalette();
        invalidate();
    }

    void Texture::releaseTexture() {
        if (_textureId) {
            glDeleteTextures(1, &_textureId);
            _textureId = 0;
        }
    }

    void Texture::releasePalette() {
        if (_paletteId) {
            glDeleteTextures(1, &_paletteId);
            _paletteId = 0;
        }
    }

    TextureGenState* getTextureGens() {
        static TextureGenState TEX_GEN_STATE[MAX_TEXTURES_QUEUED]{};
        return TEX_GEN_STATE;
    }

    int32_t getFirstAvailableTexGen() {
        auto gens = getTextureGens();
        for (int32_t i = 0; i < MAX_TEXTURES_QUEUED; i++) {
            if (gens[i].state == TEX_GEN_FREE) { return i; }
        }
        return -1;
    }

    void waitForTexGen(int32_t index, size_t sleepFor) {
        while (shouldWaitForTexGen(index)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));
        }
    }

    bool shouldWaitForTexGen(int32_t index) {
        auto& gen = getTextureGens()[index];
        switch (gen.state)
        {
            default: return true;
            case TEX_GEN_IDLE:
                gen.state = TEX_GEN_FREE;
                return false;
            case TEX_GEN_FREE:
                return false;
        }
    }

    int32_t setupTexGen(std::shared_ptr<Texture>& texture, const ImageData& data) {
        int32_t nextValid = getFirstAvailableTexGen();

        while (nextValid < 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            nextValid = getFirstAvailableTexGen();
        }

        auto& genState = getTextureGens()[nextValid];
        genState.state = TEX_GEN_WAIT;
        genState.texture = &texture;
        genState.data = data;
        return nextValid;
    }

    void updateTexGen() {
        auto texGens = getTextureGens();
        for (size_t i = 0; i < MAX_TEXTURES_QUEUED; i++) {
            auto& genState = texGens[i];
            if (genState.state == TEX_GEN_WAIT) {
                auto& data = genState.data;
                genState.state = TEX_GEN_PROCESSING;

                if (genState.texture) {
                    std::shared_ptr<Texture>& tex = *genState.texture;
                    if (!tex) {
                        tex = std::make_shared<Texture>();
                    }

                    if (tex) {
                        tex->create(data.data, data.format, data.paletteSize, data.width, data.height, data.filter, data.flags);
                    }
                }

                genState.state = TEX_GEN_IDLE;
                genState.texture = {};
                genState.data = {};
            }
        }
    }
}