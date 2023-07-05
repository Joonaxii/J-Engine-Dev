#include <JEngine/Utility/StringHelpers.h>
#include <vector>

namespace JEngine::Helpers {


	//Based on http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
	bool validateUtf8(uint32_t& state, const void* data, size_t size){
		static constexpr uint8_t utf8d[] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
			7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
			8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
			0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
			0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
			0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
			1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
			1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
			1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
		};

		uint32_t type;

		const uint8_t* str = reinterpret_cast<const uint8_t*>(data);
		for (size_t i = 0; i < size; i++) {
			type = utf8d[(uint8_t)str[i]];
			state = utf8d[256 + state * 16 + type];
			if (state == 1) { return false; }
		}
		return true;
	}

	size_t strIIndexOf(const std::string_view& strA, const std::string_view& strB) {
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

    bool strIEquals(const char* strA, const size_t lenA, const char* strB, const size_t lenB) {
        return strIEquals(std::string_view(strA, lenA), std::string_view(strB, lenB));
    }
    bool strIEquals(const std::string_view& strA, const std::string_view& strB) {
        if (strA.length() != strB.length()) { return false; }
        for (size_t i = 0; i < strA.length(); i++) {
            if (tolower(strA[i]) != tolower(strB[i])) { return false; }
        }
        return true;
    }

    bool strEquals(const char* strA, const size_t lenA, const char* strB, const size_t lenB) {
        return strIEquals(std::string_view(strA, lenA), std::string_view(strB, lenB));
    }
    bool strEquals(const std::string_view& strA, const std::string_view& strB) {
        if (strA.length() != strB.length()) { return false; }
        for (size_t i = 0; i < strA.length(); i++) {
            if (strA[i] != strB[i]) { return false; }
        }
        return true;
    }

    bool shouldBeWide(const wchar_t* str, int32_t len)  {
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

    void formatDataSize(char* buffer, size_t size) {
        static constexpr const char* SIZES[] {
            "byte",
            "KB",
            "MB",
            "GB",
            "TB",
        };

        double val = double(size);
        int32_t ind = 0;
        while (val >= 1024 && ind < (sizeof(SIZES) / sizeof(char*) - 1)) {
            val /= 1024.0;
            ind++;
        }
        sprintf_s(buffer, size, "%.3f %s", val, SIZES[ind]);
    }

	int32_t strCmpNoCase(const char* a, const char* b) {
		for (;; a++, b++) {
			const int32_t d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
			if (d != 0 || !*a) { return d; }
		}
	}

	bool equalsNoCase(const ConstSpan<char>& a, const ConstSpan<char>& b) {
		const auto len = a.length();
		if (len != b.length()) { return false; }
		if (len == 0) { return true; }
		const uint8_t* aP = reinterpret_cast<const uint8_t*>(a.get());
		const uint8_t* bP = reinterpret_cast<const uint8_t*>(b.get());
		for (size_t i = 0; i < len; i++, aP++, bP++) {
			if (tolower(*aP) != tolower(*bP)) { return false; }
		}
		return true;
	}

	bool equalsNoCase(const Span<char>& a, const Span<char>& b) {
		return equalsNoCase(reinterpret_cast<const ConstSpan<char>&>(a), reinterpret_cast<const ConstSpan<char>&>(b));
	}

	int32_t findNotIndexOf(const std::string& str, char c, size_t offset, size_t len) {
		return findNotIndexOf(str.c_str(), str.length(), c, offset, len);
	}

	int32_t findNotIndexOf(const char* str, size_t length, char c, size_t offset, size_t len) {
		size_t end = std::min(length, offset + len);
		for (size_t i = offset; i < end; i++) {
			if (str[i] != c) { return int32_t(i); }
		}
		return -1;
	}

	int32_t findNotIndexOf(const char* str, size_t length, const char* c, size_t offset, size_t len) {
		size_t end = std::min(length, offset + len);
		size_t lInp = strlen(c);
		const char* endPtr = c + lInp;
		for (size_t i = offset; i < end; i++) {
			if (std::find(c, endPtr, str[i]) == endPtr) { return int32_t(i); }
		}
		return -1;
	}

	int32_t indexOf(const char* str, const size_t length, const char c) {
		for (size_t i = 0; i < length; i++, str++) {
			if (*str == c) { return int32_t(i); }
		}
		return -1;
	}

	int32_t indexOf(const std::string& str, const char c) {
		for (size_t i = 0; i < str.length(); i++) {
			if (str[i] == c) { return int32_t(i); }
		}
		return -1;
	}

	int32_t indexOf(const char* str, const size_t length, const std::string& search) {
		const size_t sSize = search.length();
		int32_t index = -1;
		for (size_t i = 0, j = 0; i < length; i++, str++) {
			if (*str == search[j]) {
				if (j == 0) { index = int32_t(i); }
				j++;
				if (j >= sSize) { break; }
			}
		}
		return index;
	}

	int32_t indexOf(const std::string& str, const std::string& search) {
		const size_t sSize = search.length();
		int32_t index = -1;
		for (size_t i = 0, j = 0; i < str.length(); i++) {
			if (str[i] == search[j]) {
				if (j == 0) { index = int32_t(i); }
				j++;
				if (j >= sSize) { break; }
			}
		}
		return index;
	}

	uint32_t parseHexColorInt(const std::string& str, const int start, const int length) {
		int padding = 0;
		int len = length;

		uint8_t rgba[4]{ 0, 0, 0, 0xFF };

		char byteBuf[2];
		const char* cPtr = str.c_str();

		size_t k = start;
		if (length >= 7) {
			len = length > 8 ? 8 : length;
			padding = 8 - len;

			for (size_t i = 0; i < 2; i++)
			{
				if (padding-- > 0) {
					byteBuf[i] = '0';
					continue;
				}
				byteBuf[i] = cPtr[k++];
			}

			rgba[3] = std::stoi(byteBuf, 0, 16);
			len -= (2 - padding);
			padding = 0;
		}
		padding = 6 - len;

		for (size_t i = 0; i < 3; i++)
		{
			if (padding > 0) {
				for (size_t j = 0; j < 2; j++)
				{
					if (padding-- > 0) {
						byteBuf[j] = '0';
						continue;
					}
					byteBuf[j] = cPtr[k++];
				}
			}
			else {
				for (size_t j = 0; j < 2; j++)
				{
					byteBuf[j] = cPtr[k++];
				}
			}
			rgba[2 - i] = std::stoi(byteBuf, 0, 16);
		}
		return *(uint32_t*)rgba;
	}

	void parseString(const std::string& input, std::string& output) {
		size_t inQuote = 0;

		for (size_t i = 0; i < input.length(); i++) {
			auto c = input[i];
			if (c == '\\') {
				i++;
				continue;
			}
			if (c == '\"') { inQuote = i + 1; break; }
		}

		size_t endQuote = std::string::npos;
		for (size_t i = inQuote; i < input.length(); i++) {
			auto c = input[i];
			if (c == '\\') {
				i++;
				continue;
			}

			if (c == '\"') { endQuote = i; }
		}

		endQuote = endQuote != std::string::npos ? input.length() : endQuote;
		*&output = input.substr(inQuote, endQuote - inQuote);
	}

	void strSplit(const std::string& str, std::vector<std::string>& strs, const std::string& delim, const bool ignoreEmpty) {
		if (delim.length() < 1) {
			strs.push_back(str);
			return;
		}

		size_t start = 0;
		size_t end = str.find(delim, start);

		while (end != std::string::npos) {
			size_t count = end - start;

			if (ignoreEmpty && count < 1) {
				start++;
				continue;
			}

			end = str.find(delim, start + count + 1);
			strs.push_back(count < 1 ? "" : str.substr(start, count));
			start += count + 1;
		}

		if (str.length() <= start) { return; }
		const size_t c = str.length() - start;
		strs.push_back(str.substr(start, c));
	}


	void strSplit(const std::string& str, std::vector<std::string_view>& strs, const std::string& delim, const bool ignoreEmpty) {
		if (delim.length() < 1) {
			strs.push_back(std::string_view(str.c_str(), str.length()));
			return;
		}

		int32_t start = 0;
		int32_t end = indexOf(str.c_str() + start, str.length() - start, delim);

		while (end > -1) {
			int32_t count = end - start;

			const int32_t startP = start + count + 1;
			end = indexOf(str.c_str() + startP, str.length() - startP, delim);
			if (ignoreEmpty && count < 1) {
				start++;
				continue;
			}

			strs.push_back(count < 1 ? std::string_view() : std::string_view(str.c_str() + start, count));
			start += count + 1;
		}

		size_t c = str.length() - start;
		if (c < 1) { return; }
		strs.push_back(std::string_view(str.c_str() + start, c));
	}

	void addCommandArg(std::string& cmd, const std::string& arg) {
		cmd += arg + " ";
	}

	void addCommandArgString(std::string& cmd, const std::string& arg) {
		cmd.append("\"");
		cmd.append(arg);
		cmd.append("\" ");
	}
}