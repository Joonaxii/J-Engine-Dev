#include <JEngine/Components/Component.h>
#include <JEngine/Core/GameObject.h>

namespace JEngine {
    Component::Component() : IObject(), _object(nullptr) { }

    Component::~Component() {
        if ((getFlags() & FLAG_IS_DESTROYED) == 0) {
            _object = nullptr;
            getFlags() |= FLAG_IS_DESTROYED;
            UUIDFactory::removeUUID<Component>(getUUID());
            setUUID(UUID8::Empty);
        }
    }

    void Component::init(GameObject* go, uint16_t flags) {
        _object = go;
        initObj(UUID8::Empty, flags & FLAGS_INSTANCE);
        onInit();
    }

    void Component::start() {
        onStart();
    }

    void Component::update(float time, float delta) {
        onUpdate(time, delta);
    }

    void Component::destroy() {
        if (getFlags()  & FLAG_IS_DESTROYED) { return; }
        onDestroy();
        doDelete();
    }

    void Component::deserializeBinary(const Stream& stream) {
        UI8Flags flags{};
        Serialization::deserialize(flags, stream);
        getFlags() = UI16Flags(flags.getValue());

        deserializeBinaryImpl(stream);
    }

    void Component::serializeBinary(const Stream& stream) const {
        UI8Flags flags(getFlags());

        Serialization::serialize(flags, stream);
        serializeBinaryImpl(stream);
    }

    void Component::deserializeYAML(yamlNode& node) {
        UI8Flags flags{};
        Serialization::deserialize(flags, node["flags"]);
        getFlags() = UI16Flags(flags.getValue());

        deserializeYAMLImpl(node);
    }

    void Component::serializeYAML(yamlEmit& emit) const {
        UI8Flags flags(getFlags());

        emit << YAML::BeginMap; 
        Serialization::serialize(flags, emit << YAML::Key << "flags" << YAML::Value);

        serializeYAMLImpl(emit);
        emit << YAML::EndMap;
    }

    void Component::deserializeJSON(json& jsonF) {
        UI8Flags flags{};
        Serialization::deserialize(flags, jsonF["flags"]);
        getFlags() = UI16Flags(flags.getValue());
        deserializeJSONImpl(jsonF);
    }

    void Component::serializeJSON(json& jsonF) const {
        UI8Flags flags(getFlags());

        Serialization::serialize(flags, jsonF["flags"]);
        serializeJSONImpl(jsonF);
    }

}