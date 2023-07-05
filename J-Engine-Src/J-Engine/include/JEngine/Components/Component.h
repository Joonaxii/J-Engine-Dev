#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Cryptography/UUIDFactory.h>
#include <JEngine/Core/IObject.h>
#include <JEngine/Collections/PoolAllocator.h>

namespace JEngine {
    enum ComponentFlags: uint8_t {
        COMP_IS_TRANSFORM = 0x1,
    };

    template<typename T>
    struct ComponentInfo {
        static inline constexpr size_t InitPool{ 0x00 };
        static inline constexpr ComponentFlags Flags{ 0x00 };
        static inline constexpr const char* Name { "Component" };
    };

    struct ComponentRef {
        UUID8 goRef{};
        UUID8 compRef{};
    };

    class GameObject;
    class Component : public IObject {
    public:
        Component();
        virtual ~Component();
  
        GameObject* getObject() const { return _object; }

        void deserializeBinary(const Stream& stream);
        void serializeBinary(const Stream& stream) const;

        void deserializeYAML(yamlNode& node);
        void serializeYAML(yamlEmit& emit) const;

        void deserializeJSON(json& jsonF);
        void serializeJSON(json& jsonF) const;

        template<typename T, size_t init = ComponentInfo<T>::InitPool>
        static PoolAllocator<T, init>& getComponentAllocator() {
            return PoolAllocator<T, init>::getGlobal();
        }

    protected:
        friend class ComponentFactory;
        friend class GameObject;

        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component&) = delete;

        void* operator new(size_t size) = delete;
        void operator delete(void* ptr) = delete;

        virtual void doDelete() = 0;

        virtual void onInit() {};
        virtual void onSetup() {};
        virtual void onStart() {};
        virtual void onUpdate(float time, float delta) {};
        virtual void onDestroy() {};

        virtual void deserializeBinaryImpl(const Stream& stream) = 0;
        virtual void serializeBinaryImpl(const Stream& stream) const = 0;

        virtual void deserializeYAMLImpl(yamlNode& node) = 0;
        virtual void serializeYAMLImpl(yamlEmit& emit) const = 0;

        virtual void deserializeJSONImpl(json& jsonF) = 0;
        virtual void serializeJSONImpl(json& jsonF) const = 0;

        template<typename T>
        static void deleteComponent(T* ptr) {
            getComponentAllocator<T>().deallocate(ptr);
        }

    private:
        friend class GameObject;

        GameObject* _object;
        void init(GameObject* go, uint16_t flags);

        void start();
        void update(float time, float delta);
        void destroy();
    };
}
REGISTER_UUID_FACTORY(JEngine::Component);