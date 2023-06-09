#include <JEngine/Assets/Graphics/Texture/Texture2D.h>
#include <JEngine/IO/Compression/ZLibStream.h>
#include <JEngine/Utility/ConstSpan.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/IO/Helpers/IOHelpers.h>
#include <JEngine/Assets/AssetDataType.h>
#include <fstream>

namespace JEngine {
    Texture2D::Texture2D() : Texture() { }

    bool Texture2D::serializeBinary(std::ostream& stream) const {
        stream.write(toHeaderStr(AssetDataFormat::JTEX), 4);
 
        Serialization::serialize(_texFlags, stream);
        Serialization::serialize(_size, stream);

        Serialization::serialize(_format, stream);
        Serialization::serialize(_filter, stream);

        const int32_t bitsPP = getBitsPerPixel(_format);
        const int32_t bytesPP = bitsPP >> 3;

        const size_t offset = _format == TextureFormat::Indexed ? 256 * 4 : 0;
        const size_t dataSize = (_size.x * _size.y * bytesPP) + offset;
        uint16_t compression = _texFlags.select(VALUE_COMPRESSION);

        auto pixels = getTextureData();

        if (compression) {
            //Texture data is compressed

            int64_t pos = stream.tellp();
            uint32_t size = 0;
            stream.write((char*)&size, 4);

            char* ptr = nullptr;
            ptr = reinterpret_cast<char*>(malloc(dataSize));
            if (ptr) {
                memset(ptr, 0, dataSize);
            }

            membuf buf(ptr, dataSize);
            std::iostream oStrm(&buf);

            oStrm.write(reinterpret_cast<char*>(pixels), dataSize);

            oStrm.flush();
            oStrm.seekp(0, std::ios::beg);
            oStrm.seekg(0, std::ios::beg);

            auto res = ZLib::deflateData(oStrm, stream, compression == 10 ? -1 : compression);

            if (res != Z_OK) {
                std::cout << "ZLib compression failed! " << std::endl;
                std::cout << ZLib::zerr(res) << std::endl;
            }

            stream.flush();

            int64_t cPos = stream.tellp();
            size = uint32_t(cPos - (pos + 4));

            stream.seekp(pos, std::ios::beg);
            stream.write((char*)&size, 4);
            stream.seekp(cPos, std::ios::beg);

            free(ptr);
        }
        else {
            //Texture data is uncompressed
            stream.write(reinterpret_cast<char*>(pixels), dataSize);
        }

        if (!_pixelData && _texFlags & FLAG_KEEP_DATA) {
            free(pixels);
        }
        return false;
    }

    bool Texture2D::deserializeBinary(std::istream& stream, const size_t size) {
        char hdr[5]{ 0 };
        stream.read(hdr, 4);

        if (strcmp(hdr, toHeaderStr(AssetDataFormat::JTEX)) == 0) {
            Serialization::deserialize(_texFlags, stream);
            Serialization::deserialize(_size, stream);

            Serialization::deserialize(_format, stream);
            Serialization::deserialize(_filter, stream);

            const uint16_t compression = _texFlags.select(VALUE_COMPRESSION);

            const size_t offset = _format == TextureFormat::Indexed ? 256 * 4 : 0;
            const size_t dataSize = _size.x * _size.y * (getBitsPerPixel(_format) >> 3) + offset;
            uint8_t* pixels = reinterpret_cast<uint8_t*>(malloc(dataSize));

            if (compression) {
                //Data is compressed
                size_t compressedSize = 0;
                stream.read(reinterpret_cast<char*>(&compressedSize), 4);

                char* ptrComp = nullptr;
                ptrComp = reinterpret_cast<char*>(malloc(compressedSize));
                if (ptrComp) {
                    memset(ptrComp, 0, compressedSize);
                }

                membuf bufComp(ptrComp, compressedSize);
                stream.readsome(ptrComp, compressedSize);

                membuf buf(reinterpret_cast<char*>(pixels), dataSize);
                std::istream cStrm(&bufComp);
                std::iostream oStrm(&buf);
                oStrm.seekg(0, std::ios::beg);
                oStrm.clear();

                auto ret = ZLib::inflateData(cStrm, oStrm);
                if (ret != Z_OK) {
                    std::cout << ZLib::zerr(ret) << std::endl;
                }
                oStrm.flush();
                free(ptrComp);
            }
            else {
                //Data not compressed
                stream.readsome(reinterpret_cast<char*>(pixels), dataSize);
            }

            create(_size.x, _size.y, _format, _filter, pixels, _texFlags & FLAG_KEEP_DATA);
        }
        return true;
    }

    bool Texture2D::serializeJson(json& jsonF) const {
        return false;
    }

    bool Texture2D::deserializeJson(json& jsonF) {
        return false;
    }

}