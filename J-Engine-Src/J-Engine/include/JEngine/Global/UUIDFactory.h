#pragma once
#pragma comment(lib, "rpcrt4.lib")
#include <rpc.h>

#include <string>
#include <unordered_set>

namespace JEngine {
    struct UUIDHash {
        const size_t operator()(const UUID& uuid) const{
            RPC_STATUS status;
            return size_t(UuidHash(const_cast<UUID*>(&uuid), &status));
        }
    };
}
typedef std::unordered_set<UUID, const JEngine::UUIDHash> UUIDSet;

namespace JEngine {
    class UUIDFactory {
    public:
        static constexpr UUID UUID_EMPTY{ 0 };

        template<typename T>
        static UUIDSet& getUUIDSet();

        template<typename T>
        static const bool generateUUID(UUID& uuid);

        template<typename T>
        static const bool addUUID(const UUID& uuid);

        template<typename T>
        static const void removeUUID(const UUID& uuid);

        template<typename T>
        static const bool isUUIDFound(const UUID& uuid);

        static const std::string getUUIDStr(const UUID& uuid) {
            std::string uuidStr;
            RPC_CSTR szUuid = NULL;
            if (UuidToStringA(&uuid, &szUuid) == RPC_S_OK) {
                uuidStr = (char*)szUuid;
                RpcStringFreeA(&szUuid);
            }
            return uuidStr;
        }

        static const bool parseUUID(const std::string& str, UUID& uuid) {
            if (str.length() < 32) { *&uuid = UUID_EMPTY; return false; }
            RPC_CSTR rpcStr = RPC_CSTR(str.c_str());
            auto ret = UuidFromStringA(rpcStr, &uuid);
            return ret == RPC_S_OK;
        } 

    private:
       static std::unordered_set<UUID> _usedUUIDs;
    };

   /* inline bool operator == (const UUID& a, const UUID& b) {
        return std::memcmp(&a, &b, sizeof(UUID)) == 0;
    }*/

    template<typename T>
    inline const bool UUIDFactory::generateUUID(UUID& uuid) {
        auto& uuids = getUUIDSet<T>();
        *&uuid = UUID_EMPTY;
        auto ret = UuidCreate(&uuid);

        while (isUUIDFound<T>(uuid)) {
            *&uuid = UUID_EMPTY;
            ret = UuidCreate(&uuid);
            if (ret != RPC_S_OK) {
                *&uuid = UUID_EMPTY;
                return false;
            }
        }
        uuids.insert(uuid);
        return true;
    }

    template<typename T>
    inline const bool UUIDFactory::addUUID(const UUID& uuid) {
        auto& uuids = getUUIDSet<T>();
        if (isUUIDFound<T>(uuid)) { return false; }
        uuids.insert(uuid);
        return true;
    }

    template<typename T>
    inline const void UUIDFactory::removeUUID(const UUID& uuid) {
        if (uuid == UUID_EMPTY) { return; }
        getUUIDSet<T>().erase(uuid);
    }

    template<typename T>
    inline const bool UUIDFactory::isUUIDFound(const UUID& uuid) {
        auto& uuids = getUUIDSet<T>();
        return uuids.find(uuid) != uuids.end();
    }
}

#define REGISTER_UUID_FACTORY(TYPE) \
template<> \
inline UUIDSet& JEngine::UUIDFactory::getUUIDSet<TYPE>() { \
    static UUIDSet uuids; \
    return uuids; \
} \
