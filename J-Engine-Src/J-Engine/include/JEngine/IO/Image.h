#pragma once
#include <cstdint>
#include <JEngine/IO/FileStream.h>
#include <JEngine/IO/ImageUtils.h>
#include <JEngine/Utility/DataFormatUtils.h>
static constexpr uint8_t F_IMG_BUILD_PALETTE = 0x1;

namespace JEngine {
    struct ImageDecodeParams {
        uint8_t flags{ 0 };
    };

    struct ImageEncodeParams {
        uint32_t dpi{ 96 };
        int32_t compression{ 6 };
    };

    namespace Png {
        bool decode(const std::string& path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const char* path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const Stream& stream, ImageData& imgData, const ImageDecodeParams params = {});

        bool getInfo(const std::string& path, ImageData& imgData);
        bool getInfo(const char* path, ImageData& imgData);
        bool getInfo(const Stream& stream, ImageData& imgData);

        bool encode(const std::string& path, const ImageData& imgData, const uint32_t compression = 6);
        bool encode(const char* path, const ImageData& imgData, const uint32_t compression = 6);
        bool encode(const Stream& stream, const ImageData& imgData, const uint32_t compression = 6);
    }

    namespace Bmp {
        bool decode(const std::string& path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const char* path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const Stream& stream, ImageData& imgData, const ImageDecodeParams params = {});

        bool getInfo(const std::string& path, ImageData& imgData);
        bool getInfo(const char* path, ImageData& imgData);
        bool getInfo(const Stream& stream, ImageData& imgData);

        bool encode(const std::string& path, const ImageData& imgData, uint32_t dpi = 96);
        bool encode(const char* path, const ImageData& imgData, uint32_t dpi = 96);
        bool encode(const Stream& stream, const ImageData& imgData, uint32_t dpi = 96);
    }

    namespace DXT {
        bool decodeDxt1(const std::string& path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decodeDxt1(const char* path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decodeDxt1(const Stream& stream, ImageData& imgData, const ImageDecodeParams params = {});

        bool encodeDxt1(const std::string& path, const ImageData& imgData);
        bool encodeDxt1(const char* path, const ImageData& imgData);
        bool encodeDxt1(const Stream& stream, const ImageData& imgData);

        bool decodeDxt5(const std::string& path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decodeDxt5(const char* path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decodeDxt5(const Stream& stream, ImageData& imgData, const ImageDecodeParams params = {});

        bool encodeDxt5(const std::string& path, const ImageData& imgData);
        bool encodeDxt5(const char* path, const ImageData& imgData);
        bool encodeDxt5(const Stream& stream, const ImageData& imgData);
    }

    namespace DDS {
        bool decode(const std::string& path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const char* path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const Stream& stream, ImageData& imgData, const ImageDecodeParams params = {});

        bool getInfo(const std::string& path, ImageData& imgData);
        bool getInfo(const char* path, ImageData& imgData);
        bool getInfo(const Stream& stream, ImageData& imgData);

        bool encode(const std::string& path, const ImageData& imgData);
        bool encode(const char* path, const ImageData& imgData);
        bool encode(const Stream& stream, const ImageData& imgData);
    }

    namespace JTEX {
        bool decode(const std::string& path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const char* path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const Stream& stream, ImageData& imgData, const ImageDecodeParams params = {});

        bool getInfo(const std::string& path, ImageData& imgData);
        bool getInfo(const char* path, ImageData& imgData);
        bool getInfo(const Stream& stream, ImageData& imgData);

        bool encode(const std::string& path, const ImageData& imgData);
        bool encode(const char* path, const ImageData& imgData);
        bool encode(const Stream& stream, const ImageData& imgData);
    }

    namespace Image {
        bool tryGetInfo(const std::string& path, ImageData& imgData, DataFormat& format);
        bool tryGetInfo(const char* path, ImageData& imgData, DataFormat& format);
        bool tryGetInfo(const Stream& stream, ImageData& imgData, DataFormat& format);

        bool tryDecode(const std::string& path, ImageData& imgData, DataFormat& format, const ImageDecodeParams params = {});
        bool tryDecode(const char* path, ImageData& imgData, DataFormat& format, const ImageDecodeParams params = {});
        bool tryDecode(const Stream& stream, ImageData& imgData, DataFormat& format, const ImageDecodeParams params = {});

        bool tryEncode(const std::string& path, const ImageData& imgData, DataFormat format, const ImageEncodeParams& encodeParams = {});
        bool tryEncode(const char* path, const ImageData& imgData, DataFormat format, const ImageEncodeParams& encodeParams = {});
        bool tryEncode(const Stream& stream, const ImageData& imgData, DataFormat format, const ImageEncodeParams& encodeParams = {});
    }
}