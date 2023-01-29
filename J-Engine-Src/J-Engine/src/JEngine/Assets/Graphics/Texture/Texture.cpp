#include <JEngine/Assets/Graphics/Texture/Texture.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>

namespace JEngine {
    const int32_t findClosest(Span<uint8_t>& palette, const uint8_t value) {
        int16_t dist = INT16_MAX;
        int32_t index = -1;

        for (size_t i = 0; i < palette.length(); i += 4) {
            auto dst = std::abs(palette[i] - value);

            if (dst <= dist) {
                dist = dst;
                index = int32_t(i);
                if (dst == 0) { return index; }
            }
        }
        return index;
    }

    const int32_t findClosest(Span<uint8_t>& palette, const JColor24 rgb) {
        int16_t dist = INT16_MAX;
        int32_t index = -1;

        int16_t pix = rgb.r + rgb.g + rgb.b;
        for (size_t i = 0; i < palette.length(); i += 4) {
            int16_t pal = palette[i] + palette[i + 1] + palette[i + 2];
            auto dst = std::abs(pal - pix);

            if (dst <= dist) {
                dist = dst;
                index = int32_t(i);
                if (dst == 0) { return index; }
            }
        }
        return index;
    }

    const int32_t findClosest(Span<uint8_t>& palette, const JColor32 rgba) {
        int16_t dist = INT16_MAX;
        int32_t index = -1;

        int16_t pix = rgba.r + rgba.g + rgba.b + rgba.a;
        for (size_t i = 0; i < palette.length(); i += 4) {
            int16_t pal = palette[i] + palette[i + 1] + palette[i + 2] + palette[i + 3];
            auto dst = std::abs(pal - pix);

            if (dst <= dist) {
                dist = dst;
                index = int32_t(i);
                if (dst == 0) { return index; }
            }
        }
        return index;
    }

    const int32_t Texture::generatePaletteAndTexture(const uint8_t* inputData, const uint16_t width, const uint16_t height, const TextureFormat format, uint8_t* outputData, const bool validate) {
        assert(format != TextureFormat::Indexed && "Input data can't be indexed!");
        int32_t count = 0;

        const int32_t reso = width * height;
        Span<uint8_t> data(outputData, (256 * 4) + reso);
        Span<uint8_t> paletteData = data.slice(0, 0);
        Span<uint8_t> pixelData = data.slice(256 * 4);

        const int32_t dataSize = (getBitsPerPixel(format) >> 3) * reso;
        bool isFull = false;
        switch (format) {
            case TextureFormat::R8:
                for (size_t i = 0; i < reso; i++) {
                    int32_t ind = isFull ? findClosest(paletteData, inputData[i]) : paletteData.indexOf(inputData[i]);
                    if (ind < 0) {
                        if (validate && isFull) { return -1; }
                        ind = isFull ? 0 : count++;
                        isFull = count >= 256;
                    }
                    pixelData[i] = uint8_t(ind);
                }
                break;
            case TextureFormat::RGB24: {
                Span<JColor24> paletteRGB = paletteData.castTo<JColor24>();
                for (size_t i = 0, j = 0; i < reso; i++, j += 3) {
                    const auto rgb = JColor24(inputData[j], inputData[j + 1], inputData[j + 2]);
                    int32_t ind = isFull ? findClosest(paletteData, rgb) : paletteRGB.indexOf(rgb);
                    if (ind < 0) {
                        if (validate && isFull) { return -1; }
                        ind = isFull ? 0 : count++;
                        isFull = count >= 256;
                    }
                    pixelData[i] = uint8_t(ind);
                }
                break;
            }
            case TextureFormat::RGBA32: {
                Span<JColor32> paletteRGB = paletteData.castTo<JColor32>();
                for (size_t i = 0, j = 0; i < reso; i++, j += 3) {
                    const auto rgb = JColor32(inputData[j], inputData[j + 1], inputData[j + 2]);
                    int32_t ind = isFull ? findClosest(paletteData, rgb) : paletteRGB.indexOf(rgb);
                    if (ind < 0) {
                        if (validate && isFull) { return -1; }
                        ind = isFull ? 0 : count++;
                        isFull = count >= 256;
                    }
                    pixelData[i] = uint8_t(ind);
                }
                break;
            }
        }
        return count;
    }

