#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine 
{
	enum class AssetDataFormat : uint16_t
	{
		Unknown,
		
		//Engine Formats
		JPAK,

		JPRF,
		
		JMAT,
		JSHD,
		JGLI,

		JTEX,
		JRTX,
		JATL,

		JINP,

		JSCN,
		JPFB,

		JTXT,
		JBIN,

		//External formats
		WAV,
		OGG,

		//Supported Image Formats
		PNG,
		BMP,

		//Unsupported Image Formats
		JPEG,
		GIF87,
		GIF89,


		//Used as a base for custom formats
		MAX, 
	};

	enum class AssetDataType : uint8_t
	{
		Binary,
		JSON,
	};

	static inline constexpr char* assetDataFormatToHeader(const AssetDataFormat format) {
		switch (format)
		{
			//Engine formats
			case AssetDataFormat::JPAK:			return "JPAK";

			case AssetDataFormat::JPRF:			return "JPRF";

			case AssetDataFormat::JMAT:			return "JMAT";
			case AssetDataFormat::JSHD:			return "JSHD";
			case AssetDataFormat::JGLI:			return "JGLI";

			case AssetDataFormat::JTEX:			return "JTEX";
			case AssetDataFormat::JRTX:			return "JRTX";
			case AssetDataFormat::JATL:			return "JATL";

			case AssetDataFormat::JINP:			return "JINP";

			case AssetDataFormat::JSCN:			return "JSCN";
			case AssetDataFormat::JPFB:			return "JPFB";

			case AssetDataFormat::JTXT:			return "JTXT";
			case AssetDataFormat::JBIN:			return "JBIN";

			//External formats
			case AssetDataFormat::BMP:			return "BM";
			case AssetDataFormat::PNG:			return "‰PNG";
			case AssetDataFormat::WAV:			return "RIFF¤¤¤¤WAVE";
			case AssetDataFormat::OGG:			return "OggS";
			case AssetDataFormat::GIF87:		return "GIF87a";
			case AssetDataFormat::GIF89:		return "GIF89a";

			default: return "";
		}
	}

#pragma region Serialization
	template<>
	inline const bool Serializable<AssetDataFormat>::deserializeJson(AssetDataFormat& itemRef, json& jsonF, const AssetDataFormat& defaultVal) {
		itemRef = jsonF.is_number() ? AssetDataFormat(jsonF.get<uint16_t>()) : defaultVal;
		return true;
	}

	template<>
	inline const bool Serializable<AssetDataFormat>::serializeJson(const AssetDataFormat& itemRef, json& jsonF) {
		jsonF = uint16_t(itemRef);
		return true;
	}


	template<>
	inline const bool Serializable<AssetDataType>::deserializeJson(AssetDataType& itemRef, json& jsonF, const AssetDataType& defaultVal) {
		itemRef = jsonF.is_number() ? AssetDataType(jsonF.get<uint8_t>()) : defaultVal;
		return true;
	}

	template<>
	inline const bool Serializable<AssetDataType>::serializeJson(const AssetDataType& itemRef, json& jsonF) {
		jsonF = uint8_t(itemRef);
		return true;
	}
#pragma endregion
}