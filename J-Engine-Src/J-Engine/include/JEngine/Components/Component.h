#pragma once
#include <cstdint>
#include <JEngine/Collections/PoolAllocator.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Core/IObject.h>
#include <JEngine/Core/Ref.h>

namespace JEngine {
    enum ComponentFlags : uint8_t {
        COMP_IS_TRANSFORM = 0x1,
    };

    template<typename T>
    struct ComponentInfo {
        static inline constexpr size_t InitPool{ 0x00 };
        static inline constexpr ComponentFlags Flags{ 0x00 };
        static inline constexpr std::string_view Name{ "Component" };
    };

    class GameObject;
    class Component : public IObject {
    public:
        Component();
        virtual ~Component();
  
        GORef getObject() const { return GORef(getUUID()); }

        template<typename T, size_t init = ComponentInfo<T>::InitPool>
        static PoolAllocator<T, init>& getComponentAllocator() {
            return PoolAllocator<T, init>::getGlobal();
        }

        CompRef getRef() const { return CompRef(getUUID()); }

        NO_FIELDS;

    protected:
        friend class ComponentFactory;
        friend class GameObject;
        friend struct SerializedItem;

        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component&) = delete;

        void* operator new(size_t size) = delete;
        void operator delete(void* ptr) = delete;

        virtual void onInit() {};
        virtual void onSetup() {};
        virtual void onStart() {};
        virtual void onUpdate(float time, float delta) {};
        virtual void onDestroy() {};

        virtual void doDelete() = 0;
        virtual uint32_t getTypeHash() const = 0;

    private:
        friend class GameObject;
   
        void init(CompRef compRef, uint16_t flags);

        void start();
        void update(float time, float delta);
        void destroy();
    };
}

#define JE_COMPONENT(TYPE)\
protected:\
virtual uint32_t getTypeHash() const override;\
virtual void doDelete() override {\
    ::JEngine::Component::getComponentAllocator<TYPE>().deallocate(this); \
}\
public: \
::JEngine::TCompRef<TYPE> getRef_T() const {\
    return ::JEngine::TCompRef<TYPE>(getUUID()); \
}

#define JE_COMPONENT_DEF(TYPE)\
uint32_t TYPE::getTypeHash() const {\
    return ::JEngine::Types::getTypeHash<TYPE>(); \
}

#define REGISTER_COMPONENT(TYPE) \
DEFINE_TYPE(TYPE); \
DEFINE_COMPONENT(TYPE); \
VALIDATE_COMPONENT(TYPE)\
JE_COMPONENT_DEF(TYPE)