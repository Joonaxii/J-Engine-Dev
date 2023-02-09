#pragma once
#include <cstdint>
#include <istream>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Assets/Graphics/Texture/TextureFormat.h>
#include <JEngine/Assets/Decoders/DecoderResult.h>

namespace JEngine 
{
	static constexpr uint32_t BMP_FLG_FLIP_X = 0x01U;
	static constexpr uint32_t BMP_FLG_FLIP_Y = 0x02U;

	static constexpr uint32_t BMP_ERR_INVALID_IDENTIFIER      = 0x0001U;
	static constexpr uint32_t BMP_ERR_TOO_SMALL				  = 0x0002U;
	static constexpr uint32_t BMP_ERR_DATA_SIZE_MISMATCH      = 0x0004U;
	static constexpr uint32_t BMP_ERR_RESERVED_BYTES_NOT_ZERO = 0x0008U;

	static constexpr uint32_t BMP_ERR_UNSUPPORTED_BITDEPTH	  = 0x001000U;
	static constexpr uint32_t BMP_ERR_UNSUPPORTED_COMPRESSION = 0x002000U;

	DecoderResult decodeBmp(const std::string& filepath, const UI32Flags flags = BMP_FLG_FLIP_Y, int64_t dataSize = -1);
	DecoderResult decodeBmp(std::istream& stream, const UI32Flags flags = BMP_FLG_FLIP_Y, int64_t dataSize = -1);

    inline std::string& appendBmpErrorString(std::string& strOut, const UI32Flags flag, const char* breakChar) {
        if (breakChar != "\0") { strOut.append(breakChar); }
        switch (flag)
        {
            default:
                strOut.append("Unknown error flag!");
                break;
            case BMP_ERR_INVALID_IDENTIFIER:
                strOut.append("Invalid BMP identifier!");
                break;
            case BMP_ERR_TOO_SMALL:
                strOut.append("Stream doesn't have enough data to read BMP header!");
                break;
            case BMP_ERR_DATA_SIZE_MISMATCH:
                strOut.append("Data size mismatch!");
                break;
            case BMP_ERR_RESERVED_BYTES_NOT_ZERO:
                strOut.append("Reserved bytes not zero!");
                break;

            case BMP_ERR_UNSUPPORTED_BITDEPTH:
                strOut.append("Unsupported bitdepth!");
                break;
            case BMP_ERR_UNSUPPORTED_COMPRESSION:
                strOut.append("Unsupported compression!");
                break;
        }
        return strOut;
    }

    template<>
    inline std::string& getDecodeErrorString<AssetDataFormat::BMP>(std::string& strOut, const UI32Flags flags) {
        if (!flags) {
            strOut.append("Decode Ok!");
            return strOut;
        }

        size_t count = 0;
        for (uint32_t i = 0, j = 1; i < 32; i++, j <<= 1) {
            if ((flags & j)) {
                size_t cBef = count;
                appendBmpErrorString(strOut, j, count++ ? ", " : "\0");
                if (cBef == count) {
                    appendDecodeErrorString(strOut, j, count++ ? ", " : "\0");
                }
            }
        }
        return strOut;
    }
}