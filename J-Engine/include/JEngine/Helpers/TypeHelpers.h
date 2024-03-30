#pragma once
#include <JEngine/Utility/DataUtilities.h>
#include <vector>
#include <string_view>
//#define NAMEOF(name) #name

//namespace JEngine {
//    struct FieldInfo {
//        std::string_view name{};
//        std::string_view displayName{};
//        size_t offset{};
//        size_t size{};
//
//        FieldInfo() : name{}, offset{}, size{}, displayName{}{}
//        FieldInfo(std::string_view name, std::string_view displayName, size_t offset, size_t size) : name{ name }, displayName{ displayName }, offset{ offset }, size{ size } {}
//    };
//
//    struct Type {
//        std::string_view name{};
//        uint32_t hash{};
//        size_t size{};
//        std::vector<FieldInfo> fields{};
//
//        Type() : name(""), hash(), size(SIZE_MAX) {}
//        Type(std::string_view name, uint32_t hash, size_t size) : name(name), hash(hash), size(size), fields{}{}
//    };
//
//    namespace Types {
//        template <typename T>
//        inline std::string_view getTypeName() {
//#if defined(__clang__)
//            constexpr auto prefix = std::string_view{ "[T = " };
//            constexpr auto suffix = "]";
//            constexpr auto function = std::string_view{ __PRETTY_FUNCTION__ };
//#elif defined(__GNUC__)
//            constexpr auto prefix = std::string_view{ "with T = " };
//            constexpr auto suffix = "; ";
//            constexpr auto function = std::string_view{ __PRETTY_FUNCTION__ };
//#elif defined(_MSC_VER)
//            constexpr auto prefix = std::string_view{ "getTypeName<" };
//            constexpr auto suffix = ">(";
//            constexpr auto function = std::string_view{ __FUNCSIG__ };
//#else
//# error Unsupported compiler
//#endif
//            const auto start = function.find(prefix) + prefix.size();
//            const auto end = function.find(suffix);
//            const auto size = end - start;
//            return function.substr(start, size);
//        }
//
//        template<typename T>
//        constexpr std::string_view getTypeName(const T& obj) {
//            return getTypeName<T>();
//        }
//
//        constexpr uint32_t calculateNameHash(std::string_view name) {
//            uint32_t hash = Data::calcualteCRC(&name[0], name.length());
//            return hash == 0x00 ? 1 : hash;
//        }
//
//        template<typename T>
//        inline uint32_t getTypeHash() {
//            return Data::calculateCRC<std::string_view>(getTypeName<T>());
//        }
//
//        template<typename T>
//        inline uint32_t getTypeHash(const T& obj) {
//            return calculateNameHash(getTypeName<T>());
//        }
//    }
//
//    class TypeHelpers {
//    public:
//        static std::vector<Type const*>& getTypes();
//
//        template<typename T>
//        static Type const& getType();
//
//        static Type const* getTypeByHash(uint32_t hash) {
//            if (hash == 0x00) { return nullptr; }
//            auto& types = getTypes();
//            for (size_t i = 0; i < types.size(); i++) {
//                if (types[i]->hash == hash) { return types[i]; }
//            }
//            return nullptr;
//        }
//
//        static std::string_view getTypeName(uint32_t id) {
//            auto type = getTypeByHash(id);
//            return type ? type->name : std::string_view();
//        }
//
//        template<typename T>
//        static uint32_t getTypeHash() {
//            Type const* type = JEngine::TypeHelpers::getType<T>();
//            return type ? type->hash : 0x00;
//        }
//    };
//}
////
////#define ADD_FIELD(TYPE, FIELD, F_TYPE) TYPE.fields.push_back({#FIELD, #FIELD, offsetof(F_TYPE, FIELD), sizeof(F_TYPE::FIELD)})
////#define ADD_FIELD_NAMED(TYPE, FIELD, F_TYPE, D_NAME) TYPE.fields.push_back({#FIELD, D_NAME, offsetof(F_TYPE, FIELD), sizeof(F_TYPE::FIELD)})
////
//
//#define DEFINE_TYPE(TYPE)\
//template<> inline JEngine::Type const& JEngine::TypeHelpers::getType<TYPE>() { \
        static bool init{ false }; \
        static JEngine::Type type( \
            JEngine::Types::getTypeName<TYPE>(), \
            JEngine::Types::getTypeHash<TYPE>(), sizeof(TYPE)); \
    if (!init) { \
        TYPE::bindFields(type); \
        JEngine::TypeHelpers::getTypes().push_back(&type); \
        init = true; \
    } \
    return type; \
} \
//