#pragma once
#include <JEngine/Helpers/StringExtensions.h>
#include <JEngine/Cryptography/FAH16.h>
#include <typeindex>
#include <vector>

namespace JEngine {
    struct HashPair {
        FAH16 hash{};
        const char* typeName = nullptr;

        HashPair(): hash(), typeName(nullptr) {}
        HashPair(const FAH16& hash, const char* typeName) : hash(hash), typeName(typeName){}
    };

    struct TypeData {
        const HashPair hashData;
        const int32_t size;

        TypeData(const HashPair& hash, const int32_t size) : hashData(hash), size(size) {}

        const char* getName() const { return hashData.typeName; }
    };

    class TypeHelpers {
    private:
        static std::unordered_map<FAH16, const char*, std::hash<FAH16>> HashLUT;

    public:
        template<typename T> static TypeData* getType();
        static std::vector<TypeData>& TypeHelpers::getTypes();

        static int32_t indexOfHash(const FAH16& hash);

        static void getTypePair(const std::type_index& indx, HashPair& pair) {
            ConstSpan<char> inputName(indx.name(), strlen(indx.name()));
            int32_t ind = inputName.indexOf(' ');
            inputName = ind > -1 ? inputName.slice(ind) : inputName;

            FAH16 hash;
            hash.computeHash(inputName);
            auto& find = HashLUT.find(hash);

            pair.hash = hash;
            if (find == HashLUT.end()) {
                char* str = reinterpret_cast<char*>(malloc(inputName.length() + 1));
                str[inputName.length()] = 0;
                inputName.copyTo(str);

                HashLUT.insert(std::make_pair(hash, str));
                pair.typeName = str;
                return;
            }
            pair.typeName = find->second;
        }

        static const TypeData* getTypeByHash(const FAH16& hash) {
            auto& types = getTypes();
            for (size_t i = 0; i < types.size(); i++) {
                if (types[i].hashData.hash  == hash) { return &types[i]; }
            }
            return nullptr;
        }

        template<typename T>
        static const char* getTypeName() {
            TypeData* type = JEngine::getType<T>();
            return type ? type->getName() : "";
        }

        template<typename T>
        static const char* getTypeName(const T& obj) {
            const std::type_index& typeInf = typeid(obj);
            return getTypePair(typeInf).second;
        }

        template<typename T>
        static FAH16 getTypeHash() {
            TypeData* type = JEngine::getType<T>();
            return type ? type->hashData : FAH16();
        }

        template<typename T>
        static FAH16 getTypeHash(const T& obj) {
            const std::type_index& typeInf = typeid(obj);
            auto pair = getTypePair(typeInf);
            return pair.first;
        }

        static void addType(const HashPair& data, const int32_t size);
    };
}

#define DEFINE_TYPE(TYPE) \
template<> \
inline JEngine::TypeData* JEngine::TypeHelpers::getType<TYPE>() { \
    static JEngine::TypeData type; \
    JEngine::HashPair pair;\
    JEngine::TypeHelpers::getTypePair(typeid(TYPE), pair);\
    JEngine::TypeHelpers::addType(pair, int32_t(sizeof(TYPE)));\
    return &type;\
}\
