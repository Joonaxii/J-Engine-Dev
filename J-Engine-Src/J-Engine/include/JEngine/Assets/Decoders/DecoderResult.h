#pragma once
#include <cstdint>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Assets/Graphics/Texture/TextureFormat.h>
#include <JEngine/Assets/AssetDataType.h>

namespace JEngine {
    static constexpr uint32_t FAILED_TO_ALLOCATE_BUFFER = 0x10000000U;
    static constexpr uint32_t IO_ERROR = 0x20000000U;
    static constexpr uint32_t DECODER_OK = 0x00;

    struct DecoderResult {
        uint8_t* data;
        JVector2<uint16_t> resolution;
        TextureFormat format;
        UI32Flags errFlags;

        void clear(const bool freeData) {
            if (data && freeData) {
                free(data);
            }

            data = nullptr;
            format = TextureFormat::Unknown;
            errFlags = 0;
            resolution = { 0, 0 };
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const DecoderResult& res) {
        os << "- Err Flags : " << "0x" << std::setfill('0') << std::setw(8) << std::hex << uint32_t(res.errFlags) << std::dec << std::endl;
        os << "- Resolution: " << res.resolution << std::endl;
        os << "- Format    : " << getTextureFormatName(res.format);
        return os;
    }

    inline std::string& appendDecodeErrorString(std::string& strOut, const UI32Flags flag, const char* breakChar = "\0") {
        if (breakChar != "\0") { strOut.append(breakChar); }

        switch (flag)
        {
            case FAILED_TO_ALLOCATE_BUFFER:
                strOut.append("Failed to allocate a buffer!");
                break;

            case IO_ERROR:
                strOut.append("File not found or couldn't be opened!");
                break;
        }
        return strOut;
    }

    template<AssetDataFormat dataFmt>
    std::string& getDecodeErrorString(std::string& strOut, const UI32Flags flags) {
        if (!flags) {
            strOut.append("Decode Ok!");
            return strOut;
        }
        return strOut;
    }


}