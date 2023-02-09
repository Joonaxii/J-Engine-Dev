#pragma once
#include <cstdint>
#include <istream>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Assets/Graphics/Texture/TextureFormat.h>
#include <JEngine/Assets/Decoders/DecoderResult.h>

namespace JEngine 
{
	static constexpr uint32_t INVALID_PNG_SIGNATURE = 0x1;
	static constexpr uint32_t NO_IHDR_CHUNK = 0x2;
	static constexpr uint32_t UNSUPPORTED_BIT_DEPTH = 0x4;
	static constexpr uint32_t UNSUPPORTED_COLOR_FORMAT = 0x8;
	static constexpr uint32_t UNSUPPORTED_COLOR_FORMAT_OR_BIT_DEPTH = 0x10;
	static constexpr uint32_t NO_IDAT_CHUNKS = 0x20;

	DecoderResult decodePng(const std::string& filepath, const UI32Flags flags = 0, int64_t dataSize = -1);
	DecoderResult decodePng(std::istream& stream, const UI32Flags flags = 0, int64_t dataSize = -1);

    inline std::string& appendPngErrorString(std::string& strOut, const UI32Flags flag, const char* breakChar) {
        if (breakChar != "\0") { strOut.append(breakChar); }
        switch (flag)
        {
            default:
                strOut.append("Unknown error flag!");
                break;
            case INVALID_PNG_SIGNATURE:
                strOut.append("Invalid PNG signature!");
                break;
            case UNSUPPORTED_BIT_DEPTH:
                strOut.append("Unsupported bit depth!");
                break;
            case UNSUPPORTED_COLOR_FORMAT:
                strOut.append("Unsupported color format!");
                break;
            case UNSUPPORTED_COLOR_FORMAT_OR_BIT_DEPTH:
                strOut.append("Unsupported color format or bit depth!");
                break;
            case NO_IDAT_CHUNKS:
                strOut.append("No IDAT chunks found!");
                break;
        }
        return strOut;
    }  
    
    template<>
    inline std::string& getDecodeErrorString<AssetDataFormat::PNG>(std::string& strOut, const UI32Flags flags) {
        if (!flags) {
            strOut.append("Decode Ok!");
            return strOut;
        }

        size_t count = 0;
        for (uint32_t i = 0, j = 1; i < 32; i++, j <<= 1) {
            if ((flags & j)) {
                size_t cBef = count;
                appendPngErrorString(strOut, j, count++ ? ", " : "\0");

                if (cBef == count) {
                    appendDecodeErrorString(strOut, j, count++ ? ", " : "\0");
                }
            }
        }
        return strOut;
    }
}