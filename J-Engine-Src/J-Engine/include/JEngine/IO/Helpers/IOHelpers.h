#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <filesystem>
#include <JEngine/Assets/AssetDataType.h>
#define NAMEOF(name) #name

namespace JEngine {
    namespace IO {
        void enumerateFiles(const std::string& path, const std::string& extension, std::function<const void(const std::string&)> callback, const bool fixPath = true, const bool reverse = false);
        void enumerateFiles(const std::string& path, const std::string& extension, std::vector<std::string>& paths, const bool fixPath = true, const bool reverse = false);
        bool fileExists(const std::string& path);
        bool dirExists(const std::string& path);

        bool isFile(const std::string& path);
        bool isDir(const std::string& path);

        void delDirRecursively(const std::string& path);
        void copyRecursively(const std::filesystem::path& src, const std::filesystem::path& target) noexcept;

        void fixPath(std::string& path);
        void fixPath(char* path);
    }
}