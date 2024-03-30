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
                    JE_CORE_WARN("Couldn't build palette, too many colors!");
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
                    JE_CORE_WARN("Couldn't build palette, too many colors!");
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
                JE_CORE_WARN("Couldn't build palette, too many colors!");
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
            JE_CORE_WARN("[JEngine - ImageUtils] Warning: Couldn't build palette, bpp is too small {0}!", bpp);
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
                        JE_CORE_WARN("[JEngine - ImageUtils] Warning: Couldn't build palette, too many colors!");
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
            JE_CORE_WARN("Couldn't apply palette, invalid texture format! ({0})", getTextureFormatName(format));
            break;
        }
    }

    int32_t tryBuildPalette(const uint8_t* pixelData, uint32_t width, uint32_t height, TextureFormat format, uint8_t* newTexture, int32_t alphaClip) {
        switch (format)
        {
        case JEngine::TextureFormat::Unknown:
            JE_CORE_WARN("Couldn't build palette, texture format unknown!");
            return 0;
        case JEngine::TextureFormat::Indexed8:
        case JEngine::TextureFormat::Indexed16:
            JE_CORE_WARN("Didn't build palette, texture is already indexed!");
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
                    JE_CORE_WARN("Couldn't build palette, too many colors!");
                    return false;
                }

                ind = colCount;
                palette[colCount++] = buffer;
            }
            newPixData[i] = ind;
        }
        return colCount;
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
                    JE_CORE_WARN("Couldn't build palette, too many colors!");
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
        JE_CORE_ASSERT(index >= 0 && index < 4, "Index out of range");
        return _buffers[index];
    }

    ImageData& ImageBuffers::operator[](int32_t index) {
        JE_CORE_ASSERT(index >= 0 && index < 4, "Index out of range");
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
        JE_CORE_ASSERT(index >= 0 && index < 4, "Index out of range");
        uint8_t flag = (1 << index);
        auto& buf = _buffers[index];
        if (_flags & flag) {
            void* reloc = realloc(buf.data, size);
            if (!reloc) {
                JE_CORE_ERROR("[ImageUtils - Alloc] Failed to reallocate buffer! ({0} bytes)", size);
                return;
            }
            buf.data = reinterpret_cast<uint8_t*>(reloc);
        }
        else {
            buf.data = reinterpret_cast<uint8_t*>(malloc(size));
            if (!buf.data) {
                JE_CORE_ERROR("[ImageUtils - Alloc] Failed to allocate buffer! ({0} bytes)", size);
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

    void ImageData::resize(int32_t newWidth, int32_t newHeight, bool linear, ImageData* tempBuffer) {
        if ((newWidth == width && newHeight == height) || ((newHeight & newWidth) & 0x7FFFFFFF) == 0 || !data) { return; }

        ImageData tmpIM{};
        ImageData& tmp = tempBuffer ? *tempBuffer : tmpIM;

        int32_t oldW = width;
        int32_t oldH = height;

        int32_t bpp = (getBitsPerPixel(format) >> 3);
        int32_t resoNew = newWidth * newHeight;
        int32_t resoOld = width * height;

        if (!tmp.doAllocate(width, height, format, paletteSize, flags)) {
            return;
        }
        memcpy(tmp.data, data, getSize());

        if (!doAllocate(newWidth, newHeight, format)) {
            width = oldW;
            height = oldH;

            if (!tempBuffer) {
                tmp.clear(true);
            }
            return;
        }
        switch (format)
        {
        default:
            linear = false;
            break;
        case TextureFormat::RGB24:
        case TextureFormat::RGBA32:
            break;
        }


        int32_t scan = newWidth * bpp;
        int32_t scanOld = oldW * bpp;
        if (linear) {
            const uint8_t* bufferIn = tmp.getData();
            uint8_t* bufferOut = this->getData();
            JColor32 clr32[4]{
                {0x00, 0x00, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x00},
            };

            float xs = (float)oldW / width;
            float ys = (float)oldH / height;

            float fracx{ 0 }, fracy{ 0 }, ifracx{ 0 }, ifracy{ 0 }, sx{ 0 }, sy{ 0 }, l0{ 0 }, l1{ 0 }, rf{ 0 }, gf{ 0 }, bf{ 0 };
            int32_t  x0{ 0 }, x1{ 0 }, y0{ 0 }, y1{ 0 };

            JColor32 out{ 0x00, 0x00, 0x00, 0xFF };

            int32_t srcIdx = 0;

            for (int32_t y = 0, yP = 0; y < newHeight; y++, yP += scan) {
                for (int32_t x = 0, xP = yP; x < newWidth; x++, xP += bpp) {
                    sx = x * xs;
                    sy = y * ys;
                    x0 = (int)sx;
                    y0 = (int)sy;

                    // Calculate coordinates of the 4 interpolation points
                    fracx = sx - x0;
                    fracy = sy - y0;
                    ifracx = 1.0f - fracx;
                    ifracy = 1.0f - fracy;
                    x1 = x0 + 1;
                    if (x1 >= oldW)
                    {
                        x1 = x0;
                    }
                    y1 = y0 + 1;
                    if (y1 >= oldH)
                    {
                        y1 = y0;
                    }

                    // Read source color
                    const uint8_t* c = (bufferIn + (y0 * scanOld) + x0 * bpp);
                    memcpy(clr32 + 0, c, bpp);

                    c = bufferIn + (y0 * scanOld + x1 * bpp);
                    memcpy(clr32 + 1, c, bpp);

                    c = bufferIn + (y1 * scanOld + x0 * bpp);
                    memcpy(clr32 + 2, c, bpp);

                    c = bufferIn + (y1 * scanOld + x1 * bpp);
                    memcpy(clr32 + 3, c, bpp);

                    // Calculate colors
                    // Alpha
                    l0 = ifracx * clr32[0].a + fracx * clr32[1].a;
                    l1 = ifracx * clr32[2].a + fracx * clr32[3].a;
                    out.a = uint8_t(ifracy * l0 + fracy * l1);

                    // Write destination
                    if (out.a > 0)
                    {
                        // Red
                        l0 = ifracx * clr32[0].r + fracx * clr32[1].r;
                        l1 = ifracx * clr32[2].r + fracx * clr32[3].r;
                        rf = ifracy * l0 + fracy * l1;

                        // Green
                        l0 = ifracx * clr32[0].g + fracx * clr32[1].g;
                        l1 = ifracx * clr32[2].g + fracx * clr32[3].g;
                        gf = ifracy * l0 + fracy * l1;

                        // Blue
                        l0 = ifracx * clr32[0].b + fracx * clr32[1].b;
                        l1 = ifracx * clr32[2].b + fracx * clr32[3].b;
                        bf = ifracy * l0 + fracy * l1;

                        // Cast to byte
                        out.r = uint8_t(Math::clamp(rf, 0.0f, 255.0f));
                        out.g = uint8_t(Math::clamp(gf, 0.0f, 255.0f));
                        out.b = uint8_t(Math::clamp(bf, 0.0f, 255.0f));
                    }
                    else
                    {
                        out.r = 0x00;
                        out.g = 0x00;
                        out.b = 0x00;
                        out.a = 0x00;
                    }
                    memcpy(bufferOut + xP, &out, bpp);
                }
            }
        }
        else {
            const uint8_t* bufferIn = tmp.getData();
            uint8_t* bufferOut = this->getData();
            for (size_t y = 0, yP = 0; y < newHeight; y++, yP += scan) {
                int32_t srY = int32_t((oldH - 1) * (y / float(newHeight - 1.0f))) * scanOld;
                for (int32_t x = 0, xP = yP; x < newWidth; x++, xP += bpp) {
                    int32_t srX = ((oldW - 1) * x / float(newWidth - 1.0f)) * bpp;
                    memcpy(bufferOut + xP, bufferIn + srY + srX, bpp);
                }
            }
        }


        if (!tempBuffer) {
            tmp.clear(true);
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