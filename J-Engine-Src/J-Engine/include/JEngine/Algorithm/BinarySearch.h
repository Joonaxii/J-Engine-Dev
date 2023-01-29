#pragma once
#include <cstdint>
#include <functional>

namespace JEngine {
    template<typename T1, typename T2>
    const int32_t defBinComparison(T1& a, const T2& b) {
        if (a == b) { return 0; }
        return a > b ? -1 : 1;
    }

    template<typename T1, typename T2>
    const int32_t binSearch(T1* input, const int start, const int length, const T2& value, const int32_t(*getState)(T1&, const T2&)) {
        int l = start;
        int r = length - 1;

        while (l <= r) {
            int m = l + ((r - l) / 2);
            auto res = getState(input[m], value);

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

    template<typename T1, typename T2>
    const int32_t binSearch(T1* input, const int start, const int length, const T2& value) {
        return binSearch(input, start, length, value, &defBinComparison);
    }


    template<typename T1, typename T2>
    const int32_t defBinComparisonConst(const T1& a, const T2& b) {
        if (a == b) { return 0; }
        return a > b ? -1 : 1;
    }

    template<typename T1, typename T2>
    const int32_t binSearchConst(const T1* input, const int start, const int length, const T2& value, const int32_t(*getState)(const T1&, const T2&)) {
        int l = start;
        int r = length - 1;

        while (l <= r) {
            int m = l + ((r - l) / 2);
            auto res = getState(input[m], value);

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

    template<typename T1, typename T2>
    const int32_t binSearchConst(const T1* input, const int start, const int length, const T2& value) {
        return binSearchConst(input, start, length, value, [](const T1& a, const T2& b) { return defBinComparisonConst(a, b); });
    }
}