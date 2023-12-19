#pragma once
#include <cstdint>
#include <JEngine/Components/ComponentFactory.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Collections/PoolAllocator.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Core/IObject.h>
#include <JEngine/Core/Ref.h>

namespace JEngine {
    class GameObject;
    class Component : public IObject {
    public:
        Component();
        virtual ~Component();
  
        GORef getObject() const { return GORef(getUUID()); }

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

        void init(CompRef compRef, uint16_t flags);

        void start();
        void update(float time, float delta);
        void destroy();
    };
}