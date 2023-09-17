#pragma once
#include <cstdint>
#include <JEngine/IO/Stream.h>
#include <JEngine/Utility/Span.h>
#include <bitset>
#include <functional>
#include <JEngine/Utility/EnumUtils.h>

namespace JEngine {
    enum DataFormat : uint8_t {
        FMT_UNKNOWN = 0xFF,
        FMT_BINARY,
        FMT_TEXT,

        FMT_JSON, //Requires complex analysis
        FMT_YAML, //Requires complex analysis

        _FMT_START,

        //Texture formats
        FMT_PNG,
        FMT_BMP,
        FMT_DDS,
        FMT_JPEG,
        FMT_GIF87,
        FMT_GIF89,
        FMT_WEBP,

        //Audio formats
        FMT_WAVE,
        FMT_OGG,
        FMT_MP3,

        //Font formats
        FMT_TTF,
        FMT_OTF,

        //J-Engine Asset Types/Headers
        FMT_JPAK, //Asset Package (Format for packing assets into one file)

        FMT_JPRF, //Engine Preferences/Settings

        FMT_JMAT, //Material
        FMT_JSHD, //Shader (Can be just a raw shader or an "include shader")

        FMT_JTEX, //Texture (Engine native texure format)
        FMT_JRTX, //Render Texture
        FMT_JSPR, //Sprite/Atlas (Specifies either a single sprite or an atlas of sprites on a given texture)

        FMT_JINP, //Input Config (Contains input bind "blueprint" and default input bindings)

        FMT_JSCN, //Scene
        FMT_JPFB, //Prefab

        FMT_JAUD, //Audio (Audio isn't supported yet, possibly will support OGG and WAV)

        FMT_JFNT, //Font (Wrapper for TTF/OTF font data)
        FMT_JDAT, //Raw Data (Raw binary/text)

        _FMT_COUNT,
    };

    struct FormatInfo {
        const char* signature{0};
        uint64_t mask = 0;
        size_t size = 0;

        FormatInfo() : signature{0}, mask(0), size(0) {}
        FormatInfo(const void* sig, uint64_t mask, size_t size) :
            signature{reinterpret_cast<const char*>(sig)}, mask(mask), size(size > 64 ? 64 : 0)
        {
        }

        bool isValid(const void* data, size_t dataSize) const {
            if (!data || size == 0 || dataSize < size || !signature) { return false; }
            ConstSpan<char> headerPtr = ConstSpan<char>(signature, size);
            ConstSpan<char> dataPtr   = ConstSpan<char>(reinterpret_cast<const char*>(data), size);

            for (size_t i = 0, j = 1; i < size; i++, j <<= 1) {
                if ((mask & j) && (headerPtr[i] != dataPtr[i])) {
                    return false;
                }
            }
            return true;
        }
    };

    enum DataFormatFlags : uint8_t {
        FMT_F_NONE = 0x0,
        FMT_F_RESET_POS = 0x1,

        FMT_F_ANALYZE_SHIFT = 6,
        FMT_F_ANALYZE_MASK  = 0xC0,
        FMT_F_ANALYSIS_SIMPLE  = 1 << FMT_F_ANALYZE_SHIFT,
        FMT_F_ANALYSIS_COMPLEX = 2 << FMT_F_ANALYZE_SHIFT,
    };

    namespace Format {
        const char* getFormatStr(DataFormat format);
        const FormatInfo* getFormats();
        size_t getLargest();
        DataFormat getFormat(const Stream& stream, int64_t size = -1, DataFormatFlags flags = DataFormatFlags(0));
        DataFormat getFormat(const void* data, size_t size, DataFormatFlags flags = DataFormatFlags(0), bool containsData = false);
        size_t getHeaderSize(DataFormat fmt);

        void writeHeader(const Stream& stream, DataFormat format);

        bool formatMatch(const Stream& stream, DataFormat format);
        bool formatMatch(const void* data, size_t size, DataFormat format);
    };
    template<DataFormat format>
    struct DataHeader {
        static constexpr uint64_t Value = 0;
        static constexpr uint8_t Length = 0;
    };

