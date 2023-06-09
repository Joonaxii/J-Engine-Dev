#pragma once

#include <string>
#include <unordered_set>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Cryptography/UUID.h>
#include <random>

typedef std::unordered_set<JEngine::UUID8, std::hash<JEngine::UUID8>> UUIDSet;

namespace JEngine {
    class UUIDFactory {
    public:
        template<typename T>
        static UUIDSet& getUUIDSet() {
            static_assert("UUIDFactory not implemented for given type!");
        }

        template<typename T>
        static bool generateUUID(UUID8& uuid);

        template<typename T>
        static bool addUUID(const UUID8& uuid);

        template<typename T>
        static const void removeUUID(const UUID8& uuid);

        template<typename T>
        static bool isUUIDFound(const UUID8& uuid);

        static UUID8 getRandomUUID() {
            static std::random_device RandomDevice;
            static std::mt19937_64    Engine(RandomDevice());
            static std::uniform_int_distribution<uint64_t> UniformDistribution;
            return UUID8(UniformDistribution(Engine));
        }
    private:
       static std::unordered_set<UUID8> _usedUUIDs;
    };

    template<typename T>
    inline bool UUIDFactory::generateUUID(UUID8& uuid) {
        UUIDSet& uuids = getUUIDSet<T>();
        uuid = getRandomUUID();

        int32_t retries = 100000;
        while (isUUIDFound<T>(uuid)) {
            if (retries <= 0) {
                uuid = UUID8::Empty;
                return false;
            }

            uuid = getRandomUUID();
            retries--;
        }
        uuids.insert(uuid);
        return true;
    }

    template<typename T>
    inline bool UUIDFactory::addUUID(const UUID8& uuid) {
        auto& uuids = getUUIDSet<T>();
        if (isUUIDFound<T>(uuid)) { return false; }
        uuids.insert(uuid);
        return true;
    }

    template<typename T>
    inline const void UUIDFactory::removeUUID(const UUID8& uuid) {
        if (uuid == UUID8::Empty) { return; }
        getUUIDSet<T>().erase(uuid);
    }

    template<typename T>
    inline bool UUIDFactory::isUUIDFound(const UUID8& uuid) {
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
