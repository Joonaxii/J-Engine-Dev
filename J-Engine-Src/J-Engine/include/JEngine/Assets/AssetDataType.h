#pragma once
#include <cstdint>

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

		JTEX,
		JRTX,
		JATL,

		JTLE,

		JSCN,
		JPFB,

		TEXT,
		BINARY,

		WAV,
		OGG,

		//Supported Image Formats
		PNG,
		BMP,

		//Unsupported Image Formats
		JPEG,
		GIF,


		//Used as a base for custom formats
		MAX, 
	};

	enum class AssetDataType : uint8_t
	{
		Binary,
		JSON,
	};
}