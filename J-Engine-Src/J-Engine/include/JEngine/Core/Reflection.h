#pragma once
#include <cstdint>
#include <string>
#include <string_view>

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define FOR_EACH_1(what, o, i, x)         \
    what(o, i, x)

#define FOR_EACH_2(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_1(what, o, i, __VA_ARGS__)

#define FOR_EACH_3(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_2(what, o, i, __VA_ARGS__)

#define FOR_EACH_4(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_3(what, o, i,  __VA_ARGS__)

#define FOR_EACH_5(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_4(what, o, i,  __VA_ARGS__)

#define FOR_EACH_6(what, x, ...)          \
  what(o, i, x);                          \
  FOR_EACH_5(what, o, i, __VA_ARGS__)

#define FOR_EACH_7(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_6(what, o, i,  __VA_ARGS__)

#define FOR_EACH_8(what, o, i, x, ...)    \
    what(o, i, x);                        \
    FOR_EACH_7(what, o, i,  __VA_ARGS__)

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, ...) CONCATENATE(FOR_EACH_, N)(what, __VA_ARGS__)
#define FOR_EACH(what, o, i, ...) \
FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, o, i, __VA_ARGS__)