    void Texture::readPixel(const ConstSpan<uint8_t>& data, const TextureFormat format, const int32_t index, JColor32& color) {
        switch (format)
        {
            case TextureFormat::R8:
                color.r = color.g = color.b = data[index];
                color.a = 0xFF;
                break;
            case TextureFormat::Indexed: {
                const ConstSpan<uint8_t> pixelData = data.slice(256LL * 4);
                memcpy(&color, &data[pixelData[index]], 4);
                break;
            }
            case TextureFormat::RGBA32:
                memcpy(&color, &data[index], 4);
                break;
            case TextureFormat::RGB24:
                color.a = 0xFF;
                memcpy(&color, &data[index], 3);
                break;
        }
    }

    Texture::Texture() : _size(0, 0), _texFlags(0), _format(TextureFormat::RGBA32), _filter(FilterMode::Nearest), _pixelData(nullptr),
        _palId(0), _texId(0) {
    }

    Texture::~Texture() {
        freePalette();
        freeTexture();

        if (_pixelData && (_texFlags & FLAG_KEEP_DATA)) {
            free(_pixelData);
        }
    }

    const bool Texture::create(const uint16_t width, const uint16_t height) {
        return create(width, height, _format, _filter);
    }

    const bool Texture::create(const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter) {
        return create(width, height, format, filter, NULL, _texFlags & FLAG_KEEP_DATA);
    }

    const bool Texture::create(const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter, uint8_t* pixelData, const bool keepData) {
        const uint32_t maxSize = getMaximumSize();
        const bool validSize = width > 0 && height > 0 && height <= maxSize && width <= maxSize;
        if (!validSize) {
            std::cout << "[JEngine - Texture] Error: Failed to create texture! (Invalid Resolution)" << std::endl;
            return false;
        }

        const int32_t bitsPerPixel = getBitsPerPixel(format);
        const int32_t bytesPerPixel = bitsPerPixel >> 3;

        if (bytesPerPixel < 0) {
            std::cout << "[JEngine - Texture] Error: Failed to create texture! (Invalid Bit Depth)" << std::endl;
            return false;
        }

        _size.x = width;
        _size.y = height;

        _texFlags.setBit(FLAG_FLIPPED, false);

        _format = format;
        _filter = filter;
        applyData(pixelData, true);

        _texFlags.setBit(FLAG_KEEP_DATA, keepData);
        if (keepData) {
            if (_pixelData) { free(_pixelData); }
            _pixelData = pixelData;
        }
        return true;
    }

    void Texture::setPalette(const uint8_t* pixels, const size_t size, const TextureFormat format) {
        assert(format != TextureFormat::Indexed && "A palette cannot have Indexed pixel format!");
        assert(_format == TextureFormat::Indexed && "Texture must be Indexed!");
        assert(pixels != nullptr && "Palette can't be null!");

        const int32_t bitsPerPixel = getBitsPerPixel(format);
        const int32_t bytesPerPixel = bitsPerPixel >> 3;

        assert(bytesPerPixel > 0 && "Invalid bits per pixel value!");
        const int32_t entries = std::min<int32_t>(int32_t(size / bytesPerPixel), 256);

        uint8_t* pixelData = reinterpret_cast<uint8_t*>(malloc(256 * sizeof(JColor32)));
        if (pixelData) {
            if (format == TextureFormat::RGBA32) {
                memcpy(pixelData, pixels, size);
            }
            else {
                uint8_t r = 0, g = 0, b = 0, a = 0xFF;
                for (size_t i = 0, j = 0, l = 0; i < entries; i++, j += bytesPerPixel, l += 4)
                {
                    switch (format)
                    {
                        case TextureFormat::R8:
                            r = g = b = pixels[j];
                            break;
                        case TextureFormat::RGB24:
                            r = pixels[j];
                            g = pixels[j + 1];
                            b = pixels[j + 2];
                            break;
                    }

                    pixelData[l + 0] = r;
                    pixelData[l + 1] = g;
                    pixelData[l + 2] = b;
                }
            }
        }

        applyPalette(pixelData);
        free(pixelData);
    }

    const JVector2i Texture::getSize() const {
        return JVector2i(_size.x, _size.y);
    }

