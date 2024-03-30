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

    std::string combine(ConstSpan<char> lhs, ConstSpan<char> rhs);

    ConstSpan<char> getDirectory(ConstSpan<char> path);

    std::string getExeDir();

    bool getAll(const char* path, size_t length, uint8_t flags, std::vector<FilePath>& paths, bool recursive = false);
    bool getAll(const fs::path& path, uint8_t flags, std::vector<FilePath>& paths, bool recursive = false);

    bool getAllFiles(const char* path, std::vector<fs::path>& paths, bool recursive = false);
    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char* ext, bool recursive = false);
    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char** ext, size_t extCount = 1, bool recursive = false);

    void enumerateFiles(ConstSpan<char> path, ConstSpan<char> extension, std::function<const void(ConstSpan<char>)> callback, bool fixPath = true, bool reverse = false);
    void enumerateFiles(ConstSpan<char> path, ConstSpan<char> extension, std::vector<std::string>& paths, bool fixPath = true, bool reverse = false);

    bool exists_FS(const fs::path& path);
    bool isDir_FS(const fs::path& path);
    bool isFile_FS(const fs::path& path);
    bool createDir_FS(const fs::path& path);

    bool delFile_FS(const fs::path& path);
    bool delDirRecursively_FS(const fs::path& path);
    void copyRecursively_FS(const std::filesystem::path& src, const std::filesystem::path& target) noexcept;

    inline bool exists(ConstSpan<char> path) { 
        return IO::exists_FS(fs::path(path.get(), path.get() + path.length())); 
    }
    inline bool isDir(ConstSpan<char> path) {
        return IO::isDir_FS(fs::path(path.get(), path.get() + path.length()));
    }
    inline bool isFile(ConstSpan<char> path) {
        return IO::isFile_FS(fs::path(path.get(), path.get() + path.length()));
    }
    inline bool createDir(ConstSpan<char> path) {
        return IO::createDir_FS(fs::path(path.get(), path.get() + path.length()));
    }
    inline bool delDirRecursively(ConstSpan<char> path) {
        return IO::delDirRecursively_FS(fs::path(path.get(), path.get() + path.length()));
    }
    inline bool delFile(ConstSpan<char> path) {
        return IO::delFile_FS(fs::path(path.get(), path.get() + path.length()));
    }

    inline void copyRecursively(ConstSpan<char> src, ConstSpan<char> target) noexcept {
        return IO::copyRecursively_FS(fs::path(src.get(), src.get() + src.length()), fs::path(target.get(), target.get() + target.length()));
    }

    void fixPath(Span<char> path);
    bool pathsAreEqual(ConstSpan<char> lhs, ConstSpan<char> rhs);

}