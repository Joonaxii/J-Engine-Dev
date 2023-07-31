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

    enum : uint8_t {
        IMG_FLAG_HAS_ALPHA = 0x1,
    };

    struct ImageData {
        int32_t width{ 0 };
        int32_t height{ 0 };
        FilterMode filter{FilterMode::Nearest};
        TextureFormat format{ TextureFormat::Unknown };
        int32_t paletteSize{ 0 };
        uint8_t* data{ nullptr };
        uint8_t flags{0};

        ImageData() : width(), height(), format(), paletteSize(), data(), flags(), filter(FilterMode::Nearest) {}
        ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data, uint8_t flags) : 
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(flags), filter(FilterMode::Nearest) {}

        ImageData(int32_t width, int32_t height, TextureFormat format, int32_t paletteSize, uint8_t* data) : 
            width(width), height(height), format(format), paletteSize(paletteSize), data(data), flags(0), filter(FilterMode::Nearest) {}

        void replaceData(uint8_t* newData, bool destroy);
        void clear(bool destroy);
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