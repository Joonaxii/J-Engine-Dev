#pragma once
#include <map>
#include <functional>
#include <string>
#include <typeindex>
#include <JEngine/Helpers/TypeHelpers.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/Components/Component.h>
#include <JEngine/Collections/PoolAllocator.h>
#define ADD_TO_GO_CALL(name) Component*(*name)(GameObject*)

namespace JEngine {

    struct Comp {
        Type* type = nullptr;
        Component* (*addComponent)(GameObject*) = nullptr;
        void (*trimAllocPool)() = nullptr;
        void (*clearAllocPool)(bool full) = nullptr;
    };

    class ComponentFactory {
    public:
        template<typename T> static Comp* getComp();

        static std::vector<Comp*>& getComps();

        static Comp* getComponentByHash(const UUID16& hash) {
            auto& components = getComps();
            for (size_t i = 0; i < components.size(); i++) {
                if (components[i]->type->hash == hash) {
                    return components[i];
                }
            }
            return nullptr;
        }

        template<typename T>
        static inline Component* defaultAddComponent(GameObject* go) {
            auto comp = go->addComponent<T>();
            return static_cast<Component*>(comp);
        }

        template<typename T>
        static const bool hasComponent() {
            const std::type_index& tt = typeid(T);
            auto& str = TypeHelpers::getTypeName(tt);
            return hasComponent(str);
        }
        static const bool hasComponent(const UUID16& hash);
        static const bool hasComponent(const Component* component);
        static const bool hasComponent(const std::string& name);
        static const bool hasComponent(const std::string& name, UUID16& hashOut);

        template<typename T>
        static const bool hasComponent(std::pair<UUID16, std::string>& hash) {
            Type* type = JEngine::TypeHelpers::getType<T>();

            hash.first = type->hash;
            hash.second = type->name;
            return hasComponent(type->name);
        }
        static const bool hasComponent(const Component* component, std::pair<UUID16, std::string>& hash);

        static Component* addComponent(GameObject* go, const char* name);
        static Component* addComponent(GameObject* go, const char* name, size_t count);
        static Component* addComponent(GameObject* go, const std::string& name);
        static Component* addComponent(GameObject* go, const UUID16& hash);

        static void clearAllComponentPools(bool full);
        static void trimAllComponentPools();

        template<typename T>
        static void addComp(JEngine::Comp* comp, Type* type, ADD_TO_GO_CALL(addComponent)) {
            if (comp->type != nullptr) { return; }
            comp->type = type;
            comp->addComponent = addComponent;
            comp->trimAllocPool = trimPoolAllocator<T, ComponentInfo<T>::InitPool>;
            comp->clearAllocPool = clearPoolAllocator<T, ComponentInfo<T>::InitPool>;
            getComps().push_back(comp);
        }
    };
}
template<typename T>
struct ValidatedComp {
    static const JEngine::Comp* Value;
};

template<typename T>
inline const JEngine::Comp* ValidatedComp<T>::Value = nullptr;

#define DEFINE_COMPONENT(TYPE) \
template<> \
inline JEngine::Comp* JEngine::ComponentFactory::getComp<TYPE>() { \
  static JEngine::Comp comp; \
  JEngine::ComponentFactory::addComp<TYPE>(&comp, JEngine::TypeHelpers::getType<TYPE>(), JEngine::ComponentFactory::defaultAddComponent<TYPE>); \
  return &comp; \
}\

#define VALIDATE_COMPONENT(x) template<> \
inline const JEngine::Comp* ValidatedComp<x>::Value = JEngine::ComponentFactory::getComp<x>(); \

#ifndef JENGINE_AUTO_VALID_COMPS
#define JENGINE_AUTO_VALID_COMPS
#endif

#ifdef JENGINE_AUTO_VALID_COMPS

#define REGISTER_COMPONENT(x) \
DEFINE_TYPE(x); \
DEFINE_COMPONENT(x); \
VALIDATE_COMPONENT(x); \

#else

#define REGISTER_COMPONENT(x) \
DEFINE_TYPE(x); \
DEFINE_COMPONENT(x); \

#endif


