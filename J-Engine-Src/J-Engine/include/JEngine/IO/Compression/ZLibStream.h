#pragma once
#include <zlib.h>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif
#include <cstdint>
#include <istream>

namespace JEngine {
    namespace ZLib {

        struct ZLibContext {
            z_stream stream;
            bool initialized = false;

            ZLibContext() : stream() {}
            ZLibContext(char* dataIn, const size_t lenIn, char* dataOut, const size_t lenOut) : stream() {
                init(dataIn, lenIn, dataOut, lenOut);
            }

            void init(char* dataIn, const size_t lenIn, char* dataOut, const size_t lenOut) {
                stream.total_in = 0;
                stream.avail_in = lenIn;
                stream.total_out = stream.avail_out = lenOut;
                stream.next_in = reinterpret_cast<uint8_t*>(dataIn);
                stream.next_out = reinterpret_cast<uint8_t*>(dataOut);

                initialized = true;
            }

            void refreshNext(char* dataIn, const size_t lenIn) {
                stream.next_in = reinterpret_cast<uint8_t*>(dataIn);
                stream.avail_in = lenIn;
            }
        };

        static constexpr int32_t CHUNK = 16384;
        int32_t deflateData(std::istream& streamIn, std::ostream& target, const int32_t level);
        int32_t inflateData(std::istream& streamIn, std::ostream& target);

        int32_t deflateData(char* dataIn, const size_t lenIn, char* dataOut, const size_t lenOut, const int32_t level);
        int32_t inflateData(char* dataIn, const size_t lenIn, char* dataOut, const size_t lenOut);

        int32_t inflateBegin(ZLibContext& context);
        int32_t inflateBegin(ZLibContext& context, char* dataOut, const size_t lenOut);
        int32_t inflateSegment(ZLibContext& context, char* dataIn, const size_t lenIn, int32_t& dataOut, const bool isLast);
        int32_t inflateEnd(ZLibContext& context, int32_t& dataOut);

        static inline constexpr char* zerr(const int32_t ret) {
            switch (ret) {
                default:              return "";
                case Z_ERRNO:         return "ZLib error!";
                case Z_STREAM_ERROR:  return "ZStream error! (Possibly invalid compression level)";
                case Z_DATA_ERROR:    return "Invalid or incomplete deflate data!";
                case Z_MEM_ERROR:     return "ZLib out of memory!";
                case Z_VERSION_ERROR: return "ZLib version mismatch!";
            }
        }
    }
}