#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Cryptography/UUID.h>
#include <JEngine/Utility/Flags.h>

namespace JEngine {
    enum ObjectFlags : uint16_t {
        FLAG_NO_SERIALIZE = 0x1,


        FLAGS_INSTANCE    = 0x00FF,
        FLAGS_IMPL        = 0x0F00,
        FLAGS_RUNTIME     = 0xF000,
        FLAG_IS_DESTROYED = 0x1000,
        FLAG_IS_LOADED    = 0x2000,
    };

    class IObject {
    public:
        uint16_t getFlags() const { return _oFlags.getValue(); }
        uint32_t setUUID(uint32_t uuid) { return _uuid = uuid; }
        uint32_t getUUID() const     { return _uuid; }

        void initObj(uint32_t uuid, uint16_t flags) {
            _uuid = uuid;
            _oFlags = UI16Flags(flags & (FLAGS_INSTANCE | FLAGS_IMPL));
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
        uint32_t _uuid{};
        UI16Flags _oFlags{};
    };

    class INamedObject : public IObject {
    public:
        std::string_view setName(std::string_view newName) { return _name = newName; }
        std::string_view getName() const { return _name; }

        void initObj(std::string_view name, uint32_t uuid, uint16_t flags) {
            IObject::initObj(uuid, flags);
            _name = std::string(name);
        }
    protected:
        std::string& getNameInternal() { return _name; }
    private:
        std::string _name{};
    };
}