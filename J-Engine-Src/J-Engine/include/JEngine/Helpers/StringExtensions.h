#pragma once
#include <string>
#include <vector>
#include <JEngine/Utility/Span.h>
#include <JEngine/Utility/ConstSpan.h>
#include <JEngine/Helpers/StringConsts.h>

namespace JEngine::Helpers {

    int32_t strCmpNoCase(const char* a, const char* b);
    bool equalsNoCase(const Span<char>& a, const Span<char>& b);
    bool equalsNoCase(const ConstSpan<char>& a, const ConstSpan<char>& b);

    int32_t findNotIndexOf(const std::string& str, const char c, size_t offset, size_t len);

    int32_t indexOf(const char* str, const size_t length, const char c);
    int32_t indexOf(const std::string& str, const char c);

    int32_t indexOf(const char* str, const size_t length, const std::string& search);
    int32_t indexOf(const std::string& str, const std::string& search);

    uint8_t hexToUI8(const char c);
    uint8_t hexToUI8(const char a, const char b);

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

    template<typename T, size_t size>
    bool tryParseHex(ConstSpan<char>& span, T& out, const bool invert) {
        static constexpr size_t RSIZE = size;
        if (span.length() < RSIZE << 1) { return false; }
        Span<uint8_t> sp(reinterpret_cast<uint8_t*>(&out), size);

        if (invert) {
            for (size_t i = 0, j = sp.length(); i < RSIZE; i+=2, j--) {
                sp[j] = hexToUI8(span[i + 0], span[i + 1]);
            }
            return true;
        }

        for (size_t i = 0, j = 0; i < RSIZE; i+=2, j++) {
            sp[j] = hexToUI8(span[i + 0], span[i + 1]);
        }
        return true;
    }

    template<typename T, size_t size>
    bool tryParseHex(Span<char>& span, T& out, const bool invert) {
        return Helpers::tryParseHex<T, size>(ConstSpan<char>(span.begin(), span.length()), out, invert);
    }
}