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

	std::string& getBmpErrorString(std::string& strOut, const UI32Flags flags);

	DecoderResult decodeBmp(const std::string& filepath, const UI32Flags flags = BMP_FLG_FLIP_Y, int64_t dataSize = -1);
	DecoderResult decodeBmp(std::istream& stream, const UI32Flags flags = BMP_FLG_FLIP_Y, int64_t dataSize = -1);
	
}