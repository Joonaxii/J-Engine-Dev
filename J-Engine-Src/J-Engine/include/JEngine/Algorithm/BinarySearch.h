#pragma once
#include <cstdint>
#include <functional>

namespace JEngine {
    template<typename T1, typename T2, typename T3 = int32_t>
    const T3 defBinComparison(T1& a, const T2& b) {
        if (a == b) { return T3(0); }
        return T3(a > b ? -1 : 1);
    }

    template<typename T1, typename T2, typename T3 = int32_t>
    const T3 binSearch(T1* input, const size_t start, const size_t length, const T2& value, const T3(*getState)(T1&, const T2&)) {
        int32_t l = int32_t(start);
        int32_t r = int32_t(length - 1);

        while (l <= r) {
            int32_t m = l + ((r - l) / 2);
            T3 res = getState(input[m], value);

            switch (res)
            {
            default:
                return T3(m);
            case -1:
                l = m + 1;
                break;
            case 1:
                r = m - 1;
                break;
            }
        }
        return T3(-1);
    }

    template<typename T1, typename T2, typename T3 = int32_t>
    const T3 binSearch(T1* input, const size_t start, const size_t length, const T2& value) {
        return binSearch(input, start, length, value, &defBinComparison);
    }


    template<typename T1, typename T2, typename T3 = int32_t>
    const T3 defBinComparisonConst(const T1& a, const T2& b) {
        if (a == b) { return 0; }
        return a > b ? -1 : 1;
    }

    template<typename T1, typename T2, typename T3 = int32_t>
    const T3 binSearchConst(const T1* input, const size_t start, const size_t length, const T2& value, const T3(*getState)(const T1&, const T2&)) {
        int l = start;
        int r = length - 1;

        while (l <= r) {
            int32_t m = l + ((r - l) / 2);
            T3 res = getState(input[m], value);

            switch (res)
            {
            default:
                return m;
            case -1:
                l = m + 1;
                break;
            case 1:
                r = m - 1;
                break;
            }
        }
        return -1;
    }

    template<typename T1, typename T2, typename T3 = int32_t>
    const T3 binSearchConst(const T1* input, const size_t start, const size_t length, const T2& value) {
        return binSearchConst(input, start, length, value, [](const T1& a, const T2& b) { return defBinComparisonConst(a, b); });
    }
}