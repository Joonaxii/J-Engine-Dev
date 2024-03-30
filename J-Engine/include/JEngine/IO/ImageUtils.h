#pragma once
#include <cstdint>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor4444.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <unordered_map>
#include <functional>

namespace JEngine {
    template<typename T>
    using PixelConvert = std::function<void(const uint8_t*, const uint8_t*, T*)>;

    enum class FilterMode : uint8_t {
        Nearest,
        Linear,
        __Count,
    };

    enum class WrapMode : uint8_t {
        Clamp,
        Repeat,
        __Count,
    };

    inline constexpr const char* getFilterModeName(const FilterMode format) {
        switch (format) {
            default:					return "Unknown";
            case FilterMode::Nearest:	return "Nearest";
            case FilterMode::Linear:	return "Linear";
        }
    }

    enum class TextureFormat : uint8_t {
        Unknown,

        R8,
        RG8,
        RGB24,
        RGBA32,
        RGBA4444,
        RGB48,
        RGBA64,
        Indexed8,
        Indexed16,
        RGBAF,

        __Count,

        //Support will be added later
        DXT1,
        DXT5,
    };

    inline constexpr int32_t getBitsPerPixel(const TextureFormat format) {
        switch (format) {
            default: return 0;

            case TextureFormat::R8:
            case TextureFormat::Indexed8:  return 8;

            case TextureFormat::RG8:
            case TextureFormat::RGBA4444:
            case TextureFormat::Indexed16: return 16;

            case TextureFormat::RGB24:     return 24;
            case TextureFormat::RGBA32:    return 32;

            case TextureFormat::RGB48:     return 48;
            case TextureFormat::RGBA64:    return 64;

            case TextureFormat::RGBAF:     return 128;

            case TextureFormat::DXT1:      return 4;
            case TextureFormat::DXT5:      return 8;
        }
    }
    inline constexpr const char* getTextureFormatName(const TextureFormat format) {
        switch (format) {
            default:						return "Unknown";

            case TextureFormat::R8:	        return "R8";
            case TextureFormat::RG8:	    return "RG8";

            case TextureFormat::Indexed8:	return "Indexed8";
            case TextureFormat::Indexed16:	return "Indexed16";

            case TextureFormat::RGB24:		return "RGB24";
            case TextureFormat::RGBA32:		return "RGBA32";

            case TextureFormat::RGBA4444:	return "RGBA4444";

            case TextureFormat::RGB48:		return "RGB48";
            case TextureFormat::RGBA64:		return "RGBA64";
            case TextureFormat::RGBAF:		return "RGBAF";

            case TextureFormat::DXT1:		return "DXT1";
            case TextureFormat::DXT5:		return "DXT5";
        }
    }

    bool hasAlpha(const uint8_t* data, int32_t width, int32_t height, TextureFormat format, int32_t paletteSize = -1);
    int32_t findInPalette(const JColor32 color, const JColor32* palette, uint32_t size);

    bool tryBuildPalette(JColor32 pixel, int32_t& colors, TextureFormat& format, uint8_t* newPalette, std::unordered_map<uint32_t, int32_t>& paletteLut, int32_t alphaClip = -1, int32_t absoluteMax = -1);

    bool tryBuildPalette8(JColor32 pixel, int32_t& colors, TextureFormat& format, uint8_t* newPalette, JColor32 buffer[32], uint32_t& bufSize, int32_t alphaClip = -1);
    bool tryBuildPalette16(JColor32 pixel, int32_t& colors, TextureFormat& format, uint8_t* newPalette, JColor32 buffer[256], uint32_t& bufSize, int32_t alphaClip = -1, int32_t maxSize = 256 * 256);

    bool tryBuildPalette(const uint8_t* pixelData, int32_t start, int32_t length, int32_t& colors, TextureFormat& format, int32_t bpp, uint8_t* newPalette, int32_t alphaClip = -1);
    void applyPalette(const uint8_t* pixelData, int32_t width, int32_t height, int32_t colors, TextureFormat format, uint8_t* targetPixels, TextureFormat paletteFmt, int32_t alphaClip = -1);

    bool tryBuildPalette(const uint8_t* pixelData, int32_t start, int32_t length, int32_t& colors, int32_t bpp, uint8_t* newTexture, int32_t alphaClip = -1);
    int32_t tryBuildPalette(const uint8_t* pixelData, uint32_t width, uint32_t height, TextureFormat format, uint8_t* newTexture, int32_t alphaClip = -1);

