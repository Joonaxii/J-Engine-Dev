#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine  
{
	enum class TextureFormat : uint8_t  {
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

#pragma region Serialization
	 template<>
	 inline bool Serializable<TextureFormat>::deserializeJson(TextureFormat& itemRef, json& jsonF, const TextureFormat& defaultVal) {
		 itemRef = jsonF.is_number() ? TextureFormat(jsonF.get<uint8_t>()) : defaultVal;
		 return true;
	 }

	 template<>
	 inline bool Serializable<TextureFormat>::serializeJson(const TextureFormat& itemRef, json& jsonF) {
		 jsonF.update(uint8_t(itemRef));
		 return true;
	 }
#pragma endregion
}