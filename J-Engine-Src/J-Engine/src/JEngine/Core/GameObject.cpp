#include <JEngine/Core/GameObject.h>
#include <JEngine/Components/CTransform.h>

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
                if (_components[i] && _components[i]->_uuid == uuid) { return i; }
            }
        }
        return MAX_COMPONENTS;
    }
    GameObject* GameObject::createObject(const char* name, uint16_t flags) {
        return new GameObject(name, flags);
    }

    GameObject* GameObject::createObject(const std::string& name, uint16_t flags) {
        return new GameObject(name, flags);
    }

    void GameObject::destroyObject(GameObject*& go) {
        destroyObject(const_cast<const GameObject*>(go));
        go = nullptr;
    }

    void GameObject::destroyObject(const GameObject* go) {
        //TODO: Do stuff like removing from scene/unregistering
        delete go;
    }

    GameObject::GameObject(const std::string& name, uint16_t flags) : _name(name), _flags(flags) { }
    GameObject::GameObject(const char* name, uint16_t flags) : _name(name), _flags(flags) { }
    GameObject::~GameObject() {
        for (auto& comp : _components) {
            comp->destroy();
        }
    }

    CTransform* GameObject::getTransform() {
        if (_compInfo.trIndex == NULL_TRANSFORM) { return nullptr; }
        return dynamic_cast<CTransform*>(_components[_compInfo.trIndex]);
    }

    const CTransform* GameObject::getTransform() const {
        if (_compInfo.trIndex == NULL_TRANSFORM) { return nullptr; }
        return dynamic_cast<const CTransform*>(_components[_compInfo.trIndex]);
    }

    void GameObject::operator delete(void* ptr) noexcept {
        free(ptr);
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
            delete comp;
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
            delete comp;
        }
        else {
            comp->_object = nullptr;
        }
        return true;
    }
}