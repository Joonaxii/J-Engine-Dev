#include <JEngine/Utility/StringHelpers.h>
#include <JEngine/Utility/DataFormatUtils.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/IO/MemoryStream.h>

namespace JEngine {
    namespace Format {
        DataFormat complexDataAnalysis(const Stream& stream) {
            if (!stream.canRead()) { return DataFormat::FMT_BINARY; }

            uint8_t buffer[8192]{ 0 };
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

        DataFormat getFormat(const Stream& stream, int64_t size, DataFormatFlags flags) {
            size_t start = stream.tell();
            size = size < 0 ? int64_t(stream.size() - start) : size;

            size_t pos = start;
            static constexpr size_t LARGEST_HEADER = getLargest();

            uint8_t buffer[LARGEST_HEADER]{ 0 };

            size_t len = stream.read(buffer, LARGEST_HEADER, false);
            auto fmt = getFormat(buffer, len, flags, false);
            if ((flags & FMT_F_RESET_POS) == 0) {
                pos += getHeaderSize(fmt);
            }

            uint8_t analysisM = flags >> FMT_F_ANALYZE_SHIFT;
            if (fmt == FMT_BINARY && analysisM > 0) {
                stream.seek(0, SEEK_SET);
                return complexDataAnalysis(stream);
            }
            stream.seek(pos, SEEK_SET);
            return fmt;
        }
        DataFormat getFormat(const void* data, size_t size, DataFormatFlags flags, bool containsData) {
            DataFormat fmt = FMT_BINARY;

            for (size_t i = _FMT_START; i < _FMT_COUNT; i++) {
                if (FORMATS[i].isValid(data, size)) {
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

        void writeHeader(const Stream& stream, DataFormat format) {
            if (format < 0 || format >= _FMT_COUNT) {
                return;
            }
            FORMATS[format].writeSignature(stream, false);
        }

        bool formatMatch(const Stream& stream, DataFormat format) {
            if (format < 0 || format >= _FMT_COUNT) {
                return false;
            }
            auto& fmts = FORMATS[format];
            uint8_t data[getLargest()]{0};

            stream.read(data, fmts.size(), false);
            if (fmts.isValid(data, fmts.size())) {
                return true;
            }
            stream.seek(-fmts.size(), SEEK_CUR);
            return false;
        }

        bool formatMatch(const void* data, size_t size, DataFormat format) {
            if (format < 0 || format >= _FMT_COUNT) {
                return false;
            }
            return FORMATS[format].isValid(data, size);
        }
    }
}