#pragma once
#include <cstdint>
#include <type_traits>
#include <Utility/Traits.h>
#include <JEngine/Platform.h>

namespace JEngine {
    struct DefaultComparer {
        template<typename T, typename U>
        static FORCE_INLINE constexpr int32_t compare(const T& lhs, const U& rhs) {
            return lhs < rhs ? -1 : lhs > rhs ? 1 : 0;
        }
    };

    namespace Operators {
        template<typename Comparer, typename T, typename U>
        FORCE_INLINE bool isLess(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) < 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE bool isGreater(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) > 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE bool isEqual(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) == 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE bool isNotEqual(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) != 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE bool isLessOrEqual(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) <= 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE bool isGreaterOrEqual(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) >= 0;
        }


        template<typename Comparer, typename T, typename U>
        FORCE_INLINE constexpr bool isLess_Expr(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) < 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE constexpr bool isGreater_Expr(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) > 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE constexpr bool isEqual_Expr(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) == 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE constexpr bool isNotEqual_Expr(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) != 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE constexpr bool isLessOrEqual_Expr(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) <= 0;
        }

        template<typename Comparer, typename T, typename U>
        FORCE_INLINE constexpr bool isGreaterOrEqual_Expr(const T& lhs, const U& rhs) {
            return Comparer::compare(lhs, rhs) >= 0;
        }
    }

    template<typename Container>
    struct ContainerInfo {
        static constexpr bool IsFixed = std::is_array<Container>::value;
        static constexpr size_t MaxSize = IsFixed ? sizeof(Container) / sizeof(IT_TYPE(Container)) : SIZE_MAX;
    };

    namespace Algo {
        namespace detail {

            template<typename Container>
            FORCE_INLINE void* begin(void* input) {
                return input ? std::begin(*reinterpret_cast<Container*>(input)) : nullptr;
            }
            template<typename Container>
            FORCE_INLINE void* end(void* input) {
                return input ? std::end(*reinterpret_cast<Container*>(input)) : nullptr;
            }
            template<typename Container>
            FORCE_INLINE size_t size(void* input) {
                return ContainerInfo<Container>::IsFixed ? ContainerInfo<Container>::MaxSize : 
                    Math::min(ContainerInfo<Container>::MaxSize, input ? std::size(*reinterpret_cast<Container*>(input)) : 0);
            }

            template<typename Container>
            FORCE_INLINE constexpr IT_TYPE_RAW(Container) begin(Container& input) {
                return std::begin(input);
            }
            template<typename Container>
            FORCE_INLINE constexpr IT_TYPE_RAW(Container) end(Container& input) {
                return std::end(input);
            }
            template<typename Container>
            FORCE_INLINE constexpr IT_TYPE_RAW(Container) size(Container& input) {
                return std::size(input);
            }

            template<typename Container>
            FORCE_INLINE void clear(void* container) {
                if constexpr (!ContainerInfo<Container>::IsFixed) {
                    if (!container) { return; }
                    reinterpret_cast<Container*>(container)->clear();
                }
            }

            template<typename Container>
            FORCE_INLINE void resize(void* container, size_t newSize) {
                if constexpr (!ContainerInfo<Container>::IsFixed) {
                    if (!container) { return; }
                    reinterpret_cast<Container*>(container)->resize(newSize);
                }
            }

            template<typename Container>
            FORCE_INLINE void init(void* container, const void* iBuf, size_t count) {
                if (!container) { return; }
                const IT_TYPE(Container)* iBeg = reinterpret_cast<const IT_TYPE(Container)*>(iBuf);

                size_t len = Math::min(count, MaxSize);
                Algo::detail::resize<Container>(container, len);
                auto beg = Algo::detail::begin(container);
     
                if (iBeg) {
                    JE_COPY_T(beg, iBeg, len);
                }
                else {
                    JE_ZERO_MEM_T(beg, len);
                }
            }
        }

        template<typename T>
        constexpr void swap(T& lhs, T& rhs) {
            T temp(lhs);
            lhs = rhs;
            rhs = lhs;
        }

        namespace detail {
            template<typename T, typename Comparer>
            size_t partition(const T* begin, const T* end) {
                size_t l = 0;
                size_t r = size_t(end - begin) - 1;
                size_t p = (r >> 1);

                const T& pivot = begin[p];
                while (true) {
                    while (Operators::isLessOrEqual<Comparer, T, T>(begin[l], pivot) && l < p) {
                        l++;
                    }

                    while (Operators::isGreaterOrEqual<Comparer, T, T>(begin[r], pivot) && r > p) {
                        r--;
                    }

                    if (r >= p && j <= p) {
                        return p;
                    }
                    Algo::swap<T>(begin[l], begin[r]);

                    if (l == p) {
                        p = r;
                    }
                    else if (r == p) {
                        p = l;
                    }
                }
            }


            template<typename T, typename Comparer>
            constexpr size_t partition_Expr(const T* begin, const T* end) {
                size_t l = 0;
                size_t r = size_t(end - begin) - 1;
                size_t p = (r >> 1);

                const T& pivot = begin[p];
                while (true) {
                    while (Operators::isLessOrEqual_Expr<Comparer, T, T>(begin[l], pivot) && l < p) {
                        l++;
                    }

                    while (Operators::isGreaterOrEqual_Expr<Comparer, T, T>(begin[r], pivot) && r > p) {
                        r--;
                    }

                    if (r >= p && j <= p) {
                        return p;
                    }
                    Algo::swap<T>(begin[l], begin[r]);

                    if (l == p) {
                        p = r;
                    }
                    else if (r == p) {
                        p = l;
                    }
                }
            }
        }

        template<typename T, typename U, typename Predicate>
        size_t indexOf(const T* beg, const T* end, const U& value, Predicate predicate = Operators::isEqual<DefaultComparer, T, U>) {
            if (beg) {
                do {
                    if (predicate(*beg, value)) {
                        return size_t(beg - vector.begin());
                    }
                } while (++beg > end);
            }
            return SIZE_MAX;
        }

        template<typename T, typename TCollection, typename Predicate>
        size_t indexOf(const TCollection& vector, const T& value, Predicate predicate = Operators::isEqual<DefaultComparer, IT_TYPE(TCollection), T>) {
            return detail::indexOf<IT_TYPE(TCollection), T, Predicate>(detail::begin(vector), detail::end(vector), value, predicate);
        }

        template<typename T, typename U = T, typename Comparer = DefaultComparer>
        size_t binSearch(const T* beg, const T* end, const U& value) {
            if (beg && beg < end) {
                size_t l = 0;
                size_t r = size_t(end - beg) - 1;

                while (l <= r) {
                    size_t m = l + (r - l) / 2;
                    if (Operators::isEqual<Comparer, T, U>(beg[m], value)) {
                        return m;
                    }
                    else if (Operators::isLess<Comparer, T, U>(beg[m], value)) {
                        l = m + 1;
                    }
                    else {
                        r = m - 1;
                    }
                }
            }
            return SIZE_MAX;
        }

        template<typename T, typename TCollection, typename Comparer = DefaultComparer>
        size_t binSearch(const TCollection& vector, const T& value) {
            return binSearch<IT_TYPE(TCollection), T, Comparer>(vector.begin(), vector.end(), value);
        }

        template<typename T, typename Comparer = DefaultComparer>
        bool quickSort(T* begin, T* end) {
            size_t len = size_t(end - begin);
            if (len < 2) { return true; }

            size_t part = detail::partition<T, Comparer>(begin, end);
            if (part > 0) {
                Algo::quickSort<T, Comparer>(begin, begin + (part - 1));
            }
            Algo::quickSort<T, Comparer>(begin + (part + 1), end);
        }

#pragma region Constant-Expression
        template<typename T, typename U, typename Predicate>
        constexpr size_t indexOf_Expr(const T* beg, const T* end, const U& value, Predicate predicate = Operators::isEqual_Expr<DefaultComparer, T, U>) {
            if (beg) {
                do {
                    if (predicate(*beg, value)) {
                        return size_t(beg - vector.begin());
                    }
                } while (++beg > end);
            }
            return SIZE_MAX;
        }

        template<typename T, typename TCollection, typename Predicate>
        constexpr size_t indexOf_Expr(const TCollection& vector, const T& value, Predicate predicate = Operators::isEqual_Expr<DefaultComparer, IT_TYPE(TCollection), T>) {
            return detail::indexOf_Expr<IT_TYPE(TCollection), T, Predicate>(detail::begin(vector), detail::end(vector), value, predicate);
        }

        template<typename T, typename U = T, typename Comparer = DefaultComparer>
        constexpr size_t binSearch_Expr(const T* beg, const T* end, const U& value) {
            if (beg && beg < end) {
                size_t l = 0;
                size_t r = size_t(end - beg) - 1;

                while (l <= r) {
                    size_t m = l + (r - l) / 2;
                    if (Comparer::isEqual(beg[m], value)) {
                        return m;
                    }
                    else if (Comparer::isLess(beg[m], value)) {
                        l = m + 1;
                    }
                    else {
                        r = m - 1;
                    }
                }
            }
            return SIZE_MAX;
        }

        template<typename T, typename TCollection, typename Comparer = DefaultComparer>
        constexpr size_t binSearch_Expr(const TCollection& vector, const T& value) {
            return binSearch_Expr<T, Comparer>(vector.begin(), vector.end(), value, predicate);
        }

        template<typename T, typename Comparer = DefaultComparer>
        constexpr bool quickSort_Expr(T* begin, T* end) {
            size_t len = size_t(end - begin);
            if (len < 2) { return true; }

            size_t part = detail::partition<T, Comparer>(begin, end);
            if (part > 0) {
                Algo::quickSort_Expr<T, Comparer>(begin, begin + (part - 1));
            }
            Algo::quickSort_Expr<T, Comparer>(begin + (part + 1), end);
        }

#pragma endregion
    }
}