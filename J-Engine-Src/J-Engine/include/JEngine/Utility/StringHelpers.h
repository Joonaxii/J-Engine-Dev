#pragma once
#include <string>
#include <cstdint>
#include <JEngine/Utility/Span.h>

namespace JEngine::Helpers {

    bool validateUtf8(uint32_t& state, const void* data, size_t size);

    size_t strIIndexOf(const std::string_view& strA, const std::string_view& strB);

    bool strIEquals(const char* strA, const size_t lenA, const char* strB, const size_t lenB);
    bool strIEquals(const std::string_view& strA, const std::string_view& strB);

    bool strEquals(const char* strA, const size_t lenA, const char* strB, const size_t lenB);
    bool strEquals(const std::string_view& strA, const std::string_view& strB);


    int32_t strCmpNoCase(const char* a, const char* b);
    bool equalsNoCase(const Span<char>& a, const Span<char>& b);
    bool equalsNoCase(const ConstSpan<char>& a, const ConstSpan<char>& b);

    int32_t findNotIndexOf(const std::string& str, char c, size_t offset, size_t len);
    int32_t findNotIndexOf(const char* str, size_t length, char c, size_t offset, size_t len);

    int32_t findNotIndexOf(const char* str, size_t length, const char* c, size_t offset, size_t len);


    int32_t indexOf(const char* str, const size_t length, const char c);
    int32_t indexOf(const std::string& str, const char c);

    int32_t indexOf(const char* str, const size_t length, const std::string& search);
    int32_t indexOf(const std::string& str, const std::string& search);

    inline constexpr uint8_t hexToUI8(char c) {
        if (c >= '0' && c <= '9') { return (c - '0'); }
        if (c >= 'A' && c <= 'F') { return (c - 'A') + 10; }
        if (c >= 'a' && c <= 'f') { return (c - 'a') + 10; }
        return 0;
    }

    inline constexpr uint8_t hexToUI8(char a, char b) {
        return hexToUI8(a) | (hexToUI8(b) << 4);
    }

    template<typename T, size_t size = sizeof(T) << 1>
    bool tryParseHex(ConstSpan<char>& span, T& out, const bool invert = false);

    template<typename T, size_t size = sizeof(T) << 1>
    bool tryParseHex(Span<char>& span, T& out, const bool invert = false);

    uint32_t parseHexColorInt(const std::string& str, const int start, const int length);
    void parseString(const std::string& input, std::string& output);

    void strSplit(const std::string& str, std::vector<std::string>& strs, const std::string& delim, const bool ignoreEmpty = true);
    void strSplit(const std::string& str, std::vector<std::string_view>& strs, const std::string& delim, const bool ignoreEmpty = true);

    void addCommandArg(std::string& cmd, const std::string& arg);
    void addCommandArgString(std::string& cmd, const std::string& arg);

    template<typename T>
    bool tryParseHex(const ConstSpan<char>& span, T& out, const bool invert) {
        static constexpr size_t RSIZE = sizeof(T) << 1;
        if (span.length() < RSIZE) { return false; }
        Span<uint8_t> sp(reinterpret_cast<uint8_t*>(&out), sizeof(T));

        if (invert) {
            for (size_t i = 0, j = sp.length(); i < RSIZE; i += 2, j--) {
                sp[j] = hexToUI8(span[i + 0], span[i + 1]);
            }
            return true;
        }

        for (size_t i = 0, j = 0; i < RSIZE; i += 2, j++) {
            sp[j] = hexToUI8(span[i + 0], span[i + 1]);
        }
        return true;
    }

    template<typename T>
    bool tryParseHex(const Span<char>& span, T& out, const bool invert) {
        return tryParseHex<T>(ConstSpan<char>(span), out, invert);
    }

    void formatDataSize(char* buffer, size_t size);

}