#include <JEngine/Helpers/TypeHelpers.h>

namespace JEngine {
	std::vector<Type*>& TypeHelpers::getTypes() {
		static std::vector<Type*> _types;
		return _types;
	}

	void TypeHelpers::addType(Type& type, const size_t size, const std::pair<const FAH16, const std::string>& data) {
		if (type.size > -1) { return; }
		type.size = size;
		type.hash = data.first;
		type.name = data.second;
		getTypes().push_back(&type);
	}

}