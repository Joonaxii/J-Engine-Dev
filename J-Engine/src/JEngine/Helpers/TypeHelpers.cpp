#include <JEngine/Helpers/TypeHelpers.h>

namespace JEngine {
	std::vector<Type const*>& TypeHelpers::getTypes() {
		static std::vector<Type const*> _types{};
		return _types;
	}
}