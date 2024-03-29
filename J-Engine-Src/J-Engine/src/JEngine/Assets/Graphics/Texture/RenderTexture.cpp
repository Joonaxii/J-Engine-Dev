#include <JEngine/Assets/Graphics/Texture/RenderTexture.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <JEngine/Utility/DataFormatUtils.h>

namespace JEngine {
   /* static const TextureFormat getValidRenderTextureFormat(const TextureFormat tex) {
        switch (tex)
        {
            default: return tex;
            case:
        }
    }*/

    RenderTexture::RenderTexture() : Texture(), _depthId(0), _texDepth(0) {}

    RenderTexture::~RenderTexture() { }

    bool RenderTexture::create(const uint8_t* pixelData, TextureFormat format, int32_t paletteSize, int32_t width, int32_t height, FilterMode filter, uint8_t flags) {

        return false;
    }

    bool RenderTexture::update(const uint8_t* input, TextureFormat format, int32_t paletteSize, int32_t x, int32_t y, int32_t width, int32_t height, uint8_t flags) {
        //if (input && _textureId) {
        //    const int32_t reso = width * height;
        //
        //    const int32_t bPP = getBitsPerPixel(format) >> 3;
        //    const int32_t size = bPP * reso + (format == TextureFormat::Indexed8 ? 256 * 4 : 0);
        //    const int32_t sizeNew = 4 * reso + (format == TextureFormat::Indexed8 ? 256 * 4 : 0);
        //
        //    uint8_t* data = reinterpret_cast<uint8_t*>(malloc(sizeNew));
        //
        //start:
        //    if (data) {
        //        if (_format == format) {
        //            memcpy(data, input, size);
        //        }
        //        else {
        //            const int32_t bPPIn = getBitsPerPixel(format) >> 3;
        //            if (toFormat == TextureFormat::Indexed8) {
        //                int32_t res = generatePaletteAndTexture(pixels, width, height, format, data, true);
        //                if (res < 0) { toFormat = TextureFormat::RGBA32; goto start; }
        //
        //            }
        //            else {
        //                const int32_t bPPIn = getBitsPerPixel(format);
        //                const size_t scanSizeFrom = width * bPPIn;
        //                const size_t scanSizeTo = width * bPP;
        //                ConstSpan<uint8_t> span(pixels, size);
        //                JColor32 temp(0, 0, 0);
        //                for (size_t y = 0, yP = 0, yPC = 0; y < height; y++, yP += scanSizeFrom, yPC += scanSizeTo) {
        //                    for (size_t x = 0, xP = 0, xPC = 0; x < width; x++, xP += bPPIn, xPC += bPP) {
        //                        readPixel(span, format, int32_t(yP + xP), temp);
        //                        memcpy(data + yPC + xPC, &temp, bPP);
        //                    }
        //                }
        //            }
        //        }
        //    }
        //    const bool newIsIndexed = toFormat == TextureFormat::Indexed;
        //    const int32_t offset = newIsIndexed ? 256 * 4 : 0;
        //
        //    if (!newIsIndexed && _palId) {
        //        freePalette();
        //    }
        //
        //    _filter = filter;
        //    _format = toFormat;
        //    _size.x = width;
        //    _size.y = height;
        //
        //    applyPixels(data + offset, width, height, toFormat, filter);
        //
        //    if (newIsIndexed) {
        //        if (!_palId) {
        //            GLuint texture;
        //            glGenTextures(1, &texture);
        //            _palId = static_cast<uint32_t>(texture);
        //        }
        //
        //        applyPalette(data);
        //    }
        //
        //    free(data);
        //    glFlush();
        //}
        return false;
    }

    uint8_t RenderTexture::getDepth() const {
        return _texDepth;
    }

    bool RenderTexture::serializeBinary(const Stream& stream) const {
        Format::writeHeader(stream, FMT_JRTX);
        return true;
    }

    bool RenderTexture::deserializeBinary(const Stream& stream, const size_t size) {
        if (Format::formatMatch(stream, FMT_JRTX)) {
            return true;
        }
        return false;
    }

    bool RenderTexture::serializeJson(json& jsonF) const {

        return true;
    }

    bool RenderTexture::deserializeJson(json& jsonF) {

        return true;
    }

    void RenderTexture::applyData(const uint8_t* pixels, const bool doFlush) {
        //if (!_texId) {
        //    glGenTextures(1, &_texId);
        //}
        //
        //const bool isIndexed = _format == TextureFormat::Indexed;
        //applyPixels(pixels, _size.x, _size.y, _format, _filter);
        //applyDepthBuffer(_size.x, _size.y, _texDepth);
        //
        //if (doFlush) {
        //    GLCall(glFlush());
        //}
    }

    void RenderTexture::applyPixels(const uint8_t* pixels, const uint16_t width, const uint16_t height, const TextureFormat format, const FilterMode filter) {
       //const uint32_t flt = filterModeToGLFilter(filter);
       //
       //const uint8_t* pixelsBuf = pixels;
       //
       //GLCall(glBindTexture(GL_TEXTURE_2D, _texId));
       //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, flt));
       //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, flt));
       //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
       //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
       //
       //uint8_t* temp = nullptr;
       //if (pixels && format != TextureFormat::RGBA32) {
       //    const size_t pixelC = size_t(width) * height;
       //    temp = reinterpret_cast<uint8_t*>(malloc(pixelC * sizeof(JColor32)));
       //
       //    ConstSpan<uint8_t> data(pixels, pixelC * (getBitsPerPixel(format) >> 3));
       //    JColor32* tempC = reinterpret_cast<JColor32*>(temp);
       //    for (int32_t i = 0; i < pixelC; i++, tempC++)
       //    {
       //        readPixel(data, format, i, *tempC);
       //    }
       //    pixelsBuf = temp;
       //}
       //
       //GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
       //GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsBuf));
       //
       //if (temp) {
       //    free(temp);
       //}
    }

    void RenderTexture::applyPalette(const uint8_t* palette) { }

    void RenderTexture::applyDepthBuffer(const uint16_t width, const uint16_t height, const uint8_t depth) {
        freeDepthBuffer();
        if (depth) {
            glGenRenderbuffers(1, &_depthId);
            glBindRenderbuffer(GL_RENDERBUFFER, _depthId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthId);
            return;
        }
    }

    void RenderTexture::freeDepthBuffer() {
        if (_depthId) {
            GLCall(glDeleteRenderbuffers(GL_RENDERBUFFER, &_depthId));
            _depthId = 0;
        }
    }
}