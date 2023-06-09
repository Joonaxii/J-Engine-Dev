#include <JEngine/Helpers/TypeHelpers.h>

namespace JEngine {
	std::unordered_map<UUID8, const char*, std::hash<UUID8>> TypeHelpers::HashLUT;

	std::vector<TypeData>& TypeHelpers::getTypes() {
		static std::vector<TypeData> _types;
		return _types;
	}

	int32_t TypeHelpers::indexOfHash(const UUID8& hash) {
		const auto& types = getTypes();
		for (int32_t i = 0; i < types.size(); i++) {
			if (types[i].hashData.hash == hash) { return i; }
		}
		return -1;
	}

	void TypeHelpers::addType(const HashPair& data, const int32_t size) {
		if (size > -1) { return; }
		auto& types = getTypes();
		types.push_back(TypeData(data, size));
	}
}