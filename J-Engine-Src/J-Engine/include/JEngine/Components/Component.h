#pragma once
#include <cstdint>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Cryptography/UUIDFactory.h>

namespace JEngine {
    enum ObjectFlags : uint16_t {
        FLAG_NO_SERIALIZE = 0x1,
        FLAG_IS_DESTROYED = 0x1000,
    };

    template<typename T>
    struct IsTransform {
        static const bool Value{ false };
    };

    class GameObject;
    class Component {
    public:
        virtual ~Component();

        virtual const char* getComponentName() const { return "Component"; }
        GameObject* getObject() const { return _object; }

        const UUID8& getUUID() const { return _uuid; }

    protected:
        friend class ComponentFactory;
        friend class GameObject;
        Component(GameObject* obj, uint16_t flags = 0x00);

        Component(const GameObject& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component&) = delete;

        virtual void onSetup() {};
        virtual void onStart() {};
        virtual void onUpdate(float delta) {};
        virtual void onDestroy() {};

    private:
        GameObject* _object;
        UUID8 _uuid;
        UI16Flags _flags;
        friend class GameObject;

        void start();
        void update(float delta);
        void destroy();
    };
}
REGISTER_UUID_FACTORY(JEngine::Component);