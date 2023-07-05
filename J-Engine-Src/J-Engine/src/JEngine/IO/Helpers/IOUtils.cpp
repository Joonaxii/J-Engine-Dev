#include <JEngine/IO/Helpers/IOUtils.h>
#include <Windows.h>
#include <commdlg.h>
#include <shlobj_core.h>
#include <JEngine/Utility/StringHelpers.h>
#include <JEngine/Utility/Span.h>

#include <JEngine/Rendering/Window.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <JEngine/Core/Log.h>
#include <shellapi.h>

namespace JEngine::IO {
    bool hasAnyExtension(const fs::path& path, const char** extensions, size_t count) {
        if (!extensions || count < 1)  { return true; }

        if (!path.has_extension()) {
            for (size_t i = 0; i < count; i++) {
                if (strlen(extensions[i]) < 2) { return true; }
            }
            return false;
        }

        auto ext = path.extension();
        std::string_view extV(reinterpret_cast<const char*>(ext.c_str()));
        for (size_t i = 0; i < count; i++) {
            if (Helpers::strIEquals(extV, extensions[i])) {
                return true;
            }
        }
        return false;
    }

    std::string openFile(const char* filter, const size_t maxPathLen, bool allowCreate, bool noValidate, size_t filterTypes) {
        OPENFILENAMEA ofn;
        CHAR* szFile = reinterpret_cast<CHAR*>(_malloca(maxPathLen));

        if (!szFile) { return ""; }

        ZeroMemory(szFile, sizeof(CHAR) * maxPathLen);
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window(Window::getInstance()->getWindowPtr());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile  = DWORD(maxPathLen);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = DWORD(filterTypes);
        ofn.Flags = OFN_PATHMUSTEXIST | (allowCreate ? 0 : OFN_FILEMUSTEXIST) | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE) {
            std::string str = ofn.lpstrFile;
            _freea(szFile);
            return str;
        }
        _freea(szFile);
        return std::string();
    }

    std::string openFolder(const char* title, const size_t maxPathLen) {
        CHAR szPath[MAX_PATH]{0};

        BROWSEINFOA pth;
        ZeroMemory(&pth, sizeof(BROWSEINFOA));

        pth.hwndOwner = glfwGetWin32Window(Window::getInstance()->getWindowPtr());
        pth.pszDisplayName = szPath;
        pth.lpszTitle = title;
        pth.ulFlags = BIF_RETURNONLYFSDIRS;

        LPITEMIDLIST pidl = SHBrowseForFolderA(&pth);

        if (pidl != NULL) {
            if (SHGetPathFromIDListA(pidl, szPath)) {
                return szPath;
            }
        }
        return std::string();
    }

    std::string combine(const char* lhs, size_t lenLhs, const char* rhs, size_t lenRhs) {
        std::string newStr{};
        newStr.reserve(lenLhs + lenRhs + 1);
        newStr.append(lhs, lenLhs);

        uint8_t state = 0;
        char lastC = lenLhs == 0 ? 0 : *(lhs + lenLhs - 1);
        state |= (lastC == '/' || lastC == '\\') ? 1 : 0;
        lastC = lenRhs == 0 ? 0 : rhs[0];
        state |= (lastC == '/' || lastC == '\\') ? 2 : 0;

        size_t offset = 0;
        switch (state) {
            case 0:
                newStr.append(1, '/');
                break;
            case 3:
                offset = Helpers::findNotIndexOf(rhs, lenRhs, "/\\", 0, lenRhs);
                offset = offset < 0 ? 0 : offset;
                break;
        }
        newStr.append(rhs + offset, lenRhs - offset);

        fixPath(newStr);
        return newStr;
    }

    std::string combine(const std::string& lhs, const std::string& rhs) {
        return combine(lhs.c_str(), lhs.length(), rhs.c_str(), rhs.length());
    }

    std::string getExeDir() {
        char buffer[MAX_PATH]{ 0 };
        GetModuleFileName(NULL, buffer, MAX_PATH);
        ConstSpan<char> temp(buffer, strlen(buffer));
        int32_t last = temp.indexOfLast('/');
        if (last > -1) {
            temp = temp.slice(0, last);
        }
        return std::string(temp.get(), temp.length());
    }

    bool getAll(const char* path, size_t length, uint8_t flags, std::vector<FilePath>& paths, bool recursive) {
        fs::path pPath(path, path + length);
        return getAll(pPath, flags, paths, recursive);
    }

    bool getAll(const fs::path& path, uint8_t flags, std::vector<FilePath>& paths, bool recursive) {
        if ((flags & 0x3) == 0) {
            JENGINE_CORE_WARN("[J-Engine - IOUtils] Warning: Search flags are set to 0!");
            return false;
        }
        size_t startC = paths.size();

        if (recursive) {
            for (const auto& dir : fs::recursive_directory_iterator(path)) {
                if ((flags & F_TYPE_FILE) && dir.is_regular_file()) {
                    paths.emplace_back(dir.path(), F_TYPE_FILE);
                    continue;
                }

                if ((flags & F_TYPE_FOLDER) && dir.is_directory()) {
                    paths.emplace_back(dir.path(), F_TYPE_FOLDER);
                }
            }
        }
        else {
            for (const auto& dir : fs::directory_iterator(path)) {
                if ((flags & F_TYPE_FILE) && dir.is_regular_file()) {
                    paths.emplace_back(dir.path(), F_TYPE_FILE);
                    continue;
                }

                if ((flags & F_TYPE_FOLDER) && dir.is_directory()) {
                    paths.emplace_back(dir.path(), F_TYPE_FOLDER);
                }
            }
        }

        return startC != paths.size();
    }


    bool getAllFiles(const char* path, std::vector<fs::path>& paths, bool recursive) {
        size_t init = paths.size();
        if (recursive) {
            for (const auto& dir : fs::recursive_directory_iterator(path)) {
                if (dir.is_regular_file()) {
                    paths.emplace_back(dir.path());
                }
            }
        }
        else {
            for (const auto& dir : fs::directory_iterator(path)) {
                if (dir.is_regular_file()) {
                    paths.emplace_back(dir.path());
                }
            }
        }
        return paths.size() > init;
    }

    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char* ext, bool recursive) {
        return getAllFilesByExt(path, paths, &ext, 1, recursive);
    }

    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char** ext, size_t extCount, bool recursive) {
        size_t init = paths.size();
        if (recursive) {
            for (const auto& dir : fs::recursive_directory_iterator(path)) {
                if (dir.is_regular_file()) {
                    paths.emplace_back(dir.path());
                }
            }
        }
        else {
            for (const auto& dir : fs::directory_iterator(path)) {
                if (dir.is_regular_file()) {
                    paths.emplace_back(dir.path());
                }
            }
        }
        return paths.size() > init;
        return false;
    }

    void enumerateFiles(const std::string& path, const std::string& extension, std::function<const void(const std::string&)> callback, const bool fixPath, const bool reverse) {
        if (!exists(path)) {
            JENGINE_CORE_WARN("[IOUtils - Enumerate Files] Warning: Path '{0}' was not found!", path.c_str());
            return;
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            std::string name = entry.path().string();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

            bool len = name.length() >= extension.length();
            if (!isFile(name)) { continue; }

            bool isSame = true;
            if (len) {
                for (size_t i = name.length() - extension.length(), j = 0; i < name.length(); i++, j++) {
                    if (name[i] != std::tolower(extension[j])) {
                        isSame = false;
                        break;
                    }
                }
            }

            const bool run = reverse ? !isSame : isSame;
            if (run) {
                std::string str = entry.path().string();
                if (fixPath) {
                    for (size_t i = 0; i < str.length(); i++) {
                        auto c = str[i];
                        if (c == '\\') {
                            str[i] = '/';
                        }
                    }
                }
                callback(str);
            }
        }
    }

    void enumerateFiles(const std::string& path, const std::string& extension, std::vector<std::string>& paths, const bool fixPath, const bool reverse) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            std::string name = entry.path().string();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

            bool len = name.length() >= extension.length();
            if (!isFile(name)) { continue; }

            bool isSame = true;
            if (len) {
                for (size_t i = name.length() - extension.length(), j = 0; i < name.length(); i++, j++) {
                    if (name[i] != std::tolower(extension[j])) {
                        isSame = false;
                        break;
                    }
                }
            }

            const bool run = reverse ? !isSame : isSame;
            if (run) {
                std::string str = entry.path().string();
                if (fixPath) {
                    for (size_t i = 0; i < str.length(); i++) {
                        auto c = str[i];
                        if (c == '\\') {
                            str[i] = '/';
                        }
                    }
                }
                paths.push_back(str);
            }
        }
    }

    bool exists(const char* path, size_t length) {
        if (path == nullptr || length == 0) { return false; }
        auto pth = std::filesystem::u8path(path, path + length);
        return std::filesystem::exists(pth);
    }

    bool exists(const char* path) {
        return exists(path, strlen(path));
    }

    bool exists(const std::string& path) {
        return exists(path.c_str());
    }

    bool isFile(const std::string& path) {
        struct stat info;
        if (stat(path.c_str(), &info) != 0) { return 0; }
        else if (info.st_mode & S_IFREG) { return 1; }
        return 0;
    }

    bool isDir(const std::string& path) {
        struct stat info;

        if (stat(path.c_str(), &info) != 0) { return 0; }
        else if (info.st_mode & S_IFDIR) { return 1; }
        return 0;
    }

    void delDirRecursively(const std::string& path) {
        if (!exists(path)) { return; }
        std::filesystem::remove_all(path);
    }

    void copyRecursively(const std::filesystem::path& src, const std::filesystem::path& target) noexcept {
        try {
            std::filesystem::copy(src, target, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
        }
        catch (std::exception& e) {
            JENGINE_CORE_ERROR("[IOUtils - copyRecursive] Error: {0}", e.what());
        }
    }

    void fixPath(std::string& path) {
        for (size_t i = 0; i < path.length(); i++) {
            auto c = path[i];
            if (c == '\\') {
                path[i] = '/';
            }
        }
    }

    void fixPath(char* path) {
        while (true) {
            char c = *path;
            if (c == '\0') { break; }

            if (c == '\\') {
                *path = '/';
            }
            path++;
        }
    }

    void fixPath(char* path, size_t length) {
        while (length-- > 0) {
            char c = *path;
            if (c == '\\') {
                *path = '/';
            }
            path++;
        }
    }

    bool pathsAreEqual(ConstSpan<char> lhs, ConstSpan<char> rhs)  {
        if (lhs.length() == rhs.length()) {
            for (size_t i = 0; i < lhs.length(); i++) {
                char a = lhs[i];
                char b = rhs[i];

                a = char(std::tolower(a));
                b = char(std::tolower(b));

                a = a == '\\' ? '/' : a;
                b = b == '\\' ? '/' : b;

                if (a != b) { return false; }
            }
            return true;
        }
        return false;
    }
}