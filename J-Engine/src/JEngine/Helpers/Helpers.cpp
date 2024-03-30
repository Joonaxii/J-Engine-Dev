#include <JEngine/Helpers/Helpers.h>

namespace JEngine::Helpers {
    bool strEqualsIgnoreCase(const std::string& strA, const std::string& strB) {
        size_t sz = strA.size();

        if (strB.size() != sz) { return false; }

        for (unsigned int i = 0; i < sz; i++) {
            if (tolower(strA[i]) != tolower(strB[i])) { return false; }
        }
        return true;
    }

    bool wstrEqualsIgnoreCase(const std::wstring& strA, const std::wstring& strB) {
        size_t sz = strA.size();

        if (strB.size() != sz) { return false; }

        for (unsigned int i = 0; i < sz; i++) {
            if (tolower(strA[i]) != tolower(strB[i])) { return false; }
        }
        return true;
    }
    int32_t lastIndexOfSlash(const std::string& path) {
        for (int32_t i = int32_t(path.size() - 1); i >= 0; i--) {
            if (path[i] == '\\' || path[i] == '/') { return i; }
        }
        return -1;
    }
    std::string getDirectoryName(const std::string& path) {
        int32_t slash = lastIndexOfSlash(path);
        slash = slash < 0 ? int32_t(path.length()) : slash;

        return slash < 0 ? "" : path.substr(0, slash + 1);
    }
    std::string getFileName(const std::string& path, const bool includeExtension) {

        int slash = lastIndexOfSlash(path);
        std::string name = slash < 0 ? path : path.substr(slash + 1, path.length() - slash - 1);

        size_t dot = name.find_last_of('.');
        if (dot != std::string::npos && !includeExtension) {
            name = name.substr(0, dot);
        }
        return name;
    }
}