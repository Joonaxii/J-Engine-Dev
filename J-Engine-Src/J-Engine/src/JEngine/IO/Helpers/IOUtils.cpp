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
        if (!extensions || count < 1) { return true; }

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
        ofn.nMaxFile = DWORD(maxPathLen);
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
        CHAR szPath[MAX_PATH]{ 0 };

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

    std::string combine(ConstSpan<char> lhs, ConstSpan<char>  rhs) {
        std::string newStr{};
        size_t lenLhs = lhs.length();
        size_t lenRhs = rhs.length();
        newStr.reserve(lhs.length() + rhs.length() + 1);
        newStr.append(lhs.get(), lhs.length());

        uint8_t state = 0;
        char lastC = lenLhs == 0 ? 0 : *(lhs.get() + (lenLhs - 1));
        state |= (lastC == '/' || lastC == '\\') ? 1 : 0;
        lastC = lenRhs == 0 ? 0 : rhs[0];
        state |= (lastC == '/' || lastC == '\\') ? 2 : 0;

        size_t offset = 0;
        switch (state) {
            case 0:
                newStr.append(1, '/');
                break;
            case 3:
                offset = Helpers::indexNotOf(rhs, "/\\");
                offset = offset < 0 ? 0 : offset;
                break;
        }
        auto tmp = rhs.slice(offset);
        newStr.append(tmp.get(), tmp.length());

        fixPath(newStr);
        return newStr;
    }

    ConstSpan<char>  getDirectory(ConstSpan<char>  path) {
        ConstSpan<char> spn(path);
        int32_t pth = spn.indexOfAnyLast("/\\", 2);

        if (pth < 0) { return std::string_view(); }
        return path.slice(0, pth);
    }

    std::string getExeDir() {
        char buffer[MAX_PATH]{ 0 };
        GetModuleFileName(NULL, buffer, MAX_PATH);
        ConstSpan<char> temp(buffer, strlen(buffer));
        int32_t last = temp.indexOfAnyLast("/\\", 2);
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
            JENGINE_CORE_WARN("[IOUtils] Warning: File search flags are set to 0!");
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

    void enumerateFiles(ConstSpan<char> path, ConstSpan<char> extension, std::function<const void(ConstSpan<char>)> callback, bool fixPath, bool reverse) {
        if (!IO::exists(path)) {
            JENGINE_CORE_WARN("[IOUtils] Warning: Path '{0}' was not found, cannot enumerate!", path);
            return;
        }
        fs::path pth(std::string_view(path.get(), path.length()));
        for (const auto& entry : std::filesystem::recursive_directory_iterator(pth)) {
            std::string name = entry.path().string();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

            bool len = name.length() >= extension.length();
            if (!IO::isFile(name)) { continue; }

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

    void enumerateFiles(ConstSpan<char>  path, ConstSpan<char>  extension, std::vector<std::string>& paths, bool fixPath, bool reverse) {
        fs::path pth(std::string_view(path.get(), path.length()));
        for (const auto& entry : std::filesystem::recursive_directory_iterator(pth)) {
            std::string name = entry.path().string();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

            bool len = name.length() >= extension.length();
            if (!IO::isFile_FS(name)) { continue; }

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

    bool exists_FS(const fs::path& path) {
        return std::filesystem::exists(path);
    }

    bool isFile_FS(const fs::path& path) {
        return fs::is_regular_file(path);
    }

    bool isDir_FS(const fs::path& path) {
        return fs::is_directory(path);
    }

    bool createDir_FS(const fs::path& path) {
        if (IO::exists_FS(path)) {
            return IO::isDir_FS(path);
        }

        return fs::create_directories(path);
    }

    bool delFile_FS(const fs::path& path) {
        if (!IO::exists_FS(path)) { return false; }
        return std::filesystem::remove(path);
    }

    bool delDirRecursively_FS(const fs::path& path) {
        if (!IO::exists_FS(path)) { return false; }
        return std::filesystem::remove_all(path) > 0;
    }

    void copyRecursively_FS(const std::filesystem::path& src, const std::filesystem::path& target) noexcept {
        try {
            std::filesystem::copy(src, target, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
        }
        catch (std::exception& e) {
            JENGINE_CORE_ERROR("[IOUtils - copyRecursive] Error: {0}", e.what());
        }
    }

    void fixPath(Span<char> path) {
        for (size_t i = 0; i < path.length(); i++) {
            auto c = path[i];
            if (c == '\\') {
                path[i] = '/';
            }
        }
    }

    bool pathsAreEqual(ConstSpan<char> lhs, ConstSpan<char> rhs) {
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