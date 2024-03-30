#include <JEngine/Components/ComponentFactory.h>
#include <iostream>

namespace JEngine {

	std::vector<Comp const*>& ComponentFactory::getComps() {
		static std::vector<Comp const*> _components{};
		return _components;
	}

	bool ComponentFactory::hasComponent(const std::string_view& name) {
		return hasComponent(Types::calculateNameHash(name));
	}

	bool ComponentFactory::hasComponent(const std::string_view& name, uint32_t& hashOut) {
		hashOut = Types::calculateNameHash(name);
		return hasComponent(hashOut);
	}

	bool ComponentFactory::hasComponent(uint32_t hash) {
		return getComponentByHash(hash);
	}

	CompRef ComponentFactory::addComponent(GORef go,  std::string_view name) {
		return addComponent(go, Types::calculateNameHash(name));
	}

	CompRef ComponentFactory::addComponent(GORef go, uint32_t hash) {
		auto comp = getComponentByHash(hash);
		return comp ? comp->addComponent(go) : CompRef(nullptr);
	}

	void ComponentFactory::clearAllComponentPools(bool full) {
		for (auto& comp : getComps()) {
			comp->clearAllocPool(full);
		}
	}

	void ComponentFactory::trimAllComponentPools() {
		for (auto& comp : getComps()) {
			comp->trimAllocPool();
		}
	}
}