    constexpr size_t calculateTextureSize(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize) {
        switch (format) {
            case TextureFormat::R8:
                return size_t(width) * height;
            case TextureFormat::RGB24:
                return size_t(width) * height * 3;
            case TextureFormat::RGBA32:
                return size_t(width) * height * 4;
            case TextureFormat::RGBAF:
                return size_t(width) * height * 16;
            case TextureFormat::Indexed8:
                return size_t(width) * height + 1024;
            case TextureFormat::Indexed16:
                paletteSize = Math::isAlignedToPalette(paletteSize) ? paletteSize : Math::alignToPalette(paletteSize);
                return (size_t(width) * height * 2) + size_t(paletteSize) * 4;
            default: return 0;
        }
    }

    inline uint8_t remapUI16ToUI8(uint16_t value) {
        return value >> 8;
    }

    template<typename T>
    void convertPixel(TextureFormat format, const uint8_t* dataStart, const uint8_t* data, T& output) {
        static_assert("Not implemented for given type!");
    }

    template<>
    inline void convertPixel<JColor32>(TextureFormat format, const uint8_t* dataStart, const uint8_t* data, JColor32& output) {
        static PixelConvert<JColor32> converts[int32_t(TextureFormat::__Count)]{
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //Unknown
                    *output = Colors32::Clear;
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //R8
                    memset(output, data[0], 3);
                    output->a = 0xFF;
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //RG8
                    memcpy(output, data, 2);
                    output->b = 0x00;
                    output->a = 0xFF;
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //RGB24
                    memcpy(output, data, 3);
                    output->a = 0xFF;
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //RGBA32
                    memcpy(output, data, 4);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //Indexed8
                    memcpy(output, (dataStart + (size_t(*data) << 2)), 4);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //Indexed16
                    memcpy(output, (dataStart + ((size_t(data[0]) | size_t(data[1]) << 8) << 2)), 4);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //RGBA4444
                    *output = JColor32(*reinterpret_cast<const JColor4444*>(data));
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //RGB48
                    const uint16_t* dataUI = reinterpret_cast<const uint16_t*>(data);
                    output->r = remapUI16ToUI8(*dataUI++);
                    output->g = remapUI16ToUI8(*dataUI++);
                    output->b = remapUI16ToUI8(*dataUI++);
                    output->a = 0xFF;
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //RGBA64
                    const uint16_t* dataUI = reinterpret_cast<const uint16_t*>(data);
                    output->r = remapUI16ToUI8(*dataUI++);
                    output->g = remapUI16ToUI8(*dataUI++);
                    output->b = remapUI16ToUI8(*dataUI++);
                    output->a = remapUI16ToUI8(*dataUI++);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor32* output) {      //RGBAF
                   const JColor* pix = reinterpret_cast<const JColor*>(data);
                    *output = JColor32(pix[0]);
               }
        };

