#include <JEngine/IO/ImageUtils.h>
#include <JEngine/Core/Assert.h>
#include <JEngine/Math/Math.h>

namespace JEngine {

    bool hasAlpha(const uint8_t* data, int32_t width, int32_t height, TextureFormat format, int32_t paletteSize) {
        int32_t reso = width * height;
        switch (format) {
            case JEngine::TextureFormat::R8:
            case JEngine::TextureFormat::RGB24:
            case JEngine::TextureFormat::Unknown: return false;

            default: {
                int32_t bpp = getBitsPerPixel(format) >> 3;
                for (size_t i = 0, j = 3; i < reso; i++, j += bpp) {
                    if (data[j] < 0xFF) { return true; }
                }
                return false;
            }

            case JEngine::TextureFormat::Indexed8:
                paletteSize = 256;
                break;
            case JEngine::TextureFormat::Indexed16:
                paletteSize = paletteSize < -1 ? 256 * 256 : paletteSize;
                break;

        }

        bool alpha = false;
        for (size_t i = 0, j = 3; i < paletteSize; i++, j += 4) {
            if (data[j] < 0xFF) { alpha = true; break; }
        }
        if (!alpha) { return false; }

        const JColor32* palette = reinterpret_cast<const JColor32*>(data);
        switch (format) {
            case JEngine::TextureFormat::Indexed8: {
                const uint8_t* pixels = data + paletteSize * 4;
                for (size_t i = 0; i < reso; i++) {
                    if (palette[pixels[i]].a < 255) { return true; }
                }
                break;
            }
            case JEngine::TextureFormat::Indexed16: {
                const uint16_t* pixels = reinterpret_cast<const uint16_t*>(data + paletteSize * 4);
                for (size_t i = 0; i < reso; i++) {
                    if (palette[pixels[i]].a < 255) { return true; }
                }
                break;
            }
        }
        return false;
    }

    int32_t findInPalette(const JColor32 color, const JColor32* palette, uint32_t size) {
        for (size_t i = 0; i < size; i++) {
            if (color == palette[i]) { return int32_t(i); }
        }
        return -1;
    }

    bool tryBuildPalette8(JColor32 pixel, int32_t& colors, TextureFormat& format, uint8_t* newPalette, JColor32 buffer[32], uint32_t& bufSize, int32_t alphaClip) {
        int32_t target = format == TextureFormat::Indexed16 ? 256 * 256 : 256;
        if (pixel.a <= alphaClip) {
            *reinterpret_cast<uint32_t*>(&pixel) = 0;
        }

        uint32_t bSize = bufSize > 8 ? 8 : bufSize;
        for (size_t i = 0; i < bSize; i++) {
            if (buffer[i] == pixel) { return true; }
        }

        JColor32* palette = reinterpret_cast<JColor32*>(newPalette);
        int32_t ind = findInPalette(pixel, palette, colors);
        if (ind < 0) {
            palette[colors++] = pixel;
            if (colors > target) {
                if (colors > 256 * 256) {
                    JENGINE_CORE_WARN("Couldn't build palette, too many colors!");
                    return false;
                }
                target = 256 * 256;
                format = TextureFormat::Indexed16;
            }
        }
        buffer[bufSize & 31] = pixel;
        bufSize++;
        return true;
    }
    bool tryBuildPalette16(JColor32 pixel, int32_t& colors, TextureFormat& format, uint8_t* newPalette, JColor32 buffer[256], uint32_t& bufSize, int32_t alphaClip, int32_t maxSize) {
        int32_t target = 256 * 256;
        if (pixel.a <= alphaClip) {
            *reinterpret_cast<uint32_t*>(&pixel) = 0;
        }
        maxSize = maxSize < 0 || maxSize > 256 * 256 ? 256 * 256 : maxSize;

        uint32_t bSize = bufSize > 8 ? 8 : bufSize;
        for (size_t i = 0; i < bSize; i++) {
            if (buffer[i] == pixel) { return true; }
        }

        JColor32* palette = reinterpret_cast<JColor32*>(newPalette);
        int32_t ind = findInPalette(pixel, palette, colors);
        if (ind < 0) {
            palette[colors++] = pixel;
            if (colors > target) {
                if (colors > maxSize) {
                    JENGINE_CORE_WARN("Couldn't build palette, too many colors!");
                    return false;
                }
            }
        }
        buffer[bufSize & 0xFF] = pixel;
        bufSize++;
        return true;
    }

