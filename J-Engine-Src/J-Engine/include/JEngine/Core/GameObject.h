#pragma once
#include <vector>
#include <JEngine/Components/Component.h>
#include <string>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Collections/PoolAllocator.h>

namespace JEngine {
    class CTransform;
    class Scene;
    class GameObject : public IPoolObj {
    public:
        static constexpr uint8_t MAX_COMPONENTS = 32;

        ~GameObject();

        void dCtor() override;

        Span<Component*> getAllComponents() { return Span<Component*>(_components, _compInfo.count); }
        ConstSpan<Component*> getAllComponents() const { return ConstSpan<Component*>(_components, _compInfo.count); }

        std::string& setName(const char* newName) { return _name = newName; }
        std::string& setName(const std::string& newName) { return _name = newName; }
        const std::string& getName() const { return _name; }

        UUID8 getUUID() const { return _uuid; }

        CTransform* getTransform();
        const CTransform* getTransform() const;

        static PoolAllocator<GameObject, 1>& getGameObjectAllocator() {
            return PoolAllocator<GameObject, 1>::getGlobal();
        }

        template<typename T>
        T* getComponent() const {
            for (const auto& cur : _components) {
                T* comp = dynamic_cast<T*>(cur);
                if (comp) {
                    return comp;
                }
            }
            return nullptr;
        }

        template<typename T>
        T* getComponent(const UUID8& uuid) const {
            for (const auto& cur : _components) {
                T* comp = dynamic_cast<T*>(cur);
                if (comp && comp->getUUID() == uuid) {
                    return comp;
                }
            }
            return nullptr;
        }

        template<typename T>
        size_t getComponents(T* buffer, size_t maxCount) const {
            size_t count = 0;
            for (const auto& cur : _components) {
                T* comp = dynamic_cast<T*>(cur);
                if (comp) {
                    buffer[count++] = comp;
                    if (count >= maxCount) { break; }
                }
            }
            return count;
        }

        template<typename T, size_t size>
        size_t getComponents(T(&buffer)[size]) const {
            size_t count = 0;
            for (const auto& cur : _components) {
                T* comp = dynamic_cast<T*>(cur);
                if (comp) {
                    buffer[count++] = comp;
                    if (count >= size) { break; }
                }
            }
            return count;
        }

        bool addComponent(Component* comp, bool autoStart = true);
        bool removeComponent(const UUID8& uuid, bool destroy);
        bool removeComponent(Component* comp, bool destroy);

        template<typename T>
        T* addComponent(uint16_t flags = 0, bool autoStart = true) {
            if (_compInfo.count >= MAX_COMPONENTS) { 
                JENGINE_CORE_WARN("[J-Engine - GameObject] Warning: Max number of components ({0}) for GameObject '{1}' [0x{2:x}] has been reached!!", MAX_COMPONENTS, _name.c_str(), _uuid.asUInt());
                return nullptr; 
            }

            if ((ComponentInfo<T>::Flags & COMP_IS_TRANSFORM) && _compInfo.trIndex != NULL_TRANSFORM) {
                JENGINE_CORE_ERROR("[J-Engine - GameObject] Error: Cannot add multiple transforms to one gameobject!");
                return nullptr;
            }

            T* newComp = Component::getComponentAllocator<T>().allocate();
            if (!newComp) {
                JENGINE_CORE_ERROR("[J-Engine - GameObject] Error: Failed to allocate component!");
                return nullptr;
            }

            Component* comp = dynamic_cast<Component*>(newComp);
            if (!comp) {
                JENGINE_CORE_ERROR("[J-Engine - GameObject] Error: Given type isn't a Component!");
                Component::getComponentAllocator<T>().deallocate(newComp);
                return nullptr;
            }

            comp->init(this, flags);
            if (addComponent(comp, autoStart)) {
                UUID8 uuid = UUIDFactory::getRandomUUID();

                int32_t ind = indexOfComponent(uuid);
                while (uuid == UUID8::Empty || ind < MAX_COMPONENTS) {
                    uuid = UUIDFactory::getRandomUUID();
                    ind = indexOfComponent(uuid);
                }

                comp->_uuid = uuid;
                return newComp;
            }

            JENGINE_CORE_ERROR("[J-Engine - GameObject] Error: Failed to add Component!");
            Component::getComponentAllocator<T>().deallocate(newComp);
            return nullptr;
        }

        uint8_t indexOfComponent(const Component* comp) const;
        uint8_t indexOfComponent(const UUID8& uuid) const;

        static GameObject* createObject(const char* name, uint16_t flags = 0x00, UUID8 uuid = UUID8::Empty);
        static GameObject* createObject(const std::string& name, uint16_t flags = 0x00, UUID8 uuid = UUID8::Empty);

        static GameObject* destroyObject(GameObject* go);


        static void deserializeCompRefBinary(const Stream& stream, UUID8& go, UUID8& comp);
        static void serializeCompRefBinary(const Stream& stream, const GameObject* go, const Component* comp);

        static void deserializeCompRefYAML(yamlNode& node, UUID8& go, UUID8& comp);
        static void serializeCompRefYAML(yamlEmit& emit, const GameObject* go, const Component* comp);

        static void deserializeCompRefJSON(json& jsonF, UUID8& go, UUID8& comp);
        static void serializeCompRefJSON(json& jsonF, const GameObject* go, const Component* comp);

    private:
        friend class Scene;
        friend class PoolAllocator<GameObject>;
        friend struct PoolChunk<GameObject>;
        static constexpr uint8_t NULL_TRANSFORM = 0xFF;

        struct CompInfo {
            uint8_t count{ 0 };
            uint8_t trIndex{ NULL_TRANSFORM };
        };

        GameObject();

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject&) = delete;

        void* operator new(size_t size) noexcept = delete;
        void operator delete(void* ptr) noexcept = delete;
        void init(const char* name, uint16_t flags);

        UUID8 _uuid;
        UI16Flags _flags;
        std::string _name;
        Scene* _scene;

        CompInfo _compInfo{};
        Component* _components[MAX_COMPONENTS]{nullptr};
    };
}
REGISTER_UUID_FACTORY(JEngine::GameObject);