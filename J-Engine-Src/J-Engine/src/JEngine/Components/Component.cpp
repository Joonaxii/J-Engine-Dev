#include <JEngine/Components/Component.h>
#include <JEngine/Core/GameObject.h>

namespace JEngine {
    Component::Component() : IObject() { }

    Component::~Component() {
        getFlags() |= FLAG_IS_DESTROYED;
    }

    void Component::init(CompRef compRef, uint16_t flags) {
        initObj(compRef.uuid, flags & FLAGS_INSTANCE);
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
        uint8_t flags{};
        Serialization::deserialize(flags, stream);
        getFlags() = UI16Flags(flags);

        deserializeBinaryImpl(stream);
    }

    void Component::serializeBinary(const Stream& stream) const {
        Serialization::serialize(uint8_t(getFlags()), stream);
        serializeBinaryImpl(stream);
    }

    void Component::deserializeYAML(yamlNode& node) {
        uint8_t flags{};
        Serialization::deserialize(flags, node["flags"]);
        getFlags() = UI16Flags(flags);
        deserializeYAMLImpl(node);
    }

    void Component::serializeYAML(yamlEmit& emit) const {
        emit << YAML::BeginMap; 
        Serialization::serialize(uint8_t(getFlags()), emit << YAML::Key << "flags" << YAML::Value);

        serializeYAMLImpl(emit);
        emit << YAML::EndMap;
    }

    void Component::deserializeJSON(json& jsonF) {
        uint8_t flags{};
        Serialization::deserialize(flags, jsonF["flags"]);
        getFlags() = UI16Flags(flags);
        deserializeJSONImpl(jsonF);
    }

    void Component::serializeJSON(json& jsonF) const {
        Serialization::serialize(uint8_t(getFlags()), jsonF["flags"]);
        serializeJSONImpl(jsonF);
    }

}