    bool tryBuildPalette(JColor32 pixel, int32_t& colors, TextureFormat& format, uint8_t* newPalette, std::unordered_map<uint32_t, int32_t>& paletteLut, int32_t alphaClip, int32_t absoluteMax) {
        absoluteMax = absoluteMax <= 0 ? 256 * 256 : absoluteMax;

        uint32_t& colorInt = reinterpret_cast<uint32_t&>(pixel);
        if (pixel.a <= alphaClip) {
            colorInt = 0;
        }

        JColor32* palette = reinterpret_cast<JColor32*>(newPalette);
        auto find = paletteLut.find(colorInt);

        if (find == paletteLut.end()) {
            paletteLut[colorInt] = colors;
            palette[colors++] = pixel;

            if (colors > absoluteMax) {
                JENGINE_CORE_WARN("Couldn't build palette, too many colors!");
                return false;
            }

            if (format == TextureFormat::Indexed8 && colors > 256) {
                format = TextureFormat::Indexed16;
            }
        }
        return true;
    }
    bool tryBuildPalette(const uint8_t* pixelData, int32_t start, int32_t length, int32_t& colors, TextureFormat& format, int32_t bpp, uint8_t* newPalette, int32_t alphaClip) {
        if (bpp < 2) {
            format = TextureFormat::Unknown;
            JENGINE_CORE_WARN("[JEngine - ImageUtils] Warning: Couldn't build palette, bpp is too small {0}!", bpp);
            return false;
        }

        JColor32 buffer{ 0,0,0,0xFF };
        int32_t target = format == TextureFormat::Indexed16 ? 256 * 256 : 256;

        JColor32* palette = reinterpret_cast<JColor32*>(newPalette);
        for (size_t i = 0, j = 0, k = start; i < length; i++, j += bpp, k++) {
            memcpy(&buffer, pixelData + j, bpp);
            if (buffer.a <= alphaClip) {
                *reinterpret_cast<uint32_t*>(&buffer) = 0;
            }

            int32_t ind = findInPalette(buffer, palette, colors);
            if (ind < 0) {
                if (colors >= target) {
                    if (colors >= 256 * 256) {
                        JENGINE_CORE_WARN("[JEngine - ImageUtils] Warning: Couldn't build palette, too many colors!");
                        return false;
                    }
                    target = 256 * 256;
                    format = TextureFormat::Indexed16;
                }
                palette[colors++] = buffer;
            }
        }
        return true;
    }

    void applyPalette(const uint8_t* pixelData, int32_t width, int32_t height, int32_t colors, TextureFormat format, uint8_t* targetPixels, TextureFormat paletteFmt, int32_t alphaClip) {

        int32_t reso = width * height;
        JColor32* palette = reinterpret_cast<JColor32*>(targetPixels);
        JColor32 colorBuf{ 0,0,0,0xFF };
        int32_t bpp = getBitsPerPixel(format) >> 3;
        switch (paletteFmt)
        {
            case JEngine::TextureFormat::Indexed8: {
                uint8_t* pixData = targetPixels + 256 * 4;
                for (size_t i = 0, j = 0; i < reso; i++, j = 0) {
                    memcpy(&colorBuf, pixelData + j, bpp);
                    if (colorBuf.a <= alphaClip) {
                        *reinterpret_cast<uint32_t*>(&colorBuf) = 0;
                    }
                    int32_t index = findInPalette(colorBuf, palette, colors);
                    pixData[i] = uint8_t(index < 0 ? 0 : index);
                }
                break;
            }
            case JEngine::TextureFormat::Indexed16: {
                uint16_t* pixData = reinterpret_cast<uint16_t*>(targetPixels + colors * 4);
                for (size_t i = 0, j = 0; i < reso; i++, j = 0) {
                    memcpy(&colorBuf, pixelData + j, bpp);
                    if (colorBuf.a <= alphaClip) {
                        *reinterpret_cast<uint32_t*>(&colorBuf) = 0;
                    }
                    int32_t index = findInPalette(colorBuf, palette, colors);
                    pixData[i] = uint16_t(index < 0 ? 0 : index);
                }
                break;
            }
            default:
                JENGINE_CORE_WARN("Couldn't apply palette, invalid texture format! ({0})", getTextureFormatName(format));
                break;
        }
    }

