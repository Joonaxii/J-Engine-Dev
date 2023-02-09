#include <JEngine/Helpers/StringExtensions.h>

namespace JEngine::Helpers {

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
		const uint8_t* aP = reinterpret_cast<const uint8_t*>(a.begin());
		const uint8_t* bP = reinterpret_cast<const uint8_t*>(b.begin());
		for (size_t i = 0; i < len; i++, aP++, bP++)	{
			if (tolower(*aP) != tolower(*bP)) { return false; }
		}
		return true;
	}

	bool equalsNoCase(const Span<char>& a, const Span<char>& b) {
		return equalsNoCase(reinterpret_cast<const ConstSpan<char>&>(a), reinterpret_cast<const ConstSpan<char>&>(b));
	}

	int32_t findNotIndexOf(const std::string& str, const char c, size_t offset, size_t len) {
		for (size_t i = offset; i < offset + len; i++)
		{
			if (str[i] != c) { return int32_t(i); }
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

	uint8_t hexToUI8(const char c) {
		if (c >= '0' && c <= '9') { return (c - '0'); }
		if (c >= 'A' && c <= 'F') { return (c - 'A') + 10; }
		if (c >= 'a' && c <= 'f') { return (c - 'a') + 10; }
		return 0;
	}

	uint8_t hexToUI8(const char a, const char b) {
		return hexToUI8(a) | (hexToUI8(b) << 4);
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