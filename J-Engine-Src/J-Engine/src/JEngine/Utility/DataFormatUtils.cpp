#include <JEngine/Utility/StringHelpers.h>
#include <JEngine/Utility/DataFormatUtils.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/IO/MemoryStream.h>

namespace JEngine {
    const char* getDataFormatStr(DataFormat format) {
        auto fmts = getDataFormats();
        return fmts[format].signature;
    }
    const FormatInfo* getDataFormats() {
        static constexpr uint8_t PNG_SIG[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
        static constexpr uint8_t BMP_SIG[] = { 0x42, 0x4D, };
        static constexpr uint8_t JPEG_SIG[] = { 0xFF, 0xD8, 0xFF, 0xDB, };
        static constexpr uint8_t GIF87_SIG[] = { 0x47, 0x49, 0x46, 0x38, 0x37, 0x61, };
        static constexpr uint8_t GIF89_SIG[] = { 0x47, 0x49, 0x46, 0x38, 0x39, 0x61, };
        static constexpr uint8_t WEBP_SIG[] = { 0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x45, 0x42, 0x50, };
        static constexpr uint8_t WAVE_SIG[] = { 0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00, 0x57, 0x41, 0x56, 0x45, };
        static constexpr uint8_t OGG_SIG[] = { 0x4F, 0x67, 0x67, 0x53, };
        static constexpr uint8_t MP3_SIG[] = { 0x49, 0x44, 0x33, };
        static constexpr uint8_t TTF_SIG[] = { 0x00, 0x01, 0x00, 0x00, 0x00, };
        static constexpr uint8_t OTF_SIG[] = { 0x4F, 0x54, 0x54, 0x4F, };

        static constexpr uint8_t JPAK_SIG[] = { 'J', 'P', 'A', 'K', };
        static constexpr uint8_t JPRF_SIG[] = { 'J', 'P', 'R', 'F', };
        static constexpr uint8_t JMAT_SIG[] = { 'J', 'M', 'A', 'T', };
        static constexpr uint8_t JSHD_SIG[] = { 'J', 'S', 'H', 'D', };
        static constexpr uint8_t JGLI_SIG[] = { 'J', 'G', 'L', 'I', };
        static constexpr uint8_t JTEX_SIG[] = { 'J', 'T', 'E', 'X', };
        static constexpr uint8_t JRTX_SIG[] = { 'J', 'R', 'T', 'X', };
        static constexpr uint8_t JATL_SIG[] = { 'J', 'A', 'T', 'L', };
        static constexpr uint8_t JINP_SIG[] = { 'J', 'I', 'N', 'P', };
        static constexpr uint8_t JSCN_SIG[] = { 'J', 'S', 'C', 'N', };
        static constexpr uint8_t JPFB_SIG[] = { 'J', 'P', 'F', 'B', };
        static constexpr uint8_t JTXT_SIG[] = { 'J', 'T', 'X', 'T', };
        static constexpr uint8_t JBIN_SIG[] = { 'J', 'B', 'I', 'N', };
        static FormatInfo formats[_FMT_COUNT]{
            {}, {}, {}, {}, //Raw binary & text formats

            {}, //Identifiable formats marker

            FormatInfo(PNG_SIG  , 0x0FF, 8),
            FormatInfo(BMP_SIG  , 0x003, 2),
            FormatInfo(JPEG_SIG , 0x00F, 4),
            FormatInfo(GIF87_SIG, 0x03F, 6),
            FormatInfo(GIF89_SIG, 0x03F, 6),
            FormatInfo(WEBP_SIG , 0xF0F, 12),

            FormatInfo(WAVE_SIG , 0xF0F, 12),
            FormatInfo(OGG_SIG  , 0x00F, 4),
            FormatInfo(MP3_SIG  , 0x007, 3),

            FormatInfo(TTF_SIG  , 0x01F, 5),
            FormatInfo(OTF_SIG  , 0x00F, 4),

            FormatInfo(JPAK_SIG  , 0x00F, 4),
            FormatInfo(JPRF_SIG  , 0x00F, 4),
            FormatInfo(JMAT_SIG  , 0x00F, 4),
            FormatInfo(JSHD_SIG  , 0x00F, 4),
            FormatInfo(JGLI_SIG  , 0x00F, 4),
            FormatInfo(JTEX_SIG  , 0x00F, 4),
            FormatInfo(JRTX_SIG  , 0x00F, 4),
            FormatInfo(JATL_SIG  , 0x00F, 4),
            FormatInfo(JINP_SIG  , 0x00F, 4),
            FormatInfo(JSCN_SIG  , 0x00F, 4),
            FormatInfo(JPFB_SIG  , 0x00F, 4),
            FormatInfo(JTXT_SIG  , 0x00F, 4),
            FormatInfo(JBIN_SIG  , 0x00F, 4),
        };
        return formats;
    }

    size_t getLargestFormat() {
        static size_t maxSize = 0;
        if (!maxSize) {
            auto formats = getDataFormats();
            for (size_t i = _FMT_START; i < _FMT_COUNT; i++) {
                maxSize = std::max(maxSize, formats[i].size);
            }
        }
        return maxSize;
    }

    DataFormat complexDataAnalysis(const Stream& stream) {
        if (!stream.canRead()) { return DataFormat::FMT_BINARY; }

        uint8_t buffer[8192]{0};
        size_t bufSize = 0;

        static constexpr uint32_t JSON_FLAG_OPENING = 0x1;
        static constexpr uint32_t JSON_FLAG_CLOSING = 0x2;

        uint32_t state = 0;
        //JSON - START
        stream.seek(0, SEEK_SET);
        bufSize = stream.read<uint8_t, 4096>(buffer, false);

        int32_t pos = 0;
        while (pos < bufSize) {
            char c = buffer[pos++];
            if (isspace(c)) { continue; }
            if (c == '{') {
                state |= JSON_FLAG_OPENING;
            }
            break;
        }

        if (state) {
            stream.seek(std::max<int64_t>(0, stream.size() - 4096), SEEK_END);
            bufSize = stream.read<uint8_t, 4096>(buffer + 4096, true);

            pos = 0;
            while (pos < bufSize) {
                char c = buffer[pos++];
                if (isspace(c)) { continue; }
                if (c == '}') {
                    state |= JSON_FLAG_CLOSING;
                }
                break;
            }

            if (state == 0x3) {
                return DataFormat::FMT_JSON;
            }
        }
        //JSON - END


        //TEXT - START
        stream.seek(0, SEEK_SET);
        while (!stream.isEOF()) {
            bufSize = stream.read(buffer, false);
            if (bufSize < 1) { break; }
            if (!Helpers::validateUtf8(state, buffer, bufSize)) {
                break;
            }
        }

        if (state != 1) {
            return DataFormat::FMT_TEXT;
        }
        //TEXT - END
        return DataFormat::FMT_BINARY;
    }
    DataFormat complexDataAnalysis(const void* data, size_t size) {
        MemoryStream memStrm(reinterpret_cast<const uint8_t*>(data), size, size);
        return complexDataAnalysis(memStrm);
    }

    DataFormat getDataFormat(const Stream& stream, int64_t size, DataFormatFlags flags) {
        size_t start = stream.tell();
        size = size < 0 ? int64_t(stream.size() - start) : size;

        size_t pos = start;
        static size_t maxSize = getLargestFormat();

        uint8_t* buffer = reinterpret_cast<uint8_t*>(_malloca(maxSize));

        if (!buffer) {
            JENGINE_CORE_ERROR("[JEngine - DataUtils] Error: Couldn't allocate header buffer! ({0})", maxSize);
            return DataFormat::FMT_BINARY;
        }

        size_t len = stream.read(buffer, maxSize, false);
        auto fmt = getDataFormat(buffer, len, flags, false);
        if ((flags & FMT_F_RESET_POS) == 0) {
            pos += getDataHeaderSize(fmt);
        }

        uint8_t analysisM = flags >> FMT_F_ANALYZE_SHIFT;
        if (fmt == FMT_BINARY && analysisM > 0) {
            stream.seek(0, SEEK_SET);
            return complexDataAnalysis(stream);
        }
        stream.seek(pos, SEEK_SET);
        return fmt;
    }
    DataFormat getDataFormat(const void* data, size_t size, DataFormatFlags flags, bool containsData) {
        static const FormatInfo* formats = getDataFormats();
        DataFormat fmt = FMT_BINARY;

        for (size_t i = _FMT_START; i < _FMT_COUNT; i++) {
            if (formats[i].isValid(data, size)) {
                fmt = DataFormat(i);
                break;
            }
        }

        if (fmt == FMT_BINARY && containsData) {
            uint8_t analysisM = flags >> FMT_F_ANALYZE_SHIFT;
            if (analysisM > 0) {
                return complexDataAnalysis(data, size);
            }
        }
        return fmt;
    }

    size_t getDataHeaderSize(DataFormat fmt) {
        return getDataFormats()[fmt].size;
    }

    void writeHeader(const Stream& stream, DataFormat format) {
        if (format < 0 || format >= _FMT_COUNT) {
            return;
        }
        auto& fmts = getDataFormats()[format];
        stream.write(fmts.signature, fmts.size, false);
    }

    bool formatMatch(const Stream& stream, DataFormat format) {
        if (format < 0 || format >= _FMT_COUNT) {
            return false;
        }
        auto& fmts = getDataFormats()[format];

        uint8_t* data = reinterpret_cast<uint8_t*>(_malloca(fmts.size));
        if (!data) { return false; }

        stream.read(data, fmts.size, false);
        bool ret = fmts.isValid(data, fmts.size);

        _freea(data);
        return ret;
    }

    bool formatMatch(const void* data, size_t size, DataFormat format) {
        if (format < 0 || format >= _FMT_COUNT) {
            return false;
        }
        return getDataFormats()[format].isValid(data, size);
    }
}