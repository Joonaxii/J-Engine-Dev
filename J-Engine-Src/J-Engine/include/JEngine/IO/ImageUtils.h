#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor555.h>
#include <JEngine/Math/Graphics/JColor565.h>
#include <unordered_map>

namespace JEngine {

    enum class FilterMode : uint8_t {
        Nearest,
        Linear
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
        RGB24,
        RGBA32,
        RGBA4444,
        RGB48,
        RGBA64,
        Indexed8,
        Indexed16,
    };

    inline constexpr int32_t getBitsPerPixel(const TextureFormat format) {
        switch (format) {
            default: return 0;

            case TextureFormat::R8:        return 8;
            case TextureFormat::Indexed8:  return 8;
            case TextureFormat::Indexed16: return 16;
            case TextureFormat::RGB24:     return 24;
            case TextureFormat::RGBA32:    return 32;
        }
    }
    inline constexpr const char* getTextureFormatName(const TextureFormat format) {
        switch (format) {
            default:						return "Unknown";

            case TextureFormat::R8:	        return "R8";

            case TextureFormat::Indexed8:	return "Indexed8";
            case TextureFormat::Indexed16:	return "Indexed16";

            case TextureFormat::RGB24:		return "RGB24";
            case TextureFormat::RGBA32:		return "RGBA32";
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
    
    void unpackRGB565(uint16_t value, uint8_t& r, uint8_t& g, uint8_t& b);
    void unpackRGB555(uint16_t value, uint8_t& r, uint8_t& g, uint8_t& b);
    void unpackRGB555(uint16_t value, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);

    size_t calculateTextureSize(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize);

    constexpr uint8_t multUI8(uint32_t a, uint32_t b) {
        return uint8_t((a * b * 0x10101U + 0x800000U) >> 24);
    }

    inline uint8_t divUI8(int32_t a, int32_t b) {
        static bool init{ false };
        static uint8_t LUT[256 * 256]{ 0 };
        if (!init) {
            for (int32_t i = 0; i < 256; i++) {
                for (int32_t j = 0; j < 256; j++) {
                    LUT[i | (j << 8)] = uint8_t(j < 1 ? 0xFF : (i / float(j)) * 255.0f);
                }
            }
            init = true;
        }
        return LUT[(a & 0xFF) | ((b & 0xFF) << 8)];
    }

    uint8_t remapUI16ToUI8(uint16_t value);
    uint8_t remapUI8ToUI4(uint8_t value) {
        static bool init{ false };
        static uint8_t MAP[256]{};
        if (!init) {
            for (size_t i = 0; i < 256; i++)   {
                MAP[i] = (i * 15) / 255;
            }
            init = true;
        }
        return MAP[value];
    }

    inline uint8_t remapUI4ToUI8(uint8_t value) {
        static uint8_t LUT[16]{
            0,
            (1 * 255) / 15,
            (2 * 255) / 15,
            (3 * 255) / 15,
            (4 * 255) / 15,
            (5 * 255) / 15,
            (6 * 255) / 15,
            (7 * 255) / 15,
            (8 * 255) / 15,
            (9 * 255) / 15,
            (10 * 255) / 15,
            (11 * 255) / 15,
            (12 * 255) / 15,
            (13 * 255) / 15,
            (14 * 255) / 15,
            (15 * 255) / 15,
        };
        return LUT[value & 0xF];
    }

    enum : uint8_t {
        IMG_FLAG_HAS_ALPHA = 0x1,
    };

    struct ImageData {
    public:
        int32_t width{ 0 };
        int32_t height{ 0 };
        TextureFormat format{ TextureFormat::Unknown };
        int32_t paletteSize{ 0 };
        uint8_t* data{ nullptr };
        uint8_t flags{ 0 };

        ImageData() : width(), height(), format(), paletteSize(), data(), flags() {}
        ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data, uint8_t flags) :
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(flags) {}

        ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data) :
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(0) {}

        bool isIndexed() const { return format >= TextureFormat::Indexed8 && format <= TextureFormat::Indexed16; }
        bool hasAlpha() const {
            switch (format) {
                default: return false;

                case TextureFormat::Indexed8:
                case TextureFormat::Indexed16:
                case TextureFormat::RGBA4444:
                case TextureFormat::RGBA32:
                case TextureFormat::RGBA64:
                    return true;
            }
        }

        const uint8_t* getData() const {
            return isIndexed() ? data + (paletteSize * 4) : data;
        }

        uint8_t* getData() {
            return isIndexed() ? data + (paletteSize * 4) : data;
        }

        size_t getSize() const {
            size_t required = width * height;
            required *= (getBitsPerPixel(format) >> 3);
            if (isIndexed()) {
                required += paletteSize * sizeof(JColor32);
            }
            return required;
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

        void resize(int32_t newWidth, int32_t newHeight, uint8_t* buffer = nullptr);

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


#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::TextureFormat& itemRef) {
        yamlOut << YAML::Dec << static_cast<const uint16_t>(itemRef);
        return yamlOut;
    }

    template<>
    struct convert<JEngine::TextureFormat> {
        static Node encode(const JEngine::TextureFormat& rhs) {
            Node node;
            node.push_back(static_cast<const uint16_t>(rhs));
            return node;
        }

        static bool decode(const Node& node, JEngine::TextureFormat& rhs) {
            rhs = JEngine::TextureFormat(node.as<uint16_t>());
            return true;
        }
    };

    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::FilterMode& itemRef) {
        yamlOut << YAML::Dec << static_cast<const uint16_t>(itemRef);
        return yamlOut;
    }

    template<>
    struct convert<JEngine::FilterMode> {
        static Node encode(const JEngine::FilterMode& rhs) {
            Node node;
            node.push_back(static_cast<const uint16_t>(rhs));
            return node;
        }

        static bool decode(const Node& node, JEngine::FilterMode& rhs) {
            rhs = JEngine::FilterMode(node.as<uint16_t>());
            return true;
        }
    };
}
//JSON
namespace JEngine {
    template<>
    inline bool Serializable<TextureFormat>::deserializeJson(TextureFormat& itemRef, const json& jsonF, const TextureFormat& defaultVal) {
        itemRef = jsonF.is_number() ? TextureFormat(jsonF.get<uint8_t>()) : defaultVal;
        return true;
    }

    template<>
    inline bool Serializable<TextureFormat>::serializeJson(const TextureFormat& itemRef, json& jsonF) {
        jsonF = uint8_t(itemRef);
        return true;
    }

    template<>
    inline bool Serializable<FilterMode>::deserializeJson(FilterMode& itemRef, const json& jsonF, const FilterMode& defaultVal) {
        itemRef = jsonF.is_number() ? FilterMode(jsonF.get<uint8_t>()) : defaultVal;
        return true;
    }

    template<>
    inline bool Serializable<FilterMode>::serializeJson(const FilterMode& itemRef, json& jsonF) {
        jsonF = uint8_t(itemRef);
        return true;
    }
}
#pragma endregion