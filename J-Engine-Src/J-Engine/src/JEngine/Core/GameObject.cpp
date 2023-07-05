#include <JEngine/Core/GameObject.h>
#include <JEngine/Components/CTransform.h>
#include <JEngine/Components/ComponentFactory.h>

namespace JEngine {
    uint8_t GameObject::indexOfComponent(const Component* comp) const {
        if (comp) {
            for (uint8_t i = 0; i < _compInfo.count; i++) {
                if (_components[i] == comp) { return i; }
            }
        }
        return MAX_COMPONENTS;
    }
    uint8_t GameObject::indexOfComponent(const UUID8& uuid) const {
        if (uuid != UUID8::Empty) {
            for (uint8_t i = 0; i < _compInfo.count; i++) {
                if (_components[i] && _components[i]->getUUID() == uuid) { return i; }
            }
        }
        return MAX_COMPONENTS;
    }
    GameObject* GameObject::createObject(const char* name, const UUID16* components, size_t compCount, uint16_t flags, UUID8 uuid) {
        GameObject* go = getGameObjectAllocator().allocate();
        if (!go) {
            JENGINE_CORE_ERROR("[J-Engine GameObject] Error: Couldn't allocate game object!");
            return nullptr;
        }

        go->init(name, flags);
        if (uuid) { 
            go->setUUID(uuid);
            UUIDFactory::addUUID<GameObject>(uuid); 
        }
        else {
            go->setUUID(UUIDFactory::generateUUID<GameObject>(uuid) ? uuid : UUID8::Empty);
        }

        if (components) {
            for (size_t i = 0; i < compCount; i++) {
                ComponentFactory::addComponent(go, components[i]);
            }
        }
        return go;
    }


    GameObject* GameObject::createObject(const char* name, uint16_t flags, UUID8 uuid) {
        static constexpr size_t size = sizeof(GameObject);
        return createObject(name, nullptr, 0, flags, uuid);
    }

    GameObject* GameObject::createObject(const std::string& name, const UUID16* components, size_t compCount, uint16_t flags, UUID8 uuid) {
        static constexpr size_t size = sizeof(GameObject);
        return createObject(name.c_str(), components, compCount, flags, uuid);
    }

    GameObject* GameObject::createObject(const std::string& name, uint16_t flags, UUID8 uuid) {
        static constexpr size_t size = sizeof(GameObject);
        return createObject(name.c_str(), nullptr, 0, flags, uuid);
    }

    GameObject* GameObject::destroyObject(GameObject* go) {
        if (!go) { return nullptr; }
        if (!getGameObjectAllocator().deallocate(go)) {
            JENGINE_CORE_ERROR("[J-Engine GameObject] Error: Couldn't deallocate game object!");
            return go;
        }
        return nullptr;
    }

    void GameObject::deserializeCompRefBinary(const Stream& stream, UUID8& go, UUID8& comp) {
        Serialization::deserialize(go, stream);
        Serialization::deserialize(comp, stream);
    }

    void GameObject::serializeCompRefBinary(const Stream& stream, const GameObject* go, const Component* comp)  {
        if (go) {
            Serialization::serialize(go->getUUID(), stream);
            Serialization::serialize(comp ? comp->getUUID() : UUID8::Empty, stream);
        }
        else {
            Serialization::serialize(UUID8::Empty, stream);
            Serialization::serialize(UUID8::Empty, stream);
        }
    }

    GameObject::GameObject() : INamedObject() { }

    GameObject::~GameObject() {
        if (getFlags() & FLAG_IS_DESTROYED) { return; }
        for (size_t i = 0; i < _compInfo.count; i++) {
            _components[i]->destroy();
        }

        getFlags() |= FLAG_IS_DESTROYED;
        _compInfo.count = 0;
        _compInfo.trIndex = NULL_TRANSFORM;
        UUIDFactory::removeUUID<GameObject>(getUUID());
    }

    CTransform* GameObject::getTransform() {
        if (_compInfo.trIndex == NULL_TRANSFORM) { return nullptr; }
        return dynamic_cast<CTransform*>(_components[_compInfo.trIndex]);
    }

    const CTransform* GameObject::getTransform() const {
        if (_compInfo.trIndex == NULL_TRANSFORM) { return nullptr; }
        return dynamic_cast<const CTransform*>(_components[_compInfo.trIndex]);
    }

    void GameObject::init(const char* name, uint16_t flags) {
        initObj(name, UUID8::Empty, flags);
    }

    void GameObject::start() {
        for (size_t i = 0; i < _compInfo.count; i++) {
            _components[i]->start();
        }
    }

    void GameObject::update(const JTime& time) {
        float delta = time.getDeltaTime<float>(_timeSpace);
        float eTime  = time.getTime<float>(_timeSpace);
        for (size_t i = 0; i < _compInfo.count; i++) {
            _components[i]->update(eTime, delta);
        }
    }

    bool GameObject::addComponent(Component* comp, bool autoStart) {
        if (_compInfo.count >= MAX_COMPONENTS) {
            return false;
        }

        _components[_compInfo.count++] = comp;
        if (autoStart) {
            comp->start();
        }
        return true;
    }

    bool GameObject::removeComponent(const UUID8& uuid, bool destroy) {
        auto find = indexOfComponent(uuid);
        if (find == MAX_COMPONENTS) {
            return false;
        }

        Component* comp = _components[find];
        if (find < _compInfo.count - 1) {
            memcpy(_components + find, _components + find + 1, _compInfo.count - find);
        }
        _compInfo.count--;
        if (destroy) {
            comp->destroy();
        }
        else {
            comp->_object = nullptr;
        }
        return true;
    }

    bool GameObject::removeComponent(Component* comp, bool destroy) {
        auto find = indexOfComponent(comp);
        if (find == MAX_COMPONENTS) {
            return false;
        }

        if (find < _compInfo.count - 1) {
            memcpy(_components + find, _components + find + 1, _compInfo.count - find);
        }
        _compInfo.count--;
        if (destroy) {
            comp->destroy();
        }
        else {
            comp->_object = nullptr;
        }
        return true;
    }
}