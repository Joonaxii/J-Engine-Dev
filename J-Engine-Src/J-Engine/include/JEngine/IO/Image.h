#pragma once
#include <cstdint>
#include <JEngine/IO/FileStream.h>
#include <JEngine/IO/ImageUtils.h>
static constexpr uint8_t F_IMG_BUILD_PALETTE = 0x1;

namespace JEngine {
    struct ImageDecodeParams {
        uint8_t flags{0};
    };

    namespace Png {
        bool decode(const std::string& path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const char* path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const Stream& stream, ImageData& imgData, const ImageDecodeParams params = {});

        bool encode(const char* path, const ImageData& imgData, const uint32_t compression = 6);
        bool encode(const Stream& stream, const ImageData& imgData, const uint32_t compression = 6);
    }

    namespace Bmp {

        bool decode(const std::string& path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const char* path, ImageData& imgData, const ImageDecodeParams params = {});
        bool decode(const Stream& stream, ImageData& imgData, const ImageDecodeParams params = {});

        bool encode(const char* path, const ImageData& imgData);
        bool encode(const Stream& stream, const ImageData& imgData);
    }
}