        int32_t fmt = int32_t(format);
        if (fmt < 0 || format >= TextureFormat::__Count) { output = Colors32::Clear; return; }
        converts[fmt](dataStart, data, &output);
    }

    template<>
    inline void convertPixel<JColor4444>(TextureFormat format, const uint8_t* dataStart, const uint8_t* data, JColor4444& output) {
        static PixelConvert<JColor4444> converts[int32_t(TextureFormat::__Count)]{
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //Unknown
                    *output = JColor4444();
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //R8
                    *output = JColor4444(data[0]);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //RG8
                    *output = JColor4444(data[0], data[1]);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //Indexed8
                    const uint8_t* pix = (dataStart + (size_t(*data) << 2));
                    *output = JColor4444(pix[0], pix[1], pix[2], pix[3]);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //Indexed16
                    const uint8_t* pix = (dataStart + ((size_t(data[0]) | size_t(data[1]) << 8) << 2));
                    *output = JColor4444(pix[0], pix[1], pix[2], pix[3]);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //RGB24
                   *output = JColor4444(data[0], data[1], data[2]);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //RGBA32
                    *output = JColor4444(data[0], data[1], data[2], data[3]);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //RGBA4444
                    memcpy(output, data, 2);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //RGB48
                   const uint16_t* pix = reinterpret_cast<const uint16_t*>(data);
                    *output = JColor4444(pix[0] >> 8, pix[1] >> 8, pix[2] >> 8);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //RGBA64
                   const uint16_t* pix = reinterpret_cast<const uint16_t*>(data);
                    *output = JColor4444(pix[0] >> 8, pix[1] >> 8, pix[2] >> 8, pix[3] >> 8);
               },
               [](const uint8_t* dataStart, const uint8_t* data, JColor4444* output) {      //RGBAF
                   const JColor* pix = reinterpret_cast<const JColor*>(data);
                    *output = JColor4444(pix[0]);
               }
        };

        int32_t fmt = int32_t(format);
        if (fmt < 0 || format >= TextureFormat::__Count) { output = JColor4444(); return; }
        converts[fmt](dataStart, data, &output);
    }
    enum : uint8_t {
        IMG_FLAG_HAS_ALPHA = 0x1,
        IMG_FLAG_ALIGNED = 0x80,
    };

    struct ImageData {
    public:
        int32_t width{ 0 };
        int32_t height{ 0 };
        TextureFormat format{ TextureFormat::Unknown };
        int32_t paletteSize{ 0 };
        uint8_t* data{ nullptr };
        uint8_t flags{ 0 };
        FilterMode filter[2]{};
        WrapMode wrap[2]{};

        constexpr ImageData() : width(), height(), format(), paletteSize(), data(), flags() {}
        constexpr ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data, uint8_t flags) :
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(flags), filter{ FilterMode::Nearest, FilterMode::Nearest }, wrap{ WrapMode::Clamp, WrapMode::Clamp } {}

        constexpr ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data) :
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(0), filter{ FilterMode::Nearest, FilterMode::Nearest }, wrap{ WrapMode::Clamp, WrapMode::Clamp } {}

        constexpr ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data, FilterMode filter) :
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(0), filter{ filter, filter }, wrap{ WrapMode::Clamp, WrapMode::Clamp }{}

        constexpr ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data, FilterMode filter, WrapMode wrap) :
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(0), filter{ filter, filter }, wrap{ wrap, wrap }{}

        constexpr ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data, FilterMode filterX, FilterMode filterY, WrapMode wrapX, WrapMode wrapY) :
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(0), filter{ filterX, filterY }, wrap{ wrapX, wrapY }{}


        constexpr bool isIndexed() const { return format >= TextureFormat::Indexed8 && format <= TextureFormat::Indexed16; }
        constexpr bool isAligned() const { return (flags & IMG_FLAG_ALIGNED) != 0; }
        constexpr size_t getPaletteOffset() const { return getPaletteOffset(format, paletteSize, isAligned()); }

        constexpr uint8_t* getData() const { return data + getPaletteOffset(); }
        uint8_t* getData() { return data + getPaletteOffset(); }

        constexpr uint8_t* getStart() const { return data; }
        uint8_t* getStart() { return data; }

        constexpr uint8_t* getFramedData(size_t index) const {
            return (data + getPaletteOffset()) + (index * getSize());
        }
        uint8_t* getFramedData(size_t index) {
            return (data + getPaletteOffset()) + (index * getSize());
        }

        constexpr uint8_t* getFramedStart(size_t index) const {
            return data + (index * getSize());
        }
        uint8_t* getFramedStart(size_t index) {
            return data + (index * getSize());
        }

        constexpr uint8_t* getFramedStart(size_t index, size_t frameSize) const {
            return data + (index * frameSize);
        }
        uint8_t* getFramedStart(size_t index, size_t frameSize) {
            return data + (index * frameSize);
        }

        constexpr size_t getBufferSize() const { return _bufferSize; }

        ImageData getFramed(size_t frame) const {
            if (data == nullptr) { return *this; }
            size_t framePos = frame * getSize();

            ImageData temp{};
            temp._bufferSize = _bufferSize - framePos;
            temp.data = data + framePos;
            temp.paletteSize = paletteSize;
            temp.format = format;
            temp.width = width;
            temp.height = height;
            temp.flags = flags;
            return temp;
        }

        constexpr size_t getSize() const {
            return calculateSize(width, height, format, paletteSize, (this->flags & IMG_FLAG_ALIGNED) != 0);
        }

        bool isEqual(const ImageData& img) const {
            if (this == &img) { return true; }
            bool indexed = isIndexed();
            if (_bufferSize != img._bufferSize ||
                width != img.width || height != img.height ||
                format != img.format || indexed != img.isIndexed() || (indexed && paletteSize != img.paletteSize)) {
                return false;
            }
            return memcmp(data, img.data, _bufferSize) == 0;
        }

        static constexpr bool isIndexed(TextureFormat format) {
            switch (format) {
            case TextureFormat::Indexed8:
            case TextureFormat::Indexed16: return true;
            default: return false;
            }
        }

        static constexpr int32_t getPaletteSize(TextureFormat format, int32_t rawPaletteSize, bool align) {
            switch (format) {
            case TextureFormat::Indexed8:
                return align ? 256 : Math::clamp(rawPaletteSize, 0, 256);
            case TextureFormat::Indexed16:
                return align ? Math::alignToPalette(rawPaletteSize) : Math::clamp(rawPaletteSize, 0, 256 * 256);
            default: return 0;
            }
        }

        static constexpr size_t calculateSize(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, bool alignedPalette) {
            return (size_t(width) * height) * (getBitsPerPixel(format) >> 3) + (getPaletteSize(format, paletteSize, alignedPalette) * sizeof(JColor32));
        }

        static constexpr size_t getPaletteOffset(TextureFormat format, int32_t paletteSize, bool alignedPalette) {
            return getPaletteSize(format, paletteSize, alignedPalette) * sizeof(JColor32);
        }

        bool doAllocate() {
            size_t required = getSize();
            if (data) {
                if (required <= _bufferSize) { return true; }
                void* reloc = realloc(data, required);
                if (!reloc) { return false; }
                data = reinterpret_cast<uint8_t*>(reloc);
            }
            else
            {
                data = reinterpret_cast<uint8_t*>(malloc(required));
            }

            _bufferSize = required;
            if (data) {
                memset(data, 0, required);
            }
            return data != nullptr;
        }

        bool doAllocate(size_t size, bool clear = true) {
            size_t required = size;
            if (data) {
                if (required <= _bufferSize)
                {
                    if (clear) {
                        memset(data, 0, size);
                    }
                    return true;
                }

                void* reloc = realloc(data, required);
                if (!reloc) { return false; }
                data = reinterpret_cast<uint8_t*>(reloc);
                _bufferSize = required;
            }
            else
            {
                data = reinterpret_cast<uint8_t*>(malloc(required));
                _bufferSize = required;
            }

            if (data && clear) {
                memset(data, 0, required);
            }
            return data != nullptr;
        }

        bool doAllocate(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize = 0, bool alignedPalette = false, uint32_t frames = 1, bool modify = true, bool clear = true) {
            if (modify) {
                this->width = width;
                this->height = height;
                this->format = format;
                this->flags = alignedPalette ? (this->flags | IMG_FLAG_ALIGNED) : (this->flags & ~IMG_FLAG_ALIGNED);
                this->paletteSize = getPaletteSize(format, paletteSize, alignedPalette);
            }
            return doAllocate(calculateSize(width, height, format, paletteSize, alignedPalette) * frames);
        }

        void resize(int32_t newWidth, int32_t newHeight, bool linear, ImageData* tempBuffer = nullptr);

        void replaceData(uint8_t* newData, bool destroy);
        void clear(bool destroy);
    private:
        size_t _bufferSize{ 0 };
    };

    struct ImageBuffers {
    public:
        enum : uint8_t {
            //Buffer Indices
            Buf_In = 0,
            Buf_Out = 1,
            Buf_Aux0 = 2,
            Buf_Aux1 = 3,

            //Flags
            Own_Buf_In = 1 << Buf_In,
            Own_Buf_Out = 1 << Buf_Out,
            Own_Buf_Aux0 = 1 << Buf_Aux0,
            Own_Buf_Aux1 = 1 << Buf_Aux1,
        };

        ImageBuffers() : _flags(0), _buffers{ } {}
        ImageBuffers(uint8_t flags, ImageData buffers[4]) :
            _flags(flags),
            _buffers{ buffers[0], buffers[1], buffers[2], buffers[3] } {}

        ~ImageBuffers();

        const ImageData& operator[](int32_t index) const;
        ImageData& operator[](int32_t index);

        bool getOwnFlag(uint8_t flag) const;
        void setOwnFlag(uint8_t flag, bool value);
        void allocBuffer(uint8_t index, size_t size);
        void clear();

    private:
        uint8_t _flags{ 0 };
        ImageData _buffers[4]{ };
    };
}