    int32_t tryBuildPalette(const uint8_t* pixelData, uint32_t width, uint32_t height, TextureFormat format, uint8_t* newTexture, int32_t alphaClip) {
        switch (format)
        {
            case JEngine::TextureFormat::Unknown:
                JENGINE_CORE_WARN("Couldn't build palette, texture format unknown!");
                return 0;
            case JEngine::TextureFormat::Indexed8:
            case JEngine::TextureFormat::Indexed16:
                JENGINE_CORE_WARN("Didn't build palette, texture is already indexed!");
                return 0;
        }

        int32_t colCount = 0;
        JColor32 buffer{};

        JColor32* palette = reinterpret_cast<JColor32*>(newTexture);
        for (size_t i = 0; i < 256; i++) {
            palette[i].a = 0xFF;
        }

        uint32_t bpp = getBitsPerPixel(format);
        uint32_t reso = width * height;
        uint8_t* newPixData = newTexture + 1024;
        for (size_t i = 0, j = 0; i < reso; i++, j += bpp) {
            memcpy(&buffer, pixelData + j, bpp);
            if (buffer.a <= alphaClip) {
                *reinterpret_cast<uint32_t*>(&buffer) = 0;
            }

            int32_t ind = findInPalette(buffer, palette, colCount);
            if (ind < 0) {
                if (colCount >= 256) {
                    JENGINE_CORE_WARN("Couldn't build palette, too many colors!");
                    return false;
                }

                ind = colCount;
                palette[colCount++] = buffer;
            }
            newPixData[i] = ind;
        }
        return colCount;
    }

    void unpackRGB565(uint16_t value, uint8_t& r, uint8_t& g, uint8_t& b) {
        static uint8_t RGB565LUT[32 + 64]{};
        static bool init{ false };

        if (!init) {
            for (size_t i = 0; i < 32; i++) {
                RGB565LUT[i] = uint8_t((i * 255) / 31);
            }

            for (size_t i = 0; i < 64; i++) {
                RGB565LUT[i + 32] = uint8_t((i * 255) / 63);
            }
            init = true;
        }

        r = RGB565LUT[(value & RED_MASK_565)];
        g = RGB565LUT[32 + ((value & GREEN_MASK_565) >> 5)];
        b = RGB565LUT[(value & BLUE_MASK_565) >> 11];
    }

    void unpackRGB555(uint16_t value, uint8_t& r, uint8_t& g, uint8_t& b) {
        uint8_t a{};
        unpackRGB555(value, r, g, b, a);
    }

    void unpackRGB555(uint16_t value, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) {
        static uint8_t RGB555LUT[32]{};
        static bool init{ false };

        if (!init) {
            for (size_t i = 0; i < 32; i++) {
                RGB555LUT[i] = uint8_t((i * 255) / 31);
            }
            init = true;
        }

        r = RGB555LUT[(value & RED_MASK_555)];
        g = RGB555LUT[(value & GREEN_MASK_555) >> 5];
        b = RGB555LUT[(value & BLUE_MASK_555) >> 10];
        a = value & ALPHA_MASK_555 ? 0xFF : 0x00;
    }

