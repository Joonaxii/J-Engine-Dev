#pragma once
#include <zlib.h>
#include <JEngine/IO/Helpers/FileExtensions.h>

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
        static constexpr int32_t CHUNK = 1 << 14;
        const int deflateData(std::istream& streamIn, std::ostream& target, const int level);
        const int inflateData(std::istream& streamIn, std::ostream& target);
        const void zerr(const int ret);

    }
}