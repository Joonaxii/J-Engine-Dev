#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <filesystem>
#include <JEngine/Assets/AssetDataType.h>
#define NAMEOF(name) #name

namespace JEngine {
    struct membuf : std::streambuf
    {
        membuf(char* p, size_t size) {
            setp(p, p + size);
            setg(p, p, p + size);
        }
        size_t written() { return pptr() - pbase(); }

    protected:
        virtual pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in) {
            switch (dir)
            {
                case std::ios_base::cur:
                    gbump(int(off));
                    break;
                case std::ios_base::end:
                    setg(eback(), egptr() + off, egptr());
                    break;
                case std::ios_base::beg:
                    setg(eback(), eback() + off, egptr());
                    break;
            }
            return gptr() - eback();
        }
    };

    namespace IO {
        AssetDataFormat checkFileType(std::istream& stream);

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