    const uint32_t Texture::getMaximumSize() { return 8192; }

    uint8_t* Texture::getTextureData() const {
        if (!_texId) { return nullptr; }

        if (_texFlags & FLAG_KEEP_DATA && _pixelData) {
            return _pixelData;
        }

        const int32_t offset = (_format == TextureFormat::Indexed ? 256 * 4 : 0);
        const int32_t bpp = getBitsPerPixel(_format);
        const int32_t reso = _size.x * _size.y;
        const int32_t size = reso * bpp + offset;

        uint8_t* data = reinterpret_cast<uint8_t*>(malloc(size));

        GLCall(glBindTexture(GL_TEXTURE_2D, _texId));
        GLCall(glPixelStorei(GL_PACK_ALIGNMENT, getGLPixelAlignment(_format)));
        GLCall(glGetTexImage(GL_TEXTURE_2D, 0, textureFormatToGLFormat(_format, false), GL_UNSIGNED_BYTE, data + offset));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

        if (_format == TextureFormat::Indexed && _palId) {
            GLCall(glBindTexture(GL_TEXTURE_1D, _palId));
            GLCall(glPixelStorei(GL_PACK_ALIGNMENT, 4));
            GLCall(glGetTexImage(GL_TEXTURE_1D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
            GLCall(glBindTexture(GL_TEXTURE_1D, 0));
        }
        return data;
    }

    const uint32_t Texture::getMainNativeHandle() const {
        return _texId;
    }

    const uint32_t Texture::getPaletteNativeHandle() const {
        return _palId;
    }

    void Texture::update(const uint8_t* pixels, const TextureFormat format, const uint32_t width, const uint32_t height, TextureFormat toFormat, const FilterMode filter, const bool keepData) {
        if (pixels && _texId) {
            const int32_t reso = width * height;

            const int32_t bPP = getBitsPerPixel(toFormat) >> 3;
            const int32_t size = bPP * reso + (toFormat == TextureFormat::Indexed ? 256 * 4 : 0);
            const int32_t sizeNew = 4 * reso + (toFormat == TextureFormat::Indexed ? 256 * 4 : 0);

            uint8_t* data = reinterpret_cast<uint8_t*>(malloc(sizeNew));

        start:
            if (data) {
                if (toFormat == format) {
                    memcpy(data, pixels, size);
                }
                else {
                    const int32_t bPPIn = getBitsPerPixel(format) >> 3;
                    if (toFormat == TextureFormat::Indexed) {
                        int32_t res = generatePaletteAndTexture(pixels, width, height, format, data, true);
                        if (res < 0) { toFormat = TextureFormat::RGBA32; goto start; }

                    }
                    else {
                        const int32_t bPPIn = getBitsPerPixel(format);
                        const size_t scanSizeFrom = width * bPPIn;
                        const size_t scanSizeTo = width * bPP;
                        ConstSpan<uint8_t> span(pixels, size);
                        JColor32 temp(0, 0, 0);
                        for (size_t y = 0, yP = 0, yPC = 0; y < height; y++, yP += scanSizeFrom, yPC += scanSizeTo) {
                            for (size_t x = 0, xP = 0, xPC = 0; x < width; x++, xP += bPPIn, xPC += bPP) {
                                readPixel(span, format, int32_t(yP + xP), temp);
                                memcpy(data + yPC + xPC, &temp, bPP);
                            }
                        }
                    }
                }
            }
            const bool newIsIndexed = toFormat == TextureFormat::Indexed;
            const int32_t offset = newIsIndexed ? 256 * 4 : 0;

            if (!newIsIndexed && _palId) {
                freePalette();
            }

            _filter = filter;
            _format = toFormat;
            _size.x = width;
            _size.y = height;

            applyPixels(data + offset, width, height, toFormat, filter);

            if (newIsIndexed) {
                if (!_palId) {
                    GLuint texture;
                    glGenTextures(1, &texture);
                    _palId = static_cast<uint32_t>(texture);
                }

                applyPalette(data);
            }

            _texFlags.setBit(FLAG_KEEP_DATA, keepData);
            if (keepData) {
                if (_pixelData) { free(_pixelData); }
                _pixelData = data;
            }
            else {
                free(data);
            }

            glFlush();
        }
    }

    void Texture::update(Texture& texture) {
        update(texture, _format);
    }

    void Texture::update(Texture& texture, const TextureFormat toFormat) {
        update(texture, toFormat, _filter);
    }

    void Texture::update(Texture& texture, const TextureFormat toFormat, const FilterMode filter) {
        auto pix = texture.getTextureData();
        update(pix, texture._format, texture._size.x, texture._size.y, toFormat, filter, _texFlags & FLAG_KEEP_DATA);
        free(pix);
    }
    const uint32_t Texture::bind(const uint32_t slot) const {
        uint32_t bindP = slot;

        GLCall(glActiveTexture(GL_TEXTURE0 + bindP++));
        GLCall(glBindTexture(GL_TEXTURE_2D, _texId));

        if (_format == TextureFormat::Indexed) {
            GLCall(glActiveTexture(GL_TEXTURE0 + bindP++));
            GLCall(glBindTexture(GL_TEXTURE_1D, _palId));;
        }
        return bindP;
    }

    const uint32_t Texture::unbind(const uint32_t slot) const {
        uint32_t bindP = slot;

        GLCall(glActiveTexture(GL_TEXTURE0 + bindP++));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

        if (_format == TextureFormat::Indexed) {
            GLCall(glActiveTexture(GL_TEXTURE0 + bindP++));
            GLCall(glBindTexture(GL_TEXTURE_1D, 0));;
        }
        return bindP;
    }

    void Texture::setTextureFormat(const TextureFormat& format) {
        if (_format == format) { return; }
        update(*this, format);
    }
    const TextureFormat Texture::getTextureFormat() const { return _format; }

    void Texture::setFilterMode(const FilterMode& filter) {
        if (_filter == filter) { return; }
        _filter = filter;

        if (_texId) {
            const uint32_t flt = filterModeToGLFilter(filter);

            glBindTexture(GL_TEXTURE_2D, _texId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flt);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flt);
        }
    }
    const FilterMode Texture::getFilterMode() const { return _filter; }

    void Texture::setCompressionLevel(const int32_t level) {
        _texFlags.setValue(uint16_t(level > 10 ? 10 : level < 0 ? 0 : level), VALUE_COMPRESSION);
    }

    const int32_t Texture::getCompressionLevel() const {
        return _texFlags.select(VALUE_COMPRESSION);
    }

    const uint32_t Texture::bindNull(const uint32_t slot) {
        uint32_t bindP = slot;

        GLCall(glActiveTexture(GL_TEXTURE0 + bindP++));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        return bindP;
    }

    void Texture::applyData(const uint8_t* pixels, const bool doFlush) {
        if (!_texId) {
            glGenTextures(1, &_texId);
        }

        const bool isIndexed = _format == TextureFormat::Indexed;

        applyPixels(pixels + (isIndexed ? 256 * 4 : 0), _size.x, _size.y, _format, _filter);
        _texFlags.setBit(FLAG_FLIPPED, false);

        if (_format == TextureFormat::Indexed) {
            if (!_palId) {
                glGenTextures(1, &_palId);
            }
            applyPalette(pixels);
        }

        if (!doFlush) { return; }
        glFlush();
    }

    void Texture::applyPixels(const uint8_t* pixels, const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter) {
        if (!_texId) { return; }

        const uint32_t flt = filterModeToGLFilter(filter);

        GLCall(glBindTexture(GL_TEXTURE_2D, _texId));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flt));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flt));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, getGLPixelAlignment(format)));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, textureFormatToGLFormat(format, true), width, height, 0, textureFormatToGLFormat(format, false), GL_UNSIGNED_BYTE, pixels));
    }

    void Texture::applyPalette(const uint8_t* palette) {
        if (!_palId) { return; }

        GLCall(glBindTexture(GL_TEXTURE_1D, _palId));
        GLCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
        GLCall(glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, palette));
    }

    void Texture::freeTexture() {
        if (_texId) {
            glDeleteTextures(1, &_texId);
            _texId = 0;
        }
    }

    void Texture::freePalette() {
        if (_palId) {
            glDeleteTextures(1, &_palId);
            _palId = 0;
        }
    }
}