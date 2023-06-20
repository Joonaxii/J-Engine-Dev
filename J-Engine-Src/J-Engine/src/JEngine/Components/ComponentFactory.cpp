#include <JEngine/Components/ComponentFactory.h>
#include <iostream>

namespace JEngine {

	std::vector<Comp*>& ComponentFactory::getComps() {
		static std::vector<Comp*> _components;
		return _components;
	}

	const bool ComponentFactory::hasComponent(const std::string& name) {
		UUID16 hash;
		hash.computeHash(name.c_str(), name.length(), TYPE_UUID_BLOCK_SIZE);
		return hasComponent(hash);;
	}

	const bool ComponentFactory::hasComponent(const std::string& name, UUID16& hashOut) {
		hashOut.computeHash(name.c_str(), name.length(), TYPE_UUID_BLOCK_SIZE);
		return hasComponent(hashOut);;
	}

	const bool ComponentFactory::hasComponent(const UUID16& hash) {
		return getComponentByHash(hash);
	}

	const bool ComponentFactory::hasComponent(const Component* component) {
		if (component == nullptr) { return false; }
		auto typeHash = TypeHelpers::getTypeHash(*component);
		auto type = TypeHelpers::getTypeByHash(typeHash);
		return type;
	}

	const bool ComponentFactory::hasComponent(const Component* component, std::pair<UUID16, std::string>& hash) {
		if (component == nullptr) { return false; }
		const std::type_index& tt = typeid(*component);
		auto& str = TypeHelpers::getTypeName(tt);
		hash.first.computeHash(str.c_str(), str.length(), TYPE_UUID_BLOCK_SIZE);

		hash.second = str;
		return hasComponent(hash.first);
	}

	Component* ComponentFactory::addComponent(GameObject* go, const char* name) {
		UUID16 hash;
		hash.computeHash(name, strlen(name), TYPE_UUID_BLOCK_SIZE);
		return addComponent(go, hash);
	}

	Component* ComponentFactory::addComponent(GameObject* go, const char* name, size_t count) {
		UUID16 hash;
		hash.computeHash(name, count, TYPE_UUID_BLOCK_SIZE);
		return addComponent(go, hash);
	}

	Component* ComponentFactory::addComponent(GameObject* go, const std::string& name) {
		UUID16 hash;
		hash.computeHash(name.data(), name.length(), TYPE_UUID_BLOCK_SIZE);
		return addComponent(go, hash);
	}

	Component* ComponentFactory::addComponent(GameObject* go, const UUID16& hash) {
		auto comp = getComponentByHash(hash);
		return comp ? comp->addComponent(go) : nullptr;
	}

}