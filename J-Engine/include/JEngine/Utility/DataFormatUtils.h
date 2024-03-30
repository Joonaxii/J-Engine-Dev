#pragma once
#include <cstdint>
#include <functional>
#include <JEngine/IO/Stream.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Utility/EnumUtils.h>
#include <JEngine/Utility/HexStr.h>

namespace JEngine {
    enum DataFormat : uint8_t {
        FMT_UNKNOWN = 0xFF,
        FMT_BINARY = 0x00,
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

        FMT_JMAT, //Material Asset used by renderers
        FMT_JSHD, //Shader (Can be just a raw shader or an "include shader")

        FMT_JTEX, //Texture (Engine native texure format)
        FMT_JRTX, //Render Texture
        FMT_JSPR, //Sprite/Atlas (Specifies either a single sprite or an atlas of sprites on a given texture)

        FMT_JINP, //Input Config (Contains input bind "blueprint" and default input bindings)

        FMT_JSCN, //Scene
        FMT_JPFB, //Prefab

        FMT_JAUD, //Audio (Supports WAV and will possibly support OGG)
        FMT_JSEC, //Audio Section Asset
        FMT_JBPM, //Beat Map Asset

        FMT_JFNT, //Font (Wrapper for TTF/OTF/Bitmap font data)
        FMT_JDAT, //Raw Data (Raw binary/text)

        _FMT_COUNT,
    };

    struct FormatInfo {
        StrBase signature{};
        uint64_t mask = 0;

        constexpr FormatInfo() : signature{}, mask(0) {}
        constexpr FormatInfo(const StrBase& sig, uint64_t mask) :
            signature{ sig }, mask(mask)
        {
        }

        constexpr int32_t size() const {
            return signature.length();
        }

        void writeSignature(const Stream& stream, bool bigEndian = false) const {
            stream.write(signature.data(), signature.length(), bigEndian);
        }

        bool isValid(const void* data, size_t dataSize) const {
            if (!data || dataSize < signature.length() || !signature.isEmpty()) { return false; }

            ConstSpan<char> headerPtr = ConstSpan<char>(signature.data(), signature.length());
            ConstSpan<char> dataPtr   = ConstSpan<char>(reinterpret_cast<const char*>(data), signature.length());

            for (size_t i = 0, j = 1; i < signature.length(); i++, j <<= 1) {
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
        static constexpr FormatInfo FORMATS[_FMT_COUNT]{
                {}, {}, {}, {}, //Raw binary & text formats

                {}, //Identifiable formats marker

                FormatInfo(RAW_LE("‰PNG\x0D\x0A\x1A\x0A") , 0x0FF),
                FormatInfo(RAW_LE("BM")                   , 0x003),
                FormatInfo(RAW_LE("DDS ")                 , 0x00F),
                FormatInfo(RAW_LE("\xFF\xD8\xFF\xDB")     , 0x00F),
                FormatInfo(RAW_LE("GIF87a")               , 0x03F),
                FormatInfo(RAW_LE("GIF89a")               , 0x03F),
                FormatInfo(RAW_LE("RIFF\0\0\0\0WEBP")     , 0xF0F),

                FormatInfo(RAW_LE("RIFF\0\0\0\0WAVE")     , 0xF0F),
                FormatInfo(RAW_LE("OggS")                 , 0x00F),
                FormatInfo(RAW_LE("ID3")                  , 0x007),

                FormatInfo(RAW_LE("\x00\x01\x00\x00\x00") , 0x01F),
                FormatInfo(RAW_LE("OTTO")                 , 0x00F),

                // Engine Native formats
                FormatInfo(RAW_LE("JPAK"), 0x00F),
                FormatInfo(RAW_LE("JPRF"), 0x00F),
                FormatInfo(RAW_LE("JMAT"), 0x00F),
                FormatInfo(RAW_LE("JSHD"), 0x00F),
                FormatInfo(RAW_LE("JGLI"), 0x00F),
                FormatInfo(RAW_LE("JTEX"), 0x00F),
                FormatInfo(RAW_LE("JRTX"), 0x00F),
                FormatInfo(RAW_LE("JATL"), 0x00F),
                FormatInfo(RAW_LE("JINP"), 0x00F),
                FormatInfo(RAW_LE("JSCN"), 0x00F),
                FormatInfo(RAW_LE("JPFB"), 0x00F),
                FormatInfo(RAW_LE("JAUD"), 0x00F),
                FormatInfo(RAW_LE("JSEC"), 0x00F),
                FormatInfo(RAW_LE("JBPM"), 0x00F),
                FormatInfo(RAW_LE("JDAT"), 0x00F),
        };

        FORCE_INLINE constexpr const StrBase& getFormatStr(DataFormat format) {
            return FORMATS[format].signature;
        }

        FORCE_INLINE constexpr size_t getLargest() {
            size_t largest = 0;
            for (size_t i = _FMT_START + 1; i < _FMT_COUNT; i++)
            {
                largest = Math::max(largest, FORMATS[i].signature.length());
            }
            return largest;
        }

        FORCE_INLINE constexpr size_t getHeaderSize(DataFormat fmt) {
            return FORMATS[fmt].signature.length();
        }

        DataFormat getFormat(const Stream& stream, int64_t size = -1, DataFormatFlags flags = DataFormatFlags(0));
        DataFormat getFormat(const void* data, size_t size, DataFormatFlags flags = DataFormatFlags(0), bool containsData = false);
  
        void writeHeader(const Stream& stream, DataFormat format);

        bool formatMatch(const Stream& stream, DataFormat format);
        bool formatMatch(const void* data, size_t size, DataFormat format);
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
            "JEngine Beat Map",

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