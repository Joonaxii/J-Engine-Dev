#pragma once
#include <JEngine/Helpers/TypeHelpers.h>
#include <JEngine/Utility/HexStr.h>
#include <cstdint>
#include <vector>

#define FROM_BASE(TYPE, VAL) static_cast<TYPE>(VAL) 
#define BASE_TYPE(TYPE) std::underlying_type_t<TYPE>
#define TO_BASE(TYPE, VAL) static_cast<typename BASE_TYPE(TYPE)>(VAL)
#define TO_BASE_REF(TYPE, INPUT) reinterpret_cast<BASE_TYPE(TYPE)&>(INPUT)
#define TO_REF(TYPE, INPUT) reinterpret_cast<TYPE&>(INPUT)

namespace JEngine {

    template<typename T, size_t ID = 0>
    struct EnumNames {
        static inline constexpr bool IsBitField{ false };
        static inline constexpr BASE_TYPE(T) MinValue{};
        static inline constexpr size_t Count{};
        static inline constexpr std::string_view Names[];

        static constexpr std::string_view getEnumName(T value) {
            auto bVal = TO_BASE(T, value);

            BASE_TYPE(T) index = 0;
            if (IsBitField) {
                if (!Math::isPowerOf2(bVal)) {
                    return std::string_view{};
                }
                index = BASE_TYPE(T)(Math::findFirstLSB(bVal)) - BASE_TYPE(T)(MinValue);
            }
            else {
                index = bVal - MinValue;
            }

            if (index < 0 || index >= Count) { return std::string_view{}; }
            return Names[index];
        }

        static std::string_view getEnumName(const void* value) {
            return value ? getEnumName(*reinterpret_cast<const T*>(value)) : NullRawLE;
        }
     
        static constexpr bool isNoName(std::string_view name) {
            return name.length() == 0;
        }
        static constexpr bool noDraw(size_t index) {
            return index >= Count || isNoName(Names[index]);
        }
        static constexpr bool getNextValidIndex(size_t& index) {
            if (Count < 1) { return true; }

            size_t original = index;
            while (noDraw(index)) {
                index++;
                if (index >= Count) {
                    index = 0;
                }
                if (index == original) { break; }
            }
            return index != original;
        }
    };

    namespace Enum {
        typedef std::string_view(*GetEnumName)(const void* value);

        template<typename T, size_t ID = 0>
        FORCE_INLINE std::string_view nameOf_Void(const void* value) {
            return EnumNames<T, ID>::getEnumName(value);
        }

        template<typename T, size_t ID = 0>
        FORCE_INLINE constexpr std::string_view nameOf(const T& value) {
            return EnumNames<T, ID>::getEnumName(value);
        }

        template<typename T, size_t ID = 0>
        FORCE_INLINE const char* nameOf_CStr(const T& value) {
            return EnumNames<T, ID>::getEnumName(value).data();
        }
    }
}

