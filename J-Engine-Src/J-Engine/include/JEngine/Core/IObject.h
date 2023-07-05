#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Cryptography/UUID.h>
#include <JEngine/Utility/Flags.h>

namespace JEngine {
    enum ObjectFlags : uint16_t {
        FLAG_NO_SERIALIZE = 0x1,

        FLAGS_INSTANCE = 0xFF,
        FLAGS_RUNTIME = 0xFF00,
        FLAG_IS_DESTROYED = 0x1000,
        FLAG_IS_LOADED    = 0x2000,
    };

    class IObject {
    public:
        uint16_t getFlags() const { return _oFlags.getValue(); }
        UUID8 setUUID(UUID8 uuid) { return _uuid = uuid; }
        UUID8 getUUID() const     { return _uuid; }

        void initObj(UUID8 uuid, uint16_t flags) {
            _uuid = uuid;
            _oFlags = UI16Flags(flags & FLAGS_INSTANCE);
        }

    protected:
        UI16Flags& getFlags() { return _oFlags; }

        virtual void deserializeBinaryImpl(const Stream& stream) {}
        virtual void serializeBinaryImpl(const Stream& stream) const {}

        virtual void deserializeYAMLImpl(yamlNode& node) {}
        virtual void serializeYAMLImpl(yamlEmit& emit) const {}

        virtual void deserializeJSONImpl(json& jsonF) {}
        virtual void serializeJSONImpl(json& jsonF) const {}

    private:
        UUID8 _uuid{};
        UI16Flags _oFlags{};
    };

    class INamedObject : public IObject {
    public:
        const std::string& setName(const char* newName) { return _name = newName; }
        const std::string& setName(const std::string& newName) { return _name = newName; }
        const std::string& getName() const { return _name; }

        void initObj(const char* name, UUID8 uuid, uint16_t flags) {
            IObject::initObj(uuid, flags);
            _name = std::string(name);
        }
    protected:
        std::string& getNameInternal() { return _name; }
    private:
        std::string _name{};
    };
}