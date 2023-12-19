#pragma once
#include <JEngine/Helpers/TypeHelpers.h>
#include <cstdint>
#include <vector>

namespace JEngine {
    template<typename T>
    struct EnumNames {
        static constexpr int32_t Count{0};

        static bool isNoName(const char* name) {
            return strlen(name) == 0;
        }

        static const char** getEnumNames() {
            return nullptr;
        }

        static const char* getEnumName(T value) {
            int32_t index = int32_t(value);
            auto names = getEnumNames();
            if (index < 0 || index >= Count || !names) { return ""; }
            return names[index];
        }

        static bool noDraw(int32_t index);
        static bool getNextValidIndex(int32_t& index);
    };

    template<typename T>
    bool EnumNames<T>::noDraw(int32_t index) {
        auto names = getEnumNames();
        if (index < 0 || index >= Count || !names) { return true; }
        return isNoName(names[index]);
    }

    template<typename T>
    inline bool EnumNames<T>::getNextValidIndex(int32_t& index) {
        auto names = getEnumNames();
        if (!names) { return true; }

        int32_t original = index;
        if (index < 0 || index >= Count || noDraw(index)) {
            while (true) {
                index++;
                if (index >= Count) {
                    index = 0;
                }
                if (index == original) { break; }

                if (!noDraw(index)) { break; }
            }
        }
        return index != original;
    }

    struct EnumType {
        Type* type = nullptr;
        const char**(*getNames)() = nullptr;
        bool(*isNoDraw)(int32_t index) = nullptr;
        int32_t count{0};
    };

    class Enums {
    public:
        template<typename T> static EnumType* getEnum() {
            static EnumType defaultEnum;
            return &defaultEnum;
        }
        static std::vector<EnumType*>& getEnums();

        template<typename T>
        static void addEnum(JEngine::EnumType* enm, Type* type) {
            if (enm->type != nullptr) { return; }
            enm->type = type;
            enm->getNames = EnumNames<T>::getEnumNames;
            enm->isNoDraw = EnumNames<T>::noDraw;
            enm->count = EnumNames<T>::Count;
            getEnums().push_back(enm);
        }
    };
}

#define CREATE_ENUM_OPERATORS_SELF(TYPE) \
inline constexpr TYPE operator~(const TYPE lhs) { return static_cast<TYPE>(~static_cast<uint64_t>(lhs)); } \
inline constexpr TYPE operator^(const TYPE lhs, const TYPE rhs) { return static_cast<TYPE>(static_cast<uint64_t>(lhs) ^ static_cast<uint64_t>(rhs)); } \
inline constexpr TYPE operator|(const TYPE lhs, const TYPE rhs) { return static_cast<TYPE>(static_cast<uint64_t>(lhs) | static_cast<uint64_t>(rhs)); } \
inline constexpr TYPE operator&(const TYPE lhs, const TYPE rhs) { return static_cast<TYPE>(static_cast<uint64_t>(lhs) & static_cast<uint64_t>(rhs)); } \
inline TYPE& operator^=(TYPE& lhs, const TYPE& rhs) { return lhs = static_cast<TYPE>(static_cast<uint64_t>(lhs) ^ static_cast<uint64_t>(rhs)); } \
inline TYPE& operator|=(TYPE& lhs, const TYPE& rhs) { return lhs = static_cast<TYPE>(static_cast<uint64_t>(lhs) | static_cast<uint64_t>(rhs)); } \
inline TYPE& operator&=(TYPE& lhs, const TYPE& rhs) { return lhs = static_cast<TYPE>(static_cast<uint64_t>(lhs) & static_cast<uint64_t>(rhs)); } \


