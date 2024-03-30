#pragma once
#include <type_traits>
#include <iterator>

namespace JEngine {
    namespace Traits {
        template <typename T, typename = void>
        struct is_iterable : std::false_type {};

        template <typename T>
        struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T&>())),
            decltype(std::end(std::declval<T&>()))
        >
        > : std::true_type {};

        template <typename T>
        constexpr bool is_iterable_v = is_iterable<T>::value;

        template <typename R, typename... Args>
        constexpr R return_type_of(R(*)(Args...));
    }
}

#define IT_TYPE_OF(ITERATOR) std::remove_pointer_t<TYPE_OF(std::begin(std::declval<TYPE_OF(ITERATOR)&>()))> 
#define IT_TYPE(TYPE) std::remove_pointer_t<TYPE_OF(std::begin(std::declval<TYPE&>()))> 
#define IT_TYPE_OF_RAW(ITERATOR) TYPE_OF(std::begin(std::declval<TYPE_OF(ITERATOR)&>())) 
#define IT_TYPE_RAW(TYPE) TYPE_OF(std::begin(std::declval<TYPE&>())) 