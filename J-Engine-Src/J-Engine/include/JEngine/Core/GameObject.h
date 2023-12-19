#pragma once
#include <JEngine/Collections/IndexStack.h>
#include <JEngine/Components/Component.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Utility/JTime.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Core/IObject.h>
#include <JEngine/Core/Ref.h>
#include <vector>
#include <string>

namespace JEngine {
    class CTransform;
    class Scene;
    class GameObject : public INamedObject {
    public:
        static constexpr uint8_t MAX_COMPONENTS = 8;

        ~GameObject();

        template<uint32_t bufSize>
        uint32_t getAllComponents(CompRef(&buffer)[bufSize]) {
            uint32_t c = 0;
            for (uint32_t i = 0; c < bufSize && i < MAX_COMPONENTS; i++) {
                JEngine::Component** comp = _components.getAt(i);
                if (comp) {
                    buffer[c++] = ComponentRef(getUUID(), i);
                }
            }
        }

        void getAllComponents(std::vector<CompRef>& components) const;

        JTimeIndex getTimeSpace() const { return _timeSpace; }
        void setTimeSpace(JTimeIndex space) { _timeSpace = space; }

        TCompRef<CTransform> getTransform() const;

        template<typename T>
        TCompRef<T> getComponent() const {
            for (uint32_t i = 0, c = 0; i < MAX_COMPONENTS; i++) {
                Component** comp = _components.getAt(i);
                if (comp) {
                    T* compT = dynamic_cast<T*>(*comp);
                    if (compT) {
                        return TCompRef<T>(compT);
                    }
                }
            }
            return TCompRef<T>(nullptr);
        }

        Component* getComponentByUUID(CompRef uuid) const {
            if (uuid.isForObject(getUUID())) {
                Component* const* comp = _components.getAt(uuid.getIndex());
                return comp ? *comp : nullptr;
            }
            return nullptr;
        }

        Component* getComponentByIndex(uint32_t index) const {
            Component* const* comp = _components.getAt(index);
            return comp ? *comp : nullptr;
        }

        template<typename T>
        uint32_t getComponents(TCompRef<T>* buffer, uint32_t maxCount) const {
            uint32_t count = 0;
            for (uint32_t i = 0, c = 0; i < MAX_COMPONENTS; i++) {
                Component** comp = _components.getAt(i);
                if (comp) {
                    T* compT = dynamic_cast<T*>(*comp);
                    if (compT) {
                        buffer[count++] = TCompRef<T>(compT);
                        if (count >= maxCount) { break; }
                    }
                }
            }
            return count;
        }

        template<typename T, uint32_t size>
        uint32_t getComponents(TCompRef<T>(&buffer)[size]) const {
            return getComponents<T>(buffer, size);
        }

        uint32_t getComponents(CompRef* buffer, uint32_t maxCount) const {
            uint32_t count = 0;
            for (uint32_t i = 0, c = 0; i < MAX_COMPONENTS; i++) {
                Component* const* comp = _components.getAt(i);
                if (comp && *comp) {
                    buffer[count++] = CompRef((*comp)->getUUID());
                    if (count >= maxCount) { break; }
                }
            }
            return count;
        }

        template<uint32_t size>
        uint32_t getComponents(CompRef(&buffer)[size]) const {
            return getComponents(buffer, size);
        }


        CompRef addComponent(Component* comp, uint16_t flags = 0, bool autoStart = true);
        bool removeComponent(uint32_t uuid, bool destroy);
        bool removeComponent(Component* comp, bool destroy);

        template<typename T>
        TCompRef<T> addComponent(uint16_t flags = 0, bool autoStart = true) {
            if (_compInfo.getCount() >= MAX_COMPONENTS) {
                JENGINE_CORE_WARN("[GameObject] Warning: Max number of components ({0}) for GameObject '{1}' [0x{2:X}] has been reached!!", MAX_COMPONENTS, getName(), getUUID());
                return TCompRef<T>(nullptr);
            }

            if ((ComponentInfo<T>::Flags & COMP_IS_TRANSFORM) && _compInfo.getTrIndex() != NULL_TRANSFORM) {
                JENGINE_CORE_ERROR("[GameObject] Error: Cannot add multiple transforms to one GameObject!");
                return TCompRef<T>(nullptr);
            }

            T* newComp = Component::getComponentAllocator<T>().allocate();
            if (!newComp) {
                JENGINE_CORE_ERROR("[GameObject] Error: Failed to allocate Component!");
                return TCompRef<T>(nullptr);
            }

            Component* comp = dynamic_cast<Component*>(newComp);
            if (!comp) {
                JENGINE_CORE_ERROR("[GameObject] Error: Given type isn't a Component!");
                Component::getComponentAllocator<T>().deallocate(newComp);
                return TCompRef<T>(nullptr);
            }

            if (addComponent(comp, flags, autoStart)) {
                return TCompRef<T>(newComp);
            }

            JENGINE_CORE_ERROR("[GameObject] Error: Failed to add Component!");
            Component::getComponentAllocator<T>().deallocate(newComp);
            return TCompRef<T>(nullptr);
        }

    private:
        friend class Scene;
        friend class ChunkedLUT<GameObject>;
        static constexpr uint8_t NULL_TRANSFORM = 0xF;

        struct CompInfo {
        public:
            constexpr CompInfo() : _count(0xF0) {}
            constexpr CompInfo(uint8_t count, uint8_t trIndex) : 
                _count((count & 0xF) | ((trIndex & 0xF) << 4)) {}

            void setCount(uint8_t count) { _count = (_count & 0xF0) | (count & 0xF); }
            CompInfo operator++(int) {
                auto prev = *this;
                if ((_count & 0xF) == 0xF) { 
                    return prev; 
                }
                _count = ((_count & 0xF) + 1) |(_count & 0xF0);
                return prev;
            }
            CompInfo operator--(int) {
                auto prev = *this;
                if ((_count & 0xF) == 0x0) {
                    return prev; 
                }
                _count = ((_count & 0xF) - 1) | (_count & 0xF0);
                return prev;
            }

            CompInfo& operator++() {
                if ((_count & 0xF) == 0xF) {
                    return *this;
                }
                _count = ((_count & 0xF) + 1) | (_count & 0xF0);
                return *this;
            }
            CompInfo& operator--() {
                if ((_count & 0xF) == 0x0) {
                    return *this;
                }
                _count = ((_count & 0xF) - 1) | (_count & 0xF0);
                return *this;
            }
            void setTrIndex(uint8_t index) { _count = (_count & 0xF) | ((index & 0xF) << 4); }

            uint8_t getCount() const { return _count & 0xF; }
            uint8_t getTrIndex() const { return (_count >> 4) & 0xF; }

        private:
            uint8_t _count{ 0 };
        };
        JTimeIndex _timeSpace;

        CompInfo _compInfo{};
        FixedLUT<Component*, MAX_COMPONENTS> _components{};

        GameObject();

        //GameObject(const GameObject& other) = delete;
        //GameObject(GameObject&& other) = delete;
        //GameObject& operator=(const GameObject&) = delete;
        //
        //void* operator new(size_t size) noexcept {
        //    return ::operator new(size);
        //}
        //void operator delete(void* ptr) noexcept = delete;
        void init(std::string_view name, uint16_t flags);

        void start();
        void update(const JTime& time);
    };
    static constexpr size_t siz = sizeof(GameObject);
}