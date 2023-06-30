#include <JEngine/Components/Component.h>
#include <JEngine/Core/GameObject.h>

namespace JEngine {
    Component::Component() : _object(nullptr), _flags(), _uuid() { }

    Component::~Component() {
        if ((_flags & FLAG_IS_DESTROYED) == 0) {
            dCtor();
        }
    }

    void Component::dCtor() {
        _object = nullptr;
        _flags |= FLAG_IS_DESTROYED;
        UUIDFactory::removeUUID<Component>(_uuid);
        _uuid = UUID8::Empty;
    }

    void Component::init(GameObject* go, uint16_t flags) {
        _object = go;
        _flags = UI16Flags(flags & FLAGS_INSTANCE);
        onInit();
    }

    void Component::start() {
        onStart();
    }

    void Component::update(float delta) {
        onUpdate(delta);
    }

    void Component::destroy() {
        if (_flags & FLAG_IS_DESTROYED) { return; }
        onDestroy();
    }

    void Component::deserializeBinary(const Stream& stream) {
        UI8Flags flags{};
        Serialization::deserialize(flags, stream);
        _flags = UI16Flags(flags.getValue());

        deserializeBinaryImpl(stream);
    }

    void Component::serializeBinary(const Stream& stream) const {
        UI8Flags flags(uint8_t(_flags.getValue()));

        Serialization::serialize(flags, stream);
        serializeBinaryImpl(stream);
    }

    void Component::deserializeYAML(yamlNode& node) {
        UI8Flags flags{};
        Serialization::deserialize(flags, node["flags"]);
        _flags = UI16Flags(flags.getValue());

        deserializeYAMLImpl(node);
    }

    void Component::serializeYAML(yamlEmit& emit) const {
        UI8Flags flags(uint8_t(_flags.getValue()));

        emit << YAML::BeginMap;
        emit << YAML::Key << "flags" << YAML::Value << flags;
        serializeYAMLImpl(emit);
        emit << YAML::EndMap;
    }

    void Component::deserializeJSON(json& jsonF) {
        UI8Flags flags{};
        Serialization::deserialize(flags, jsonF["flags"]);
        _flags = UI16Flags(flags.getValue());
        deserializeJSONImpl(jsonF);
    }

    void Component::serializeJSON(json& jsonF) const {
        UI8Flags flags(uint8_t(_flags.getValue()));

        Serialization::serialize(flags, jsonF["flags"]);
        serializeJSONImpl(jsonF);
    }

}