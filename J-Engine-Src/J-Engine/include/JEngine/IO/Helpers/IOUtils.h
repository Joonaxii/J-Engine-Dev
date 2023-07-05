#pragma once
#include <string>
#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include <JEngine/Utility/Span.h>

namespace fs = std::filesystem;

namespace JEngine::IO {
    enum : uint8_t {
        F_TYPE_FILE = 0x1,
        F_TYPE_FOLDER = 0x2,
        F_TYPE_ALL = F_TYPE_FILE | F_TYPE_FOLDER,
    };

    struct FilePath {
        fs::path path{};
        uint8_t type{0};

        FilePath() : path(), type() {}
        FilePath(const fs::path& path, uint8_t type) : path(path), type(type) {}
    };

    static bool hasAnyExtension(const fs::path& path, const char** extensions, size_t count);

    std::string openFile(const char* filter, const size_t maxPathLen = 260, bool allowCreate = false, bool noValidate = false, size_t filterTypes = 1);
    std::string openFolder(const char* title, const size_t maxPathLen = 260);

    std::string combine(const char* lhs, size_t lenLhs, const char* rhs, size_t lenRhs);
    std::string combine(const std::string& lhs, const std::string& rhs);

    std::string getExeDir();

    bool getAll(const char* path, size_t length, uint8_t flags, std::vector<FilePath>& paths, bool recursive = false);
    bool getAll(const fs::path& path, uint8_t flags, std::vector<FilePath>& paths, bool recursive = false);

    bool getAllFiles(const char* path, std::vector<fs::path>& paths, bool recursive = false);
    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char* ext, bool recursive = false);
    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char** ext, size_t extCount = 1, bool recursive = false);

    void enumerateFiles(const std::string& path, const std::string& extension, std::function<const void(const std::string&)> callback, const bool fixPath = true, const bool reverse = false);
    void enumerateFiles(const std::string& path, const std::string& extension, std::vector<std::string>& paths, const bool fixPath = true, const bool reverse = false);

    bool exists(const char* path, size_t length);
    bool exists(const char* path);
    bool exists(const std::string& path);
   
    bool isFile(const std::string& path);
    bool isDir(const std::string& path);

    void delDirRecursively(const std::string& path);
    void copyRecursively(const std::filesystem::path& src, const std::filesystem::path& target) noexcept;

    void fixPath(std::string& path);
    void fixPath(char* path);
    void fixPath(char* path, size_t length);

    bool pathsAreEqual(ConstSpan<char> lhs, ConstSpan<char> rhs);

}