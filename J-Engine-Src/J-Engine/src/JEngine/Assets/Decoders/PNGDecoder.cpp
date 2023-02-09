#include <JEngine/Assets/Decoders/PNGDecoder.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/IO/Helpers/IOHelpers.h>
#include <JEngine/IO/Compression/ZLibStream.h>
#include <cstdlib>
#include <fstream>

namespace JEngine {
    inline constexpr uint8_t PNG_SIGNATURE[]{ 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

    enum PNGFilterType : uint8_t {
        PF_None = 0,
        PF_Sub = 1,
        PF_Up = 2,
        PF_Average = 3,
        PF_Paeth = 4,
    };

    enum PNGChunk : uint32_t {
        CH_Unknown = 0x0000000,

        //Important Chunk types
        CH_IHDR = 0x52444849U,
        CH_PLTE = 0x45544C50U,
        CH_IDAT = 0x54414449U,
        CH_IEND = 0x44454E49U,

        CH_tRNS = 0x534E5274U,
    };

    enum PNGColorType : uint8_t {
        Grayscale = 0x0,
        Truecolor = 0x2,
        Indexed = 0x3,
        GrayAlpha = 0x4,
        TrueAlpha = 0x6,
    };

    struct Chunk {
        uint32_t length;
        PNGChunk chunkId;
        size_t dataStart;
        uint32_t crc;

        Chunk() : length(), chunkId(), dataStart(), crc() {}
    };

#pragma pack(push, 1)

    struct IHDR {
        int32_t width;
        int32_t height;

        uint8_t bitDepth;
        PNGColorType colorType;
        uint8_t compressionMethod;
        uint8_t filterMethod;
        uint8_t interlaceMethod;

        TextureFormat getTextureFormat() const {
            switch (colorType)
            {
                default:                 return TextureFormat::Unknown;
                case JEngine::GrayAlpha: return TextureFormat::Unknown;
                case JEngine::Grayscale: return TextureFormat::R8;
                case JEngine::Indexed:   return TextureFormat::Indexed;
                case JEngine::Truecolor: return TextureFormat::RGB24;
                case JEngine::TrueAlpha: return TextureFormat::RGBA32;
            }
        }
    };


#pragma pack(pop)

    inline constexpr bool validateColorAndBitDepth(const PNGColorType type, const uint8_t bitDepth, int32_t& bytesPerPix) {
        bytesPerPix = 0;
        switch (type)
        {
            default:                      return false;
            case PNGColorType::GrayAlpha: return false;

            case PNGColorType::Indexed:
                bytesPerPix = 1;
                return bitDepth == 8;

            case PNGColorType::Grayscale:
                bytesPerPix = 1;
                return bitDepth == 8;

            case PNGColorType::TrueAlpha:
                bytesPerPix = 4;
                return bitDepth == 8;

            case PNGColorType::Truecolor:
                bytesPerPix = 3;
                return bitDepth == 8;
        }
    }

    inline int32_t paethReverse(const int32_t a, const int32_t b, const int32_t c) {
        int32_t p  = a + b - c;
        int32_t pa = std::abs(p - a);
        int32_t pb = std::abs(p - b);
        int32_t pc = std::abs(p - c);

        if (pa <= pb && pa <= pc) { return a; }
        return pb <= pc ? b : c;
    }

    void reverseFilter(const PNGFilterType filter, char* current, char* prior, char* target, const int32_t size, const int32_t step) {
        memcpy(target, current, size);
        switch (filter)
        {
            case PF_Sub:
                for (int32_t i = step, j = 0; i < size; i++, j++) {
                    target[i] += target[j];
                }
                break;
            case PF_Up:
                for (int32_t i = 0; i < size; i++) {
                    target[i] += prior[i];
                }
                break;
            case PF_Average: {
                int32_t left;
                for (int32_t i = 0, j = -step; i < size; i++, j++) {
                    left = (j < 0 ? 0 : target[j]);
                    target[i] += ((int32_t(prior[i]) + left) >> 1) & 0xFF;
                }
                break;
            }
            case PF_Paeth: {
                int32_t a, b, c;
                for (int32_t i = 0, j = -step; i < size; i++, j++) {
                    a = j < 0 ? 0 : target[j];
                    b = prior[i];
                    c = prior[j];

                    target[i] += paethReverse(a, b, c);
                }
                break;
            }
        }
    }

    const bool readChunk(std::istream& stream, Chunk& chunk) {
        if (stream.eof()) { return false; }

        char buffer[8];
        stream.read(buffer, 8);
        Span<char> data(buffer, 8);

        chunk.length = data.slice(0, 4).readBigEndian<uint32_t>();
        chunk.chunkId = data.slice(4, 4).read<PNGChunk>();
        chunk.dataStart = stream.tellg();

        if (stream.eof()) { return false; }

        stream.seekg(chunk.length, std::ios::cur);
        stream.read(reinterpret_cast<char*>(&chunk.crc), 4);
        return true;
    }

    DecoderResult decodePng(const std::string& filepath, const UI32Flags flags, const int64_t dataSize) {
        std::ifstream stream(filepath, std::ios::binary | std::ios::in);

        DecoderResult res = decodePng(stream, flags, dataSize);
        stream.close();
        return res;
    }

    DecoderResult decodePng(std::istream& stream, const UI32Flags flags, int64_t dataSize) {
        if (!stream.good()) {
            return { nullptr, {0, 0}, TextureFormat::Unknown, IO_ERROR };
        }

        char buff[9]{ 0 };
        stream.read(buff, 8);

        //Basic file validation and PNG idetifier checking
        if (strcmp(buff, reinterpret_cast<const char*>(PNG_SIGNATURE)) != 0) {
            return { nullptr, {0, 0}, TextureFormat::Unknown, INVALID_PNG_SIGNATURE };
        }

        uint8_t* cmpData = nullptr;

        IHDR header{};
        Chunk chunk;

        std::vector<Chunk> idats{};
        size_t idatSize = 0;

        bool foundIHDR = false;

        size_t totalSize = 0;
        int32_t bytesPerPixel = 0;

        TextureFormat fmt = TextureFormat::Unknown;
        size_t paletteOff = 0;

        char* pixelData = nullptr;

        //Read the file chunk by chunk, pretty starightforward
        while (!stream.eof() && readChunk(stream, chunk)) {
            size_t curPos = stream.tellg();
            switch (chunk.chunkId) {
                case PNGChunk::CH_IHDR: {
                    if (foundIHDR) {
                        //We found multiple IHDR chunks!
                        //TODO: Add assertions/logging
                        continue;
                    }

                    //If we hit an IHDR chunk, read the chunk as is.
                    //Also check for invalid/unsupported color formats/bit depths
                    stream.seekg(chunk.dataStart, std::ios::beg);
                    stream.read(reinterpret_cast<char*>(&header), sizeof(IHDR));
                    stream.seekg(curPos, std::ios::beg);

                    Span<int32_t> hdr(&header, sizeof(IHDR));
                    hdr[0] = hdr.readBigEndian<int32_t>();
                    hdr[1] = hdr.readBigEndian<int32_t>(4);

                    if (!validateColorAndBitDepth(header.colorType, header.bitDepth, bytesPerPixel)) {
                        return { nullptr, {0, 0}, TextureFormat::Unknown, UNSUPPORTED_COLOR_FORMAT_OR_BIT_DEPTH };
                    }

                    paletteOff = header.colorType == PNGColorType::Indexed ? (256 * sizeof(JColor32)) : 0;
                    totalSize = size_t(header.width * bytesPerPixel + 1) * (header.height) + paletteOff;
                    fmt = header.getTextureFormat();

                    //This buffer is going to be returned, it will be larger than the actual
                    //pixel data requires, but it allows us to do the PNG decoding in-place
                    pixelData = reinterpret_cast<char*>(malloc(totalSize));

                    if (header.colorType == PNGColorType::Indexed) {
                        std::fill_n(reinterpret_cast<JColor32*>(pixelData), 256, JColor32::Black);
                    }
                    foundIHDR = true;
                    break;
                }

                case PNGChunk::CH_IDAT:
                    if (!foundIHDR) { goto end; }

                    //If we hit an IDAT chunk, add it's size to the total idatSize and
                    //and push it to the idats vector to be processed later
                    idats.push_back(chunk);
                    idatSize = std::max<size_t>(chunk.length, idatSize);
                    break;

                case PNGChunk::CH_PLTE: {
                    if (!foundIHDR) { goto end; }

                    //Read palette data and write it to the
                    //palette section of the pixel data
                    size_t len = chunk.length / 3;
                    len = len > 256 ? 256 : len;

                    JColor24 temp[256]{};
                    stream.seekg(chunk.dataStart, std::ios::beg);
                    stream.read(reinterpret_cast<char*>(temp), len * sizeof(JColor24));
                    stream.seekg(curPos, std::ios::beg);

                    //Use JColor32.set() to not change the alpha
                    JColor32* plt = reinterpret_cast<JColor32*>(pixelData);
                    for (size_t i = 0; i < len; i++) {
                        plt[i].set(temp[i]);
                    }
                    break;
                }
                case PNGChunk::CH_tRNS: {
                    if (!foundIHDR) { goto end; }

                    if (header.colorType == PNGColorType::Indexed) {
                        //Same as with the palette data except we just read
                        //one byte's worth of data instead of JColor24's
                        size_t len = chunk.length;
                        len = len > 256 ? 256 : len;

                        uint8_t temp[256]{};
                        stream.seekg(chunk.dataStart, std::ios::beg);
                        stream.read(reinterpret_cast<char*>(temp), len);
                        stream.seekg(curPos, std::ios::beg);

                        JColor32* plt = reinterpret_cast<JColor32*>(pixelData);
                        for (size_t i = 0; i < len; i++) {
                            plt[i].a = temp[i];
                        }
                    }
                    break;
                }
                case PNGChunk::CH_IEND:
                    //If we run into the IEND chunk, break out of the loop with a goto
                    goto end;
            }
        }
    end:

        if (!foundIHDR) {
            free(pixelData);
            return { nullptr, {0, 0}, TextureFormat::Unknown, NO_IHDR_CHUNK };
        }

        if (idats.size() < 1) {
            free(pixelData);
            return { nullptr, {0, 0}, TextureFormat::Unknown, NO_IDAT_CHUNKS };
        }

        stream.clear(std::ios::eofbit);

        size_t cur = 0;
        //Setup ZLIB decompression
        uint8_t* idatCmp = reinterpret_cast<uint8_t*>(alloca(idatSize));
        char* ptr = reinterpret_cast<char*>(idatCmp);

        //Go through all found IDAT chunks and append their data 
        //to the idatCmp buffer which is going to be used when 
        //decompressing the ZLIB data

        ZLib::ZLibContext context;
        int32_t dataOut;
        ZLib::inflateBegin(context, pixelData + paletteOff, totalSize - paletteOff);
        for (size_t i = 0; i < idats.size(); i++)
        {
            const Chunk& ch = idats[i];
            stream.seekg(ch.dataStart, std::ios::beg);
            
            size_t len = ch.length;
    
            stream.read(ptr, len);
            ZLib::inflateSegment(context, ptr, len, dataOut, i >= idats.size() - 1);
        }
        ZLib::inflateEnd(context, dataOut);
 
        //Calculate scan size in both PNG "space" and pixel data "space",
        //then allocate a double buffer for one scan on the stack (should be fine, I think)

        const int32_t scanSize       = header.width * bytesPerPixel;
        const int32_t scanSizePNG    = scanSize + bytesPerPixel;
     
        char* scanBuffer = reinterpret_cast<char*>(alloca(scanSizePNG));
        memset(scanBuffer, 0, scanSizePNG);

        char* curScan = pixelData + paletteOff;
        //Parse and de-filter the decompressed PNG data scan by scan
        for (size_t y = 0, scanA = paletteOff, scanB = paletteOff; y < header.height; y++, scanA += scanSize) {
            //Apply PNG filter reversing, starting at index 1 to ignore the filter type
            auto filter = PNGFilterType(*curScan++);
            char* pixData = pixelData + scanA;

            reverseFilter(filter, curScan, scanBuffer + bytesPerPixel, pixData, scanSize, bytesPerPixel);
            curScan += scanSize;

            //Copy the de-filtered scan to previous scan
            memcpy(scanBuffer + bytesPerPixel, pixData, scanSize);
        }

        //We're done here!

        size_t finalSize = (header.width * header.height) * bytesPerPixel + paletteOff;
        pixelData = reinterpret_cast<char*>(realloc(pixelData, finalSize));
        return { reinterpret_cast<uint8_t*>(pixelData), {uint16_t(header.width), uint16_t(header.height)}, fmt, DECODER_OK };
    }
}