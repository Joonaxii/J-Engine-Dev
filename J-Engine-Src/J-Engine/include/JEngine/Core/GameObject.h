#pragma once
#include <vector>
#include <JEngine/Components/Component.h>
#include <string>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Utility/Span.h>

namespace JEngine {
    class CTransform;
    class GameObject {
    public:
        static constexpr uint8_t MAX_COMPONENTS = 32;

        ~GameObject();

        Span<Component*> getAllComponents() { return Span<Component*>(_components, _compInfo.count); }
        ConstSpan<Component*> getAllComponents() const { return ConstSpan<Component*>(_components, _compInfo.count); }

        std::string& setName(const char* newName) { return _name = newName; }
        std::string& setName(const std::string& newName) { return _name = newName; }
        const std::string& getName() const { return _name; }

        CTransform* getTransform();
        const CTransform* getTransform() const;

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
        T* addComponent(bool autoStart = true) {
            T* newComp = new T(this);
            Component* comp = dynamic_cast<Component*>(newComp);
            if (comp) {
                if (IsTransform<T>::Value && _compInfo.trIndex != NULL_TRANSFORM) {
                    JENGINE_CORE_ERROR("Cannot add multiple transforms to one gameobject!");
                    goto doDel;
                }

                if (addComponent(comp, autoStart)) {
                    return newComp;
                }
            }

            doDel:
            delete newComp;
            return nullptr;
        }

        uint8_t indexOfComponent(const Component* comp) const;
        uint8_t indexOfComponent(const UUID8& uuid) const;

        static GameObject* createObject(const char* name, uint16_t flags = 0x00);
        static GameObject* createObject(const std::string& name, uint16_t flags = 0x00);

        static void destroyObject(GameObject*& go);
        static void destroyObject(const GameObject* go);

    private:
        static constexpr uint8_t NULL_TRANSFORM = 0xFF;

        struct ComponentInfo {
            uint8_t count{ 0 };
            uint8_t trIndex{ NULL_TRANSFORM };
        };

        GameObject(const char* name, uint16_t flags);
        GameObject(const std::string& name, uint16_t flags);

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject&) = delete;

        void operator delete(void* ptr) noexcept;

        UI16Flags _flags;
        std::string _name;

        ComponentInfo _compInfo{};
        Component* _components[MAX_COMPONENTS]{nullptr};
    };
}