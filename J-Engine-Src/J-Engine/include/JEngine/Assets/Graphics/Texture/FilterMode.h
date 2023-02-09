#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    enum class FilterMode : uint8_t {
        Nearest,
        Linear
    };

	constexpr char* getFilterModeName(const FilterMode format) {
		switch (format) {
			default:					return "Unknown";
			case FilterMode::Nearest:	return "Nearest";
			case FilterMode::Linear:	return "Linear";
		}
	}

#pragma region Serialization
	template<>
	inline bool Serializable<FilterMode>::deserializeJson(FilterMode& itemRef, json& jsonF, const FilterMode& defaultVal) {
		itemRef = jsonF.is_number() ? FilterMode(jsonF.get<uint8_t>()) : defaultVal;
		return true;
	}

	template<>
	inline bool Serializable<FilterMode>::serializeJson(const FilterMode& itemRef, json& jsonF) {
		jsonF = uint8_t(itemRef);
		return true;
	}
#pragma endregion
}