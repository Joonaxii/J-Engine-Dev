#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Cryptography/UUIDFactory.h>
#include <JEngine/Collections/PoolAllocator.h>

namespace JEngine {
    enum ObjectFlags : uint16_t {
        FLAG_NO_SERIALIZE = 0x1,

        FLAGS_INSTANCE = 0xFF,
        FLAGS_RUNTIME = 0xFF00,
        FLAG_IS_DESTROYED = 0x1000,
    };

    enum ComponentFlags: uint8_t {
        COMP_IS_TRANSFORM = 0x1,
    };

    template<typename T>
    struct ComponentInfo {
        static inline constexpr ComponentFlags Flags{ 0x00 };
        static inline constexpr const char* Name { "Component" };
    };

    struct ComponentRef {
        UUID8 goRef{};
        UUID8 compRef{};
    };

    class GameObject;
    class Component : public IPoolObj {
    public:
        Component();
        virtual ~Component();
        virtual void dCtor() override;

        GameObject* getObject() const { return _object; }

        UUID8 getUUID() const { return _uuid; }

        void deserializeBinary(const Stream& stream);
        void serializeBinary(const Stream& stream) const;

        void deserializeYAML(yamlNode& node);
        void serializeYAML(yamlEmit& emit) const;

        void deserializeJSON(json& jsonF);
        void serializeJSON(json& jsonF) const;

        template<typename T>
        static PoolAllocator<T>& getComponentAllocator() {
            return PoolAllocator<T>::getGlobal();
        }

    protected:
        friend class ComponentFactory;
        friend class GameObject;

        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component&) = delete;

        void* operator new(size_t size) = delete;
        void operator delete(void* ptr) = delete;

        virtual void onInit() {};
        virtual void onSetup() {};
        virtual void onStart() {};
        virtual void onUpdate(float delta) {};
        virtual void onDestroy() = 0;

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
        UUID8 _uuid;
        UI16Flags _flags;

        void init(GameObject* go, uint16_t flags);

        void start();
        void update(float delta);
        void destroy();
    };
}
REGISTER_UUID_FACTORY(JEngine::Component);