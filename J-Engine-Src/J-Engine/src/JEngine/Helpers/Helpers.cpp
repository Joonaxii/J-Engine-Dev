#include <JEngine/Helpers/Helpers.h>

namespace JEngine::Helpers {


    const bool strEqualsIgnoreCase(const std::string& strA, const std::string& strB) {
        unsigned int sz = strA.size();

        if (strB.size() != sz) { return false; }

        for (unsigned int i = 0; i < sz; i++) {
            if (tolower(strA[i]) != tolower(strB[i])) { return false; }
        }
        return true;
    }

    const bool wstrEqualsIgnoreCase(const std::wstring& strA, const std::wstring& strB) {
        unsigned int sz = strA.size();

        if (strB.size() != sz) { return false; }

        for (unsigned int i = 0; i < sz; i++) {
            if (tolower(strA[i]) != tolower(strB[i])) { return false; }
        }
        return true;
    }
    const int lastIndexOfSlash(const std::string& path) {
        for (int i = path.size() - 1; i >= 0; i--) {
            if (path[i] == '\\' || path[i] == '/') { return i; }
        }
        return -1;
    }
    const std::string getDirectoryName(const std::string& path) {
        int slash = lastIndexOfSlash(path);
        slash = slash < 0 ? path.length() : slash;

        return slash < 0 ? "" : path.substr(0, slash + 1);
    }
    const std::string getFileName(const std::string& path, const bool includeExtension) {

        int slash = lastIndexOfSlash(path);
        std::string name = slash < 0 ? path : path.substr(slash + 1, path.length() - slash - 1);

        int dot = name.find_last_of('.');
        if (dot > -1 && !includeExtension) {
            name = name.substr(0, dot);
        }
        return name;
    }

    const int32_t bytesRequired(const int32_t bits) {
        const int32_t bitsShft = bits >> 3;
        return ((bitsShft << 3) == bits) ? bitsShft : bitsShft + 1;
    }
}