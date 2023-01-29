#include <JEngine/Assets/Decoders/BMPDecoder.h>
#include <JEngine/IO/Helpers/FileExtensions.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <iostream>
#include <fstream>

namespace JEngine {

    static const int32_t calcualtePadding(const int32_t width, const int32_t bpp) {
        const int32_t rem = (width * bpp) & 0x3;
        return rem ? 4 - rem : 0;
    }

    static void appendErrorString(std::string& strOut, const uint32_t flag, const char* breakChar = "\0") {
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


            case FAILED_TO_ALLOCATE_BUFFER:
                strOut.append("Failed to allocate a buffer!");
                break;

            case IO_ERROR:
                strOut.append("File not found or couldn't be opened!");
                break;
        }
    }

    std::string& getBmpErrorString(std::string& strOut, const UI32Flags flags) {
        if (!flags) {
            strOut.append("Decode Ok!");
            return strOut;
        }

        size_t count = 0;
        for (uint32_t i = 0, j = 1; i < 32; i++, j <<= 1) {
            if ((flags & j)) {
                appendErrorString(strOut, j, count++ ? ", " : "\0");
            }
        }
        return strOut;
    }

    DecoderResult decodeBmp(const std::string& filepath, const UI32Flags flags, const int64_t dataSize) {
        std::ifstream stream(filepath, std::ios::binary);

        DecoderResult res = decodeBmp(stream, flags, dataSize);
        stream.close();
        return res;
    }

    DecoderResult decodeBmp(std::istream& stream, const UI32Flags flags, int64_t dataSize) {
        if (!stream.good()) {
            return { nullptr, {0, 0}, TextureFormat::Unknown, IO_ERROR };
        }

        const auto start = stream.tellg();

        if (dataSize < 0) {
            stream.seekg(0, std::ios::end);
            dataSize = stream.tellg() - start;
            stream.seekg(start, std::ios::beg);
        }

        if (dataSize < 54) {
            return { nullptr,JVector2<uint16_t>(), TextureFormat::Unknown,  BMP_ERR_TOO_SMALL };
        }

        uint8_t buffer[54];
        stream.read(reinterpret_cast<char*>(buffer), 54);

        Span<uint8_t> buf(buffer, 54);
        DecoderResult results = { nullptr, JVector2<uint16_t>(), TextureFormat::Unknown, DECODER_OK };

        const uint16_t id = buf.read<uint16_t>();
        const uint32_t size = buf.slice(2).read<uint32_t>();
        const uint32_t reserved = buf.slice(6).read<uint32_t>();
        const uint32_t offsetToData = buf.slice(10).read<uint32_t>();
      
        if (id != 0x4D42U)    { results.errFlags |= BMP_ERR_INVALID_IDENTIFIER; }
        if (size != dataSize) { results.errFlags |= BMP_ERR_DATA_SIZE_MISMATCH; }
        if (reserved != 0)    { results.errFlags |= BMP_ERR_RESERVED_BYTES_NOT_ZERO; }

        if (results.errFlags) { return results; }
        buf = buf.slice(14);

        int32_t dHdrSize    = buf.read<int32_t>();

        int32_t width       = buf.read<int32_t>(4);
        int32_t height      = buf.read<int32_t>(8);

        uint16_t bpp        = buf.read<uint16_t>(14);
        int32_t compression = buf.read<int32_t>(16);

        int32_t numOfColors = buf.read<int32_t>(32);

        switch (bpp)
        {
            case 8:
            case 24:
            case 32:
                break;
            default:
                results.errFlags |= BMP_ERR_UNSUPPORTED_BITDEPTH;
                break;
        }
        if(compression != 0 && compression != 0x3) { results.errFlags |= BMP_ERR_UNSUPPORTED_COMPRESSION; }

        if (results.errFlags) { return results; }

        const bool flipX = (flags & BMP_FLG_FLIP_X) ? (width  >= 0)  : (width  < 0);
        const bool flipY = (flags & BMP_FLG_FLIP_Y) ? (height >= 0)  : (height < 0);
        const uint32_t bytesPerPixel = bpp >> 3;

        if (width < 0) { width = -width; }
        if (height < 0) { height = -height; }
 
        const size_t reso = size_t(width) * height;
        const size_t reqData = reso;

        const int32_t padding = calcualtePadding(width, bytesPerPixel);

        const size_t rawScanSize = (width * bytesPerPixel);
        const size_t scanSize = rawScanSize + padding;
        char* scan = reinterpret_cast<char*>(alloca(scanSize));
        const size_t paletteOff = bpp == 8 ? 256LL * 4 : 0;

        const size_t pixelDataSize = rawScanSize * height;
        const size_t outSize = pixelDataSize + paletteOff;

        results.data = reinterpret_cast<uint8_t*>(malloc(outSize));

        if (!results.data || !scan) {
            results.errFlags |= FAILED_TO_ALLOCATE_BUFFER;
            return results;
        }

        memset(results.data, 0, outSize);
        results.resolution = JVector2<uint16_t>(uint16_t(width), uint16_t(height));

        //Take note of position before any potential extra data used for 
        //bit depts other than 24 bits.
        const auto pos = stream.tellg();

        const size_t end = (pixelDataSize - rawScanSize) + paletteOff;
        //Seek to start of data and read data to buffer
        stream.seekg(start + std::streamoff(offsetToData), std::ios::beg);
        for (size_t i = 0, sP = flipY ? 0 : paletteOff; i < height; i++, sP += rawScanSize) {
            stream.read(scan, scanSize);
            memcpy(results.data + (flipY ?  (end - sP) : sP), scan, rawScanSize);
        }

        stream.seekg(pos, std::ios::beg);
        switch (bpp)
        {
            case 8: {
                //Read palette, swap Red & Blue channels, and then set alpha to 255
                stream.read(reinterpret_cast<char*>(results.data), numOfColors * 4);
                JColor32* cPtr = reinterpret_cast<JColor32*>(results.data);

                for (size_t i = 0; i < numOfColors; i++) {
                    auto& color = cPtr[i];
                    std::swap(color.b, color.r);
                    color.a = 0xFF;
                }
                results.format = TextureFormat::Indexed;
                break;
            }
            case 24: {
                //Swap Red & Blue channels
                JColor24* cPtr = reinterpret_cast<JColor24*>(results.data);
                for (size_t i = 0; i < reso; i++) {
                    auto& color = cPtr[i];
                    std::swap(color.b, color.r);
                }
                results.format = TextureFormat::RGB24;
                break;
            }
            case 32: {
                //Read color bit masks and calculate the bit offsets for them
                uint32_t maskBuffer[4]{ 0 };
                stream.read(reinterpret_cast<char*>(maskBuffer), 16);

                const int32_t maskOffsets[4]{
                    Math::findFirstLSB(maskBuffer[0]),
                    Math::findFirstLSB(maskBuffer[1]),
                    Math::findFirstLSB(maskBuffer[2]),
                    Math::findFirstLSB(maskBuffer[3]),
                };

                //Apply color masks
                Span<uint8_t> spanData = Span<uint8_t>(results.data, outSize);
                JColor32* cPtr = reinterpret_cast<JColor32*>(results.data);
                for (size_t i = 0, j = 0; i < reso; i++, j += 4) {
                    const uint32_t data = spanData.read<uint32_t>(j);
                    auto& color = cPtr[i];

                    color.r = (data & maskBuffer[0]) >> maskOffsets[0];
                    color.g = (data & maskBuffer[1]) >> maskOffsets[1];
                    color.b = (data & maskBuffer[2]) >> maskOffsets[2];
                    color.a = (data & maskBuffer[3]) >> maskOffsets[3];
                }
                results.format = TextureFormat::RGBA32;
                break;
            }
        }
        return results;
    }
}