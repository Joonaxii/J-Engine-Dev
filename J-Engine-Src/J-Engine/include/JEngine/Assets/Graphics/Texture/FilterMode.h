#pragma once
#include <cstdint>

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
}