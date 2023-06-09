#include <JEngine/IO/Helpers/IOUtils.h>
#include <Windows.h>
#include <commdlg.h>
#include <shlobj_core.h>
#include <JEngine/Utility/StringHelpers.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <JEngine/Rendering/Window.h>
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
        ofn.hwndOwner = glfwGetWin32Window(Window::getInstance()->getNativeWindow());
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

        pth.hwndOwner = glfwGetWin32Window(Window::getInstance()->getNativeWindow());
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
}