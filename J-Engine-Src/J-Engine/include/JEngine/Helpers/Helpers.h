#pragma once
#include <string>

namespace JEngine::Helpers {
    const bool strEqualsIgnoreCase(const std::string& strA, const std::string& strB);
    const bool wstrEqualsIgnoreCase(const std::wstring& strA, const std::wstring& strB);

    const int lastIndexOfSlash(const std::string& path);

    const std::string getDirectoryName(const std::string& path);
    const std::string getFileName(const std::string& path, const bool includeExtension);

    const int32_t bytesRequired(const int32_t bits);
}