#define CREATE_ENUM_OPERATORS(TYPE) \
inline constexpr bool operator!=(const TYPE lhs, const int32_t rhs) { return static_cast<int32_t>(lhs) != rhs; } \
inline constexpr bool operator==(const TYPE lhs, const int32_t rhs) { return static_cast<int32_t>(lhs) == rhs; } \
inline constexpr bool operator!=(const int32_t lhs, const TYPE rhs) { return static_cast<int32_t>(rhs) != lhs; } \
inline constexpr bool operator==(const int32_t lhs, const TYPE rhs) { return static_cast<int32_t>(rhs) == lhs; } \
inline constexpr TYPE operator^(const TYPE lhs, const int32_t rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) ^ rhs); } \
inline constexpr TYPE operator|(const TYPE lhs, const int32_t rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) | rhs); } \
inline constexpr TYPE operator&(const TYPE lhs, const int32_t rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) & rhs); } \
inline constexpr TYPE operator^(const TYPE lhs, const uint64_t rhs) { return static_cast<TYPE>(static_cast<uint64_t>(lhs) ^ rhs); } \
inline constexpr TYPE operator|(const TYPE lhs, const uint64_t rhs) { return static_cast<TYPE>(static_cast<uint64_t>(lhs) | rhs); } \
inline constexpr TYPE operator&(const TYPE lhs, const uint64_t rhs) { return static_cast<TYPE>(static_cast<uint64_t>(lhs) & rhs); } \
inline constexpr int32_t operator^(const int32_t lhs, const TYPE rhs) { return (lhs ^ static_cast<int32_t>(rhs)); } \
inline constexpr int32_t operator|(const int32_t lhs, const TYPE rhs) { return (lhs | static_cast<int32_t>(rhs)); } \
inline constexpr int32_t operator&(const int32_t lhs, const TYPE rhs) { return (lhs & static_cast<int32_t>(rhs)); } \
inline constexpr uint64_t operator^(const uint64_t lhs, const TYPE rhs) { return (lhs ^ static_cast<uint64_t>(rhs)); } \
inline constexpr uint64_t operator|(const uint64_t lhs, const TYPE rhs) { return (lhs | static_cast<uint64_t>(rhs)); } \
inline constexpr uint64_t operator&(const uint64_t lhs, const TYPE rhs) { return (lhs & static_cast<uint64_t>(rhs)); } \
inline constexpr bool operator&(const TYPE lhs, const bool rhs) { return (static_cast<int32_t>(lhs) != 0) & rhs; } \
inline constexpr bool operator|(const TYPE lhs, const bool rhs) { return (static_cast<int32_t>(lhs) != 0) | rhs; } \
inline constexpr bool operator&(const bool lhs, const TYPE rhs) { return (static_cast<int32_t>(rhs) != 0) & lhs; } \
inline constexpr bool operator|(const bool lhs, const TYPE rhs) { return (static_cast<int32_t>(rhs) != 0) | lhs; } \
inline constexpr bool operator!(const TYPE lhs) { return !static_cast<int32_t>(lhs); } \
inline const TYPE& operator^=(TYPE& lhs, const TYPE rhs) { return lhs = lhs ^ rhs; } \
inline const TYPE& operator&=(TYPE& lhs, const TYPE rhs) { return lhs = lhs & rhs; } \
inline const TYPE& operator|=(TYPE& lhs, const TYPE rhs) { return lhs = lhs | rhs; } \
inline const TYPE& operator^=(TYPE& lhs, const int32_t rhs) { return lhs = lhs ^ rhs; } \
inline const TYPE& operator&=(TYPE& lhs, const int32_t rhs) { return lhs = lhs & rhs; } \
inline const TYPE& operator|=(TYPE& lhs, const int32_t rhs) { return lhs = lhs | rhs; } \
inline const int32_t& operator^=(int32_t& lhs, const TYPE rhs) { return lhs = static_cast<int32_t>(lhs ^ rhs); } \
inline const int32_t& operator&=(int32_t& lhs, const TYPE rhs) { return lhs = static_cast<int32_t>(lhs & rhs); } \
inline const int32_t& operator|=(int32_t& lhs, const TYPE rhs) { return lhs = static_cast<int32_t>(lhs | rhs); } \
inline const uint64_t& operator^=(uint64_t& lhs, const TYPE rhs) { return lhs = static_cast<uint64_t>(lhs ^ rhs); } \
inline const uint64_t& operator&=(uint64_t& lhs, const TYPE rhs) { return lhs = static_cast<uint64_t>(lhs & rhs); } \
inline const uint64_t& operator|=(uint64_t& lhs, const TYPE rhs) { return lhs = static_cast<uint64_t>(lhs | rhs); } \
CREATE_ENUM_OPERATORS_SELF(TYPE);\


#define FROM_BASE(TYPE, VAL) static_cast<TYPE>(VAL) 
#define TO_BASE(TYPE, VAL) static_cast<typename const std::underlying_type_t<TYPE>>(VAL)


template<typename T>
struct ValidatedEnum {
    static const JEngine::EnumType* Value;
};

template<typename T>
inline const JEngine::EnumType* ValidatedEnum<T>::Value = nullptr;

#define DEFINE_ENUM(TYPE) \
template<> \
inline JEngine::EnumType* JEngine::Enums::getEnum<TYPE>() { \
  static JEngine::EnumType enm; \
  JEngine::Enums::addEnum<TYPE>(&enm, JEngine::TypeHelpers::getType<TYPE>()); \
  return &enm; \
}\

#define VALIDATE_ENUM(x) template<> \
inline const JEngine::EnumType* ValidatedEnum<x>::Value = JEngine::Enums::getEnum<x>(); \

#define REGISTER_ENUM(x) \
DEFINE_TYPE(x); \
DEFINE_ENUM(x); \
VALIDATE_ENUM(x); \

