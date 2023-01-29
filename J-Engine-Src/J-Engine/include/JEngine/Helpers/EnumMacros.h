#pragma once
#include <type_traits>

#define CREATE_ENUM_OPERATORS_SELF(TYPE) \
inline constexpr TYPE operator~(const TYPE lhs) { return static_cast<TYPE>(~static_cast<int32_t>(lhs)); } \
inline constexpr TYPE operator^(const TYPE lhs, const TYPE rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) ^ static_cast<int32_t>(rhs)); } \
inline constexpr TYPE operator|(const TYPE lhs, const TYPE rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) | static_cast<int32_t>(rhs)); } \
inline constexpr TYPE operator&(const TYPE lhs, const TYPE rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) & static_cast<int32_t>(rhs)); } \
inline TYPE& operator^=(TYPE& lhs, const TYPE& rhs) { return lhs = static_cast<TYPE>(static_cast<int32_t>(lhs) ^ static_cast<int32_t>(rhs)); } \
inline TYPE& operator|=(TYPE& lhs, const TYPE& rhs) { return lhs = static_cast<TYPE>(static_cast<int32_t>(lhs) | static_cast<int32_t>(rhs)); } \
inline TYPE& operator&=(TYPE& lhs, const TYPE& rhs) { return lhs = static_cast<TYPE>(static_cast<int32_t>(lhs) & static_cast<int32_t>(rhs)); } \

#define CREATE_ENUM_OPERATORS(TYPE) \
inline constexpr bool operator!=(const TYPE lhs, const int32_t rhs) { return static_cast<int32_t>(lhs) != rhs; } \
inline constexpr bool operator==(const TYPE lhs, const int32_t rhs) { return static_cast<int32_t>(lhs) == rhs; } \
inline constexpr bool operator!=(const int32_t lhs, const TYPE rhs) { return static_cast<int32_t>(rhs) != lhs; } \
inline constexpr bool operator==(const int32_t lhs, const TYPE rhs) { return static_cast<int32_t>(rhs) == lhs; } \
inline constexpr TYPE operator^(const TYPE lhs, const int32_t rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) ^ rhs); } \
inline constexpr TYPE operator|(const TYPE lhs, const int32_t rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) | rhs); } \
inline constexpr TYPE operator&(const TYPE lhs, const int32_t rhs) { return static_cast<TYPE>(static_cast<int32_t>(lhs) & rhs); } \
inline constexpr TYPE operator^(const TYPE lhs, const size_t rhs) { return static_cast<TYPE>(static_cast<size_t>(lhs) ^ rhs); } \
inline constexpr TYPE operator|(const TYPE lhs, const size_t rhs) { return static_cast<TYPE>(static_cast<size_t>(lhs) | rhs); } \
inline constexpr TYPE operator&(const TYPE lhs, const size_t rhs) { return static_cast<TYPE>(static_cast<size_t>(lhs) & rhs); } \
inline constexpr int32_t operator^(const int32_t lhs, const TYPE rhs) { return (lhs ^ static_cast<int32_t>(rhs)); } \
inline constexpr int32_t operator|(const int32_t lhs, const TYPE rhs) { return (lhs | static_cast<int32_t>(rhs)); } \
inline constexpr int32_t operator&(const int32_t lhs, const TYPE rhs) { return (lhs & static_cast<int32_t>(rhs)); } \
inline constexpr size_t operator^(const size_t lhs, const TYPE rhs) { return (lhs ^ static_cast<size_t>(rhs)); } \
inline constexpr size_t operator|(const size_t lhs, const TYPE rhs) { return (lhs | static_cast<size_t>(rhs)); } \
inline constexpr size_t operator&(const size_t lhs, const TYPE rhs) { return (lhs & static_cast<size_t>(rhs)); } \
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
inline const size_t& operator^=(size_t& lhs, const TYPE rhs) { return lhs = static_cast<size_t>(lhs ^ rhs); } \
inline const size_t& operator&=(size_t& lhs, const TYPE rhs) { return lhs = static_cast<size_t>(lhs & rhs); } \
inline const size_t& operator|=(size_t& lhs, const TYPE rhs) { return lhs = static_cast<size_t>(lhs | rhs); } \
CREATE_ENUM_OPERATORS_SELF(TYPE);\

#define FROM_BASE(TYPE, VAL) static_cast<TYPE>(VAL) 
#define TO_BASE(TYPE, VAL) static_cast<typename const std::underlying_type_t<TYPE>>(VAL)
