#pragma once
#include <JEngine/IO/Serialization/SerializedItem.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Core/String.h>

namespace JEngine {
    enum ObjectFlags : uint64_t {
        OBJ_FLAG_NO_SERIALIZE = 0x1,

        OBJ_FLAGS_INSTANCE = 0x00000000FFFFFFFFU,
        OBJ_FLAGS_IMPL = 0x00FFFFFF00000000U,
        OBJ_FLAGS_RUNTIME = 0xFF00000000000000U,

        FLAG_IS_DESTROYED = 0x0100000000000000U,
        FLAG_IS_LOADED = 0x0200000000000000U,
    };

    class IObject {
    public:
        ObjectFlags getFlags() const { return ObjectFlags(_oFlags.getValue()); }
        uint64_t setUUID(uint64_t uuid) { return _uuid = uuid; }
        uint64_t getUUID() const { return _uuid; }

        void initObj(uint64_t uuid, uint64_t flags) {
            _uuid = uuid;
            _oFlags = UI64Flags(flags & (OBJ_FLAGS_INSTANCE | OBJ_FLAGS_IMPL));
        }

        virtual void deserialize(const SerializedItem& item) {
            _uuid = item.findByKey()
        }
        virtual void serialize(SerializedItem& item) const {

        }


    protected:
        virtual void deserializeImpl(const SerializedItem& item) = 0;
        virtual void serializeImpl(SerializedItem& item) const = 0;

        UI64Flags& getFlags() { return _oFlags; }
    private:
        uint64_t _uuid{};
        UI64Flags _oFlags{};
    };

    class INamedObject : public IObject {
    public:
        std::string_view setName(std::string_view newName) { return _name = newName; }
        std::string_view getName() const { return _name; }

        void initObj(std::string_view name, uint64_t uuid, uint64_t flags) {
            IObject::initObj(uuid, flags);
            _name = String(name);
        }
    private:
        String _name{};
    };
}