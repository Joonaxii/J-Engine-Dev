#pragma once
#include <string>
#include <vector>
#include <string>
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

namespace JEngine::IO {

    static bool hasAnyExtension(const fs::path& path, const char** extensions, size_t count);

    std::string openFile(const char* filter, const size_t maxPathLen = 260, bool allowCreate = false, bool noValidate = false, size_t filterTypes = 1);
    std::string openFolder(const char* title, const size_t maxPathLen = 260);

    bool getAllFiles(const char* path, std::vector<fs::path>& paths, bool recursive = false);
    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char* ext, bool recursive = false);
    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char** ext, size_t extCount = 1, bool recursive = false);

    void enumerateFiles(const std::string& path, const std::string& extension, std::function<const void(const std::string&)> callback, const bool fixPath = true, const bool reverse = false);
    void enumerateFiles(const std::string& path, const std::string& extension, std::vector<std::string>& paths, const bool fixPath = true, const bool reverse = false);

    bool exists(const char* path);
    bool exists(const std::string& path);
   
    bool isFile(const std::string& path);
    bool isDir(const std::string& path);

    void delDirRecursively(const std::string& path);
    void copyRecursively(const std::filesystem::path& src, const std::filesystem::path& target) noexcept;

    void fixPath(std::string& path);
    void fixPath(char* path);

}