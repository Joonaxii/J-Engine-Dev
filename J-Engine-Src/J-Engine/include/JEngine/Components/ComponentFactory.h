#pragma once
#include <map>
#include <functional>
#include <JEngine/Core/Ref.h>
#include <JEngine/Collections/PoolAllocator.h>
#include <JEngine/Helpers/TypeHelpers.h>
#define ADD_TO_GO_CALL(name) JEngine::CompRef(*name)(JEngine::GORef)

namespace JEngine {
    using AddComponent = CompRef(*)(GORef);
    using TrimAllocPool = void (*)();
    using ClearAllocPool = void (*)(bool full);

    enum ComponentFlags : uint8_t {
        COMP_IS_TRANSFORM = 0x1,
    };

    template<typename T>
    struct ComponentInfo {
        static inline constexpr size_t InitPool{ 0x00 };
        static inline constexpr ComponentFlags Flags{ 0x00 };
        static inline constexpr const char* Name{ "Component" };
    };

    struct Comp {
        Type const* type = nullptr;
        AddComponent addComponent = nullptr;
        TrimAllocPool trimAllocPool = nullptr;
        ClearAllocPool clearAllocPool = nullptr;

        constexpr Comp() : type{ nullptr }, addComponent{ nullptr }, trimAllocPool{ nullptr }, clearAllocPool{ nullptr }{};
        constexpr Comp(
            Type const& type,
            AddComponent addComponent,
            TrimAllocPool trimAllocPool,
            ClearAllocPool clearAllocPool
        ) : type{ &type }, addComponent{ addComponent }, trimAllocPool{ trimAllocPool }, clearAllocPool{ clearAllocPool }{};
    };

    namespace detail {
        template<typename T>
        inline TCompRef<T> defaultAddComponent(GORef go) {
            return go->addComponent<T>();
        }
    }

    class ComponentFactory {
    public:
        static std::vector<Comp const*>& getComps();

        template<typename T>
        static Comp const& getComp();

        static Comp const* getComponentByHash(uint32_t hash) {
            auto& components = getComps();
            for (size_t i = 0; i < components.size(); i++) {
                if (components[i]->type->hash == hash) {
                    return components[i];
                }
            }
            return nullptr;
        }

        template<typename T>
        static bool hasComponent() {
            auto& str = TypeHelpers::getTypeName<T>();
            return TypeHelpers::getType<T>().hash;
        }
        static bool hasComponent(uint32_t hash);
        static bool hasComponent(const std::string_view& name);
        static bool hasComponent(const std::string_view& name, uint32_t& hashOut);

        template<typename T>
        static bool hasComponent(std::pair<uint32_t, std::string>& hash) {
            Type* type = JEngine::TypeHelpers::getType<T>();

            hash.first = type->hash;
            hash.second = type->name;
            return hasComponent(type->name);
        }

        static CompRef addComponent(GORef go, std::string_view name);
        static CompRef addComponent(GORef go, uint32_t hash);

        static void clearAllComponentPools(bool full);
        static void trimAllComponentPools();
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
inline JEngine::Comp const& JEngine::ComponentFactory::getComp<TYPE>() { \
    static JEngine::Comp comp{}; \
    if (comp.type == nullptr) { \
        comp.type = &JEngine::TypeHelpers::getType<TYPE>(); \
        comp.addComponent = JEngine::AddComponent(JEngine::detail::defaultAddComponent<TYPE>); \
        comp.trimAllocPool = JEngine::TrimAllocPool(JEngine::trimPoolAllocator<TYPE, JEngine::ComponentInfo<TYPE>::InitPool>); \
        comp.clearAllocPool = JEngine::ClearAllocPool(JEngine::trimPoolAllocator<TYPE, JEngine::ComponentInfo<TYPE>::InitPool>); \
        JEngine::ComponentFactory::getComps().push_back(&comp); \
    } \
    return comp; \
} \

#define VALIDATE_COMPONENT(x) template<> \
inline const JEngine::Comp* ValidatedComp<x>::Value = &JEngine::ComponentFactory::getComp<x>(); \

#define REGISTER_COMPONENT(x) \
DEFINE_TYPE(x); \
DEFINE_COMPONENT(x); \
VALIDATE_COMPONENT(x); \

