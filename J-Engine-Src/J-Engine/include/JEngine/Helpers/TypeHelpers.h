#pragma once
#include <JEngine/Cryptography/UUID.h>
#include <typeindex>
#include <vector>
#include <string>
#define NAMEOF(name) #name

namespace JEngine {
    static constexpr size_t TYPE_UUID_BLOCK_SIZE = 8;

    struct Type {
        std::string name;
        size_t size;
        UUID16 hash;

        Type() : name(""), hash(), size(-1) {}
    };

    class TypeHelpers {
    public:
        template<typename T> static Type* getType() {
            static_assert("Not implemented for given type!");
        }
        static std::vector<Type*>& getTypes();

        const static std::pair<const UUID16, const std::string> getTypePair(const std::type_index& indx) {
            std::string tname = indx.name();
            size_t ind = tname.find(' ', 0);
            if (ind != std::string::npos) {
                tname.erase(0, ind + 1);
            }

            UUID16 hash;
            hash.computeHash(tname.c_str(), tname.length(), TYPE_UUID_BLOCK_SIZE);
            return std::pair<const UUID16, const std::string>(hash, tname);
        }

        const static Type* getTypeByHash(const UUID16& hash) {
            auto& types = getTypes();
            for (size_t i = 0; i < types.size(); i++) {
                if (types[i]->hash == hash) { return types[i]; }
            }
            return nullptr;
        }

        const static std::string getTypeName(const UUID16& id) {
            auto type = getTypeByHash(id);
            return type ? type->name : "";
        }

        template<typename T>
        const static std::string getTypeName() {
            Type* type = JEngine::TypeHelpers::getType<T>();
            return type ? type->name : "";
        }

        template<typename T>
        const static std::string getTypeName(const T& obj) {
            const std::type_index& typeInf = typeid(obj);
            return getTypePair(typeInf).second;
        }

        template<typename T>
        const static UUID16 getTypeHash() {
            Type* type = JEngine::TypeHelpers::getType<T>();
            return type ? type->hash : UUID16();
        }

        template<typename T>
        const static UUID16 getTypeHash(const T& obj) {
            const std::type_index& typeInf = typeid(obj);
            auto pair = getTypePair(typeInf);
            return pair.first;
        }

        static void addType(Type& type, const size_t size, const std::pair<const UUID16, const std::string>& data);
    };
}

#define DEFINE_TYPE(TYPE) \
template<> \
inline JEngine::Type* JEngine::TypeHelpers::getType<TYPE>() { \
    static JEngine::Type type; \
    JEngine::TypeHelpers::addType(type, sizeof(TYPE), JEngine::TypeHelpers::getTypePair(typeid(TYPE)));\
    return &type;\
}\
