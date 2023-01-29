#pragma once
#include <cstdint>

namespace JEngine  
{
	enum class TextureFormat : uint8_t 
	{
		Indexed,
		R8,
		RGB24,
		RGBA32,

		Unknown = 0xFF,
	};

	constexpr int32_t getBitsPerPixel(const TextureFormat format) {
		switch (format) {
			default: return 0;

			case TextureFormat::Indexed:
			case TextureFormat::R8: 
				return 8;

			case TextureFormat::RGB24: return 24;
			case TextureFormat::RGBA32: return 32;
		}
	}

     constexpr char* getTextureFormatName(const TextureFormat format) {
		switch (format) {
			default:						return "Unknown";

			case TextureFormat::Indexed:	return "Indexed";
			case TextureFormat::R8:			return "R8";
				
			case TextureFormat::RGB24:		return "RGB24";
			case TextureFormat::RGBA32:		return "RGBA32";
		}
	}
}