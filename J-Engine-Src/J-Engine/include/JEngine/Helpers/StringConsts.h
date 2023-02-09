#pragma once
#include <cstring>
#include <cctype>

namespace JEngine {
    constexpr char WHITE_CHARS[] = " \t\r\n\f\v";
    constexpr size_t WHITE_CHAR_LEN = sizeof(WHITE_CHARS) - 1;

    inline bool isWhiteSpace(const char c) { return c == '\0' || std::isspace(c); }
}