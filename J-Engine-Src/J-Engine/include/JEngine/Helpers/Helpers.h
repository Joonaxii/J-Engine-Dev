#pragma once
#include <string>

namespace JEngine::Helpers {
    bool strEqualsIgnoreCase(const std::string& strA, const std::string& strB);
    bool wstrEqualsIgnoreCase(const std::wstring& strA, const std::wstring& strB);

    int32_t lastIndexOfSlash(const std::string& path);

    std::string getDirectoryName(const std::string& path);
    std::string getFileName(const std::string& path, const bool includeExtension);
}