    template<>
    inline constexpr int32_t JEngine::EnumNames<DataFormat>::Count{ _FMT_COUNT };

    template<>
    inline const char** JEngine::EnumNames<DataFormat>::getEnumNames() {
        static const char* names[Count] = {
            "Binary",
            "Text",
            "JSON",
            "YAML",

            "",

            "PNG",
            "BMP",
            "DDS",
            "JPEG",
            "GIF87",
            "GIF89",
            "WEBP",
            "WAVE",
            "OGG",
            "MP3",
            "TTF",
            "OTF",

            "JEngine Asset Pak",
            "JEngine Preferences",
            "JEngine Material",
            "JEngine Shader",
            "JEngine Texture",
            "JEngine Render Texture",
            "JEngine Sprite",
            "JEngine Input Config",
            "JEngine Scene",
            "JEngine Prefab",

            "JEngine Audio",

            "JEngine Font",
            "JEngine Data File",
        };
        return names;
    }

    template<>
    inline bool EnumNames<DataFormat>::noDraw(int32_t index) {
        auto names = getEnumNames();
        if (index < 2 || index >= Count || !names) { return true; }
        return isNoName(names[index]);
    }
}

#define S_TO_UINT16(val) uint16_t(val[0] | (val[1] << 8))
#define S_TO_UINT24(val) uint32_t(val[0] | (val[1] << 8) | (val[2] << 16))
#define S_TO_UINT32(val) uint32_t(val[0] | (val[1] << 8) | (val[2] << 16) | (val[3] << 24))
#define S_TO_UINT40(val) uint64_t(val[0] | (val[1] << 8) | (val[2] << 16) | (val[3] << 24) | (val[4] << 32))
#define S_TO_UINT48(val) uint64_t(val[0] | (val[1] << 8) | (val[2] << 16) | (val[3] << 24) | (val[4] << 32) | (val[5] << 40))
#define S_TO_UINT56(val) uint64_t(val[0] | (val[1] << 8) | (val[2] << 16) | (val[3] << 24) | (val[4] << 32) | (val[5] << 40) | (val[6] << 48))
#define S_TO_UINT64(val) uint64_t(val[0] | (val[1] << 8) | (val[2] << 16) | (val[3] << 24) | (val[4] << 32) | (val[5] << 40) | (val[6] << 48) | (val[7] << 56))

#define CREATE_DATA_HEADER(FORMAT, value, length) \
template<> \
inline constexpr uint64_t JEngine::DataHeader<FORMAT>::Value = value; \
template<> \
inline constexpr uint8_t JEngine::DataHeader<FORMAT>::Length = length; \
 \

CREATE_DATA_HEADER(JEngine::FMT_JPAK, S_TO_UINT32("JPAK"), 4);
CREATE_DATA_HEADER(JEngine::FMT_JPRF, S_TO_UINT32("JPRF"), 4);

CREATE_DATA_HEADER(JEngine::FMT_JMAT, S_TO_UINT32("JMAT"), 4);
CREATE_DATA_HEADER(JEngine::FMT_JSHD, S_TO_UINT32("JSHD"), 4);

CREATE_DATA_HEADER(JEngine::FMT_JTEX, S_TO_UINT32("JTEX"), 4);
CREATE_DATA_HEADER(JEngine::FMT_JRTX, S_TO_UINT32("JRTX"), 4);
CREATE_DATA_HEADER(JEngine::FMT_JSPR, S_TO_UINT32("JSPR"), 4);

CREATE_DATA_HEADER(JEngine::FMT_JINP, S_TO_UINT32("JINP"), 4);

CREATE_DATA_HEADER(JEngine::FMT_JSCN, S_TO_UINT32("JSCN"), 4);
CREATE_DATA_HEADER(JEngine::FMT_JPFB, S_TO_UINT32("JPFB"), 4);

CREATE_DATA_HEADER(JEngine::FMT_JAUD, S_TO_UINT32("JAUD"), 4);

CREATE_DATA_HEADER(JEngine::FMT_JFNT, S_TO_UINT32("JFNT"), 4);
CREATE_DATA_HEADER(JEngine::FMT_JDAT, S_TO_UINT32("JDAT"), 4);