    size_t calculateTextureSize(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize) {
        switch (format) {
            case JEngine::TextureFormat::R8:
                return size_t(width) * height;
            case JEngine::TextureFormat::RGB24:
                return size_t(width) * height * 3;
            case JEngine::TextureFormat::RGBA32:
                return size_t(width) * height * 4;
            case JEngine::TextureFormat::Indexed8:
                return size_t(width) * height + 1024;
            case JEngine::TextureFormat::Indexed16:
                paletteSize = Math::isAlignedToPalette(paletteSize) ? paletteSize : Math::alignToPalette(paletteSize);
                return (size_t(width) * height * 2) + size_t(paletteSize) * 4;
            default: return 0;
        }
    }

    bool tryBuildPalette(const uint8_t* pixelData, int32_t start, int32_t length, int32_t& colors, int32_t bpp, uint8_t* newTexture, int32_t alphaClip) {
        JColor32 buffer{ 0,0,0,0xFF };

        JColor32* palette = reinterpret_cast<JColor32*>(newTexture);
        uint8_t* newPix = (newTexture + 1024);
        for (size_t i = 0, j = 0, k = start; i < length; i++, j += bpp, k++) {
            memcpy(&buffer, pixelData + j, bpp);
            if (buffer.a <= alphaClip) {
                *reinterpret_cast<uint32_t*>(&buffer) = 0;
            }
            int32_t ind = findInPalette(buffer, palette, colors);
            if (ind < 0) {
                if (colors >= 256) {
                    JENGINE_CORE_WARN("Couldn't build palette, too many colors!");
                    return false;
                }

                palette[colors] = buffer;
                ind = colors++;
            }
            newPix[k] = uint8_t(ind);
        }
        return true;
    }

    ImageBuffers::~ImageBuffers() { clear(); }
    const ImageData& ImageBuffers::operator[](int32_t index) const {
        JENGINE_CORE_ASSERT(index >= 0 && index < 4, "Index out of range");
        return _buffers[index];
    }

    ImageData& ImageBuffers::operator[](int32_t index) {
        JENGINE_CORE_ASSERT(index >= 0 && index < 4, "Index out of range");
        return _buffers[index];
    }

    bool ImageBuffers::getOwnFlag(uint8_t flag) const { return bool(_flags & flag); }
    void ImageBuffers::setOwnFlag(uint8_t flag, bool value) {
        _flags &= ~flag;
        if (value) {
            _flags |= flag;
        }
    }

    void ImageBuffers::allocBuffer(uint8_t index, size_t size) {
        JENGINE_CORE_ASSERT(index >= 0 && index < 4, "Index out of range");
        uint8_t flag = (1 << index);
        auto& buf = _buffers[index];
        if (_flags & flag) {
            void* reloc = realloc(buf.data, size);
            if (!reloc) {
                JENGINE_CORE_ERROR("[ImageUtils - Alloc] Failed to reallocate buffer! ({0} bytes)", size);
                return;
            }
            buf.data = reinterpret_cast<uint8_t*>(reloc);
        }
        else {
            buf.data = reinterpret_cast<uint8_t*>(malloc(size));
            if (!buf.data) {
                JENGINE_CORE_ERROR("[ImageUtils - Alloc] Failed to allocate buffer! ({0} bytes)", size);
                return;
            }
        }
        _flags |= (1 << index);
    }

    void ImageBuffers::clear() {
        for (size_t i = 0, j = 1; i < 4; i++, j <<= 1) {
            _buffers[i].clear((_flags & j));
        }
    }

    void ImageData::replaceData(uint8_t* newData, bool destroy) {
        if (destroy && data) {
            free(data);
        }
        data = newData;
    }

    void ImageData::clear(bool destroy) {
        if (destroy && data) {
            free(data);
        }
        data = nullptr;
        format = TextureFormat::Unknown;
        paletteSize = 0;
        width = 0;
        height = 0;
        flags = 0;
    }
}