#pragma once
#include <string>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace JEngine::IO {

    static bool hasAnyExtension(const fs::path& path, const char** extensions, size_t count);

    std::string openFile(const char* filter, const size_t maxPathLen = 260, bool allowCreate = false, bool noValidate = false, size_t filterTypes = 1);
    std::string openFolder(const char* title, const size_t maxPathLen = 260);

    bool getAllFiles(const char* path, std::vector<fs::path>& paths, bool recursive = false);
    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char* ext, bool recursive = false);
    bool getAllFilesByExt(const char* path, std::vector<fs::path>& paths, const char** ext, size_t extCount = 1, bool recursive = false);

}