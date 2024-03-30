#include <JEngine/Core/ComponentRef.h>
#include <JEngine/Core/GameObject.h>

namespace JEngine {
    ComponentRef::ComponentRef(const Component* comp) :
        object(comp ? comp->getObject() : nullptr),
        uuid(comp ? comp->getUUID() : UINT32_MAX) {}

    Component* ComponentRef::get() const {
        return nullptr;
    }
}