#define CREATE_ENUM_OPERATORS_SELF(TYPE) \
FORCE_INLINE constexpr TYPE operator~(TYPE lhs) { return FROM_BASE(TYPE, ~TO_BASE(TYPE, lhs)); } \
FORCE_INLINE constexpr TYPE operator^(TYPE lhs, TYPE rhs) { return FROM_BASE(TYPE, TO_BASE(TYPE, lhs) ^ TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE operator|(TYPE lhs, TYPE rhs) { return FROM_BASE(TYPE, TO_BASE(TYPE, lhs) | TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE operator&(TYPE lhs, TYPE rhs) { return FROM_BASE(TYPE, TO_BASE(TYPE, lhs) & TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE& operator^=(TYPE& lhs, const TYPE& rhs) { return lhs = FROM_BASE(TYPE, TO_BASE(TYPE, lhs) ^ TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE& operator|=(TYPE& lhs, const TYPE& rhs) { return lhs = FROM_BASE(TYPE, TO_BASE(TYPE, lhs) | TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE& operator&=(TYPE& lhs, const TYPE& rhs) { return lhs = FROM_BASE(TYPE, TO_BASE(TYPE, lhs) & TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE operator-(TYPE lhs) { return FROM_BASE(TYPE, -TO_BASE(TYPE, lhs)); } \
FORCE_INLINE constexpr TYPE operator-(TYPE lhs, TYPE rhs) { return FROM_BASE(TYPE, TO_BASE(TYPE, lhs) + TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE operator+(TYPE lhs, TYPE rhs) { return FROM_BASE(TYPE, TO_BASE(TYPE, lhs) - TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE& operator-=(TYPE& lhs, const TYPE& rhs) { return lhs = FROM_BASE(TYPE, TO_BASE(TYPE, lhs) - TO_BASE(TYPE, rhs)); } \
FORCE_INLINE constexpr TYPE& operator+=(TYPE& lhs, const TYPE& rhs) { return lhs = FROM_BASE(TYPE, TO_BASE(TYPE, lhs) + TO_BASE(TYPE, rhs)); } \

#define CREATE_ENUM_OPERATORS(TYPE) \
CREATE_ENUM_OPERATORS_SELF(TYPE)\
FORCE_INLINE constexpr bool operator!=(TYPE lhs, BASE_TYPE(TYPE) rhs) { return TO_BASE(TYPE, lhs) != rhs; } \
FORCE_INLINE constexpr bool operator==(TYPE lhs, BASE_TYPE(TYPE) rhs) { return TO_BASE(TYPE, lhs) == rhs; } \
FORCE_INLINE constexpr bool operator!=(BASE_TYPE(TYPE) lhs,TYPE rhs) { return lhs != TO_BASE(TYPE, rhs); } \
FORCE_INLINE constexpr bool operator==(BASE_TYPE(TYPE) lhs,TYPE rhs) { return lhs == TO_BASE(TYPE, rhs); } \
FORCE_INLINE constexpr TYPE operator^(TYPE lhs, BASE_TYPE(TYPE) rhs) { return FROM_BASE(TYPE, TO_BASE(TYPE, lhs) ^ rhs); } \
FORCE_INLINE constexpr TYPE operator|(TYPE lhs, BASE_TYPE(TYPE) rhs) { return FROM_BASE(TYPE, TO_BASE(TYPE, lhs) | rhs); } \
FORCE_INLINE constexpr TYPE operator&(TYPE lhs, BASE_TYPE(TYPE) rhs) { return FROM_BASE(TYPE, TO_BASE(TYPE, lhs) & rhs); } \
FORCE_INLINE constexpr BASE_TYPE(TYPE) operator^(BASE_TYPE(TYPE) lhs, TYPE rhs) { return lhs ^ TO_BASE(TYPE, rhs); } \
FORCE_INLINE constexpr BASE_TYPE(TYPE) operator|(BASE_TYPE(TYPE) lhs, TYPE rhs) { return lhs | TO_BASE(TYPE, rhs); } \
FORCE_INLINE constexpr BASE_TYPE(TYPE) operator&(BASE_TYPE(TYPE) lhs, TYPE rhs) { return lhs & TO_BASE(TYPE, rhs); } \
FORCE_INLINE constexpr bool operator!(TYPE lhs) { return TO_BASE(TYPE, lhs) == 0; } \
FORCE_INLINE TYPE& operator^=(TYPE& lhs, TYPE rhs) { return lhs = lhs ^ rhs; } \
FORCE_INLINE TYPE& operator&=(TYPE& lhs, TYPE rhs) { return lhs = lhs & rhs; } \
FORCE_INLINE TYPE& operator|=(TYPE& lhs, TYPE rhs) { return lhs = lhs | rhs; } \
FORCE_INLINE TYPE& operator^=(TYPE& lhs, BASE_TYPE(TYPE) rhs) { return lhs = lhs ^ FROM_BASE(TYPE, rhs); } \
FORCE_INLINE TYPE& operator&=(TYPE& lhs, BASE_TYPE(TYPE) rhs) { return lhs = lhs & FROM_BASE(TYPE, rhs); } \
FORCE_INLINE TYPE& operator|=(TYPE& lhs, BASE_TYPE(TYPE) rhs) { return lhs = lhs | FROM_BASE(TYPE, rhs); } \
FORCE_INLINE uint64_t& operator^=(BASE_TYPE(TYPE)& lhs, TYPE rhs) { return lhs ^= TO_BASE(TYPE, rhs); } \
FORCE_INLINE uint64_t& operator&=(BASE_TYPE(TYPE)& lhs, TYPE rhs) { return lhs &= TO_BASE(TYPE, rhs); } \
FORCE_INLINE uint64_t& operator|=(BASE_TYPE(TYPE)& lhs, TYPE rhs) { return lhs |= TO_BASE(TYPE, rhs); } 