#pragma once
#include <JEngine/Helpers/StringExtensions.h>
#include <JEngine/Cryptography/FAH16.h>
#include <typeindex>
#include <vector>
#include <string>
#define NAMEOF(name) #name

namespace JEngine {
    struct Type {
        std::string name;
        size_t size;
        FAH16 hash;

        Type() : name(""), hash(), size(-1) {}
    };

    class TypeHelpers {
    public:
        template<typename T> static Type* getType();
        static std::vector<Type*>& TypeHelpers::getTypes();

        const static std::pair<const FAH16, const std::string> getTypePair(const std::type_index& indx) {
            std::string tname = indx.name();
            int ind = Helpers::indexOf(tname, ' ');
            if (ind > -1) {
                tname.erase(0, ind + 1);
            }

            FAH16 hash;
            hash.computeHash(tname.c_str(), tname.length());
            return std::pair<const FAH16, const std::string>(hash, tname);
        }

        const static Type* getTypeByHash(const FAH16& hash) {
            auto& types = getTypes();
            for (size_t i = 0; i < types.size(); i++) {
                if (types[i]->hash == hash) { return types[i]; }
            }
            return nullptr;
        }

        const static std::string getTypeName(const FAH16& id) {
            auto type = getTypeByHash(id);
            return type ? type->name : "";
        }

        template<typename T>
        const static std::string getTypeName() {
            Type* type = JEngine::getType<T>();
            return type ? type->name : "";
        }

        template<typename T>
        const static std::string getTypeName(const T& obj) {
            const std::type_index& typeInf = typeid(obj);
            return getTypePair(typeInf).second;
        }

        template<typename T>
        const static FAH16 getTypeHash() {
            Type* type = JEngine::getType<T>();
            return type ? type->hash : FAH16();
        }

        template<typename T>
        const static FAH16 getTypeHash(const T& obj) {
            const std::type_index& typeInf = typeid(obj);
            auto pair = getTypePair(typeInf);
            return pair.first;
        }

        static void addType(Type& type, const size_t size, const std::pair<const FAH16, const std::string>& data);
    };
}

#define DEFINE_TYPE(TYPE) \
template<> \
inline JEngine::Type* JEngine::TypeHelpers::getType<TYPE>() { \
    static JEngine::Type type; \
    JEngine::TypeHelpers::addType(type, sizeof(TYPE), JEngine::TypeHelpers::getTypePair(typeid(TYPE)));\
    return &type;\
}\
