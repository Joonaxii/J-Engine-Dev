#include <JEngine/Core/GameObject.h>
#include <JEngine/Components/CTransform.h>
#include <JEngine/Components/ComponentFactory.h>

namespace JEngine {
    GameObject::GameObject() : INamedObject() { }

    GameObject::~GameObject() {
        if (getFlags() & FLAG_IS_DESTROYED) { return; }
        for (uint32_t i = 0, j = 0; i < MAX_COMPONENTS && j < _compInfo.getCount(); i++) {
            Component** comp = _components.getAt(i);
            if (comp) {
                (*comp)->destroy();
                j++;
            }
        }
        _components.clear(false);

        getFlags() |= FLAG_IS_DESTROYED;
        _compInfo.setCount(0);
        _compInfo.setTrIndex(NULL_TRANSFORM);
    }

    //void GameObject::getAllComponents(std::vector<Component*>& components) const {
    //    for (uint32_t i = 0; i < MAX_COMPONENTS; i++) {
    //        auto comp = _components.getAt(i);
    //        if (comp) {
    //            components.emplace_back(*comp);
    //        }
    //    }
    //}

    TCompRef<CTransform> GameObject::getTransform() const {
        uint32_t trIndex = _compInfo.getTrIndex();
        if (trIndex == NULL_TRANSFORM) { return TCompRef<CTransform>(nullptr); }
        return TCompRef<CTransform>(getUUID(), trIndex);
    }

    void GameObject::init(std::string_view name, uint16_t flags) {
        initObj(name, UINT32_MAX, flags);
    }

    void GameObject::start() {
        for (uint32_t i = 0, j = 0; i < MAX_COMPONENTS; i++) {
            Component** comp = _components.getAt(i);
            if (comp && *comp) {
                (*comp)->start();
            }
        }
    }

    void GameObject::update(const JTime& time) {
        float delta = time.getDeltaTime<float>(_timeSpace);
        float eTime  = time.getTime<float>(_timeSpace);

        for (uint32_t i = 0, j = 0; i < MAX_COMPONENTS; i++) {
            Component** comp = _components.getAt(i);
            if (comp && *comp) {
                (*comp)->update(eTime, delta);
            }
        }
    }

    CompRef GameObject::addComponent(Component* comp, uint16_t flags, bool autoStart) {
        if (_compInfo.getCount() >= MAX_COMPONENTS) {
            JE_CORE_WARN("[GameObject] Warning: Max number of components ({0}) for GameObject '{1}' [0x{2:X}] has been reached!!", MAX_COMPONENTS, getName(), getUUID());
            return CompRef();
        }

        CompRef uuid = CompRef(getUUID(), _components.setNext(comp));
        comp->init(uuid, flags);
        _compInfo++;
        if (autoStart) {
            comp->start();
        }
        return uuid;
    }

    bool GameObject::removeComponent(uint32_t uuid, bool destroy) {
        Component** compPtr = _components.getAt(uuid);
        if (!compPtr) {
            return false;
        }

        Component* comp = *compPtr;
        comp->setUUID(UINT32_MAX);
        _components.markFree(uuid);
        _compInfo--;
        if (uuid == _compInfo.getTrIndex()) {
            _compInfo.setTrIndex(NULL_TRANSFORM);
        }
        if (destroy) {
            comp->destroy();
        }
        return true;
    }

    bool GameObject::removeComponent(Component* comp, bool destroy) {
        CompRef compRef(comp);
        if (compRef.isForObject(getUUID())) {
            return false;
        }

        uint32_t uuid = comp->getUUID();
        comp->setUUID(UINT32_MAX);
        _components.markFree(compRef.getIndex());
        _compInfo--;
        if (compRef.getIndex() == _compInfo.getTrIndex()) {
            _compInfo.setTrIndex(NULL_TRANSFORM);
        }
        if (destroy) {
            comp->destroy();
        }
        return true;
    }
}