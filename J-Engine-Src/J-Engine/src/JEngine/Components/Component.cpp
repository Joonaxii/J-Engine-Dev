#include <JEngine/Components/Component.h>
#include <JEngine/Core/GameObject.h>

namespace JEngine {
    Component::Component(GameObject* obj, uint16_t flags) : _object(obj), _flags(flags), _uuid() { }

    Component::~Component() {
        if (_object) {
            auto obj = _object;
            destroy();
            obj->removeComponent(this, false);
        }
    }

    void Component::start() {
        onStart();
    }

    void Component::update(float delta) {
        onUpdate(delta);
    }

    void Component::destroy() {
        if (_flags & FLAG_IS_DESTROYED) { return; }

        onDestroy();
        _object = nullptr;
        _flags |= FLAG_IS_DESTROYED;

        UUIDFactory::removeUUID<Component>(_uuid);
        _uuid = UUID8::Empty;
    }
}