#include <JEngine/Utility/StringHelpers.h>
#include <vector>

namespace JEngine::Helpers {

	//Based on http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
	bool validateUtf8(uint32_t& state, const void* data, size_t size){
		static constexpr uint8_t UTF8_TABLE[] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
			7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
			8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
			0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3,
			0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,
			0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,
			1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,
			1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1,
			1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		};

		uint32_t type;

		const uint8_t* str = reinterpret_cast<const uint8_t*>(data);
		for (size_t i = 0; i < size; i++) {
			type = UTF8_TABLE[str[i]];
			state = UTF8_TABLE[256 + state * 16 + type];
			if (state == 1) { return false; }
		}
		return true;
	}
    
    size_t strIIndexOf(ConstSpan<char>  strA, ConstSpan<char>  strB) {
        if (strB.length() < 1) { return 0; }
        size_t c = 0;
        size_t ind = 0;

        char cB = tolower(strB[c]);
        for (size_t i = 0; i < strA.length(); i++) {
            char cA = tolower(strA[i]);
            if (cA == cB) {
                if (c >= strB.length()) { return ind; }
                if (c == 0) {
                    ind = i;
                }
                cB = tolower(strB[++c]);
            }
            else if (c) {
                c = 0;
                cB = tolower(strB[c]);
            }
        }
        return std::string::npos;
    }

    bool strIEquals(ConstSpan<char>  strA, ConstSpan<char> strB) {
        if (strA.length() != strB.length()) { return false; }
        for (size_t i = 0; i < strA.length(); i++) {
            if (tolower(strA[i]) != tolower(strB[i])) { return false; }
        }
        return true;
    }

    bool strIContains(ConstSpan<char> strA, ConstSpan<char> strB) {
        if (strB.length() < 1) { return true; }

        if (strB.length() > strA.length()) { return false; }
        size_t inRow = 0;
        for (size_t i = 0; i < strA.length(); i++) {
            if (tolower(strA[i]) == tolower(strB[inRow++]))
            {
                if (inRow >= strB.length()) { return true; }
                continue;
            }
            inRow = 0;
        }
        return false;
    }

    bool strEquals(ConstSpan<char> strA, ConstSpan<char> strB) {
        if (strA.length() != strB.length()) { return false; }
        for (size_t i = 0; i < strA.length(); i++) {
            if (strA[i] != strB[i]) { return false; }
        }
        return true;
    }

    bool shouldBeWide(const wchar_t* str, int32_t len) {
        for (size_t i = 0; i < len; i++) {
            if (str[i] > 255) { return true; }
        }
        return false;
    }

    bool wideToASCII(wchar_t* str, int32_t len) {
        char* ascii = reinterpret_cast<char*>(str);
        for (size_t i = 0; i < len; i++) {
            ascii[i] = char(str[i] & 0xFF);
        }
        return false;
    }

    bool getLine(const char*& buffer, size_t& size, std::string& line) {
        line.clear();
        while (size > 0) {
            char c = *buffer++;
            size--;
            if (c == '\n') { break; }

            if (c != '\r') {
                line.push_back(c);
            }
        }
        return line.length() > 0;
    }

    bool endsWith(ConstSpan<char> str, ConstSpan<char> end, bool caseSensitive) {
        size_t len = str.length();
        size_t lenB = end.length();

        if (len < lenB) { return false; }

        int32_t lnA = int32_t(len - 1);
        int32_t lnB = int32_t(lenB - 1);
        for (int32_t i = lnA, j = lnB; j >= 0; i--, j--) {
            char cA = str[i];
            char cB = end[j];

            if (!caseSensitive) {
                cA = char(tolower(cA));
                cB = char(tolower(cB));
            }

            if (cA != cB) { return false; }
        }
        return true;
    }

    bool startsWith(ConstSpan<char> str, ConstSpan<char> end, bool caseSensitive) {
        size_t len = str.length();
        size_t lenB = end.length();

        if (len < lenB) { return false; }

        for (size_t i = 0, j = 0; j < lenB; i++, j++) {
            char cA = str[i];
            char cB = end[j];
            if (!caseSensitive) {
                cA = char(tolower(cA));
                cB = char(tolower(cB));
            }
            if (cA != cB) { return false; }
        }
        return true;
    }

    size_t lastIndexOf(ConstSpan<char> str, ConstSpan<char> end, bool caseSensitive) {
        size_t len = str.length();

        if (len < 1) { return std::string::npos; }
        size_t bufSize = end.length();
        size_t i = len;
        size_t j = bufSize;
        size_t sI = 0;
        while (i > 0) {
            char cA = str[--i];
            char cB = end[--j];

            if (!caseSensitive) {
                cA = tolower(cA);
                cB = tolower(cB);
            }

            if (cA == cB)
            {
                if (j <= 0) {
                    return i;
                }
                continue;
            }
            if (j != bufSize) { j = bufSize; }
        }
        return std::string::npos;
    }
    size_t indexOf(ConstSpan<char> str, ConstSpan<char> end, bool caseSensitive) {
        size_t len = str.length();
        if (len < 1) { return std::string::npos; }

        size_t j = 0;
        size_t sI = 0;
        size_t bufSize = end.length();
        for (size_t i = 0; i < len; i++) {
            char cA = str[i++];
            char cB = end[j++];

            if (!caseSensitive) {
                cA = tolower(cA);
                cB = tolower(cB);
            }

            if (cA == cB)
            {
                if (j == 1) {
                    sI = i;
                }
                if (j >= bufSize) { return sI; }
                continue;
            }

            if (j != 0) { j = 0; }
        }
        return std::string::npos;
    }

    size_t lastIndexNotOf(ConstSpan<char> str, ConstSpan<char> end, bool caseSensitive) {
        size_t len = str.length();
        if (len < 1) { return std::string::npos; }
        size_t bufSize = end.length();
        size_t i = len;
        size_t j = bufSize;
        size_t sI = 0;
        while (i > 0)
        {
            char cA = str[--i];
            char cB = end[--j];

            if (!caseSensitive) {
                cA = tolower(cA);
                cB = tolower(cB);
            }

            if (cA != cB)
            {
                if (j <= 0) {
                    return i;
                }
                continue;
            }
            if (j != bufSize) { j = bufSize; }
        }
        return std::string::npos;
    }
    size_t indexNotOf(ConstSpan<char> str, ConstSpan<char> end, bool caseSensitive) {
        size_t len = str.length();
        if (len < 1) { return std::string::npos; }

        size_t bufSize = end.length();
        size_t j = 0;
        size_t sI = 0;
        for (size_t i = 0; i < len; i++) {
            char cA = str[i++];
            char cB = end[j++];

            if (!caseSensitive) {
                cA = tolower(cA);
                cB = tolower(cB);
            }

            if (cA != cB)
            {
                if (j == 1) {
                    sI = i;
                }
                if (j >= bufSize) { return sI; }
                continue;
            }

            if (j != 0) { j = 0; }
        }
        return std::string::npos;
    }

    size_t lastIndexOf(ConstSpan<char> str, const char* const* end, size_t endCount, bool caseSensitive) {
        if (endCount < 1 || !end) { return std::string::npos; }
        size_t ind = lastIndexOf(str, end[0], caseSensitive);
        for (size_t i = 1; i < endCount; i++) {
            auto ii = lastIndexOf(str, end[i], caseSensitive);
            ind = (ind == std::string::npos) ? ii : ii != std::string::npos ? std::max(ii, ind) : ind;
        }
        return ind;
    }
    size_t indexOf(ConstSpan<char> str, const char* const* end, size_t endCount, bool caseSensitive) {
        if (endCount < 1 || !end) { return std::string::npos; }
        size_t ind = indexOf(str, end[0], caseSensitive);
        for (size_t i = 1; i < endCount; i++) {
            ind = std::min(indexOf(str, end[i], caseSensitive), ind);
        }
        return ind;
    }

    size_t indexOfNonNum(ConstSpan<char> str) {
        for (size_t i = 0; i < str.length(); i++) {
            if (str[i] < '0' || str[i] > '9') { return i; }
        }
        return std::string::npos;
    }

    void wideToUTF8(const wchar_t* strIn, size_t size, std::string& strOut) {
        strOut.clear();
        uint32_t codepoint = 0;
        for (size_t i = 0; i < size; i++) {
            wchar_t cur = strIn[i];
            if (cur >= 0xd800 && cur <= 0xdbff) {
                codepoint = ((*strIn - 0xd800) << 10) + 0x10000;
            }
            else
            {
                if (cur >= 0xdc00 && cur <= 0xdfff)
                    codepoint |= cur - 0xdc00;
                else {
                    codepoint = cur;
                }

                if (codepoint <= 0x7f) {
                    strOut.append(1, static_cast<char>(codepoint));
                }
                else if (codepoint <= 0x7ff) {
                    strOut.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
                    strOut.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                }
                else if (codepoint <= 0xffff) {
                    strOut.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
                    strOut.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                    strOut.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                } else {
                    strOut.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
                    strOut.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
                    strOut.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
                    strOut.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
                }
                codepoint = 0;
            }
        }
    }
}