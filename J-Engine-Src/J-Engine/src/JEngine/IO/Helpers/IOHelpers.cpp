#include <JEngine/IO/Helpers/IOHelpers.h>
#include <fstream>
#include <sys/stat.h>

namespace JEngine::IO {
    AssetDataFormat checkFileType(std::istream& stream) {
        //Check if a known binary format
        char hdr[5] { 0, 0, 0, 0, 0 };
        stream.read(hdr, 4);
        stream.seekg(0, std::ios::beg);

        AssetDataFormat fmt = headerToAssetFormat(hdr);
        if (fmt != AssetDataFormat::Unknown) { return fmt; }

        //Check for JSON
        {
            char c = '\0';
            while (!stream.eof()) {
                stream.read(&c, 1);
                if (c == '{') { break; }
                if (!std::isspace(c)) { break; }
            }

            if (c == '{') {
                int off = 1;
                stream.seekg(-1, std::ios::end);
                auto len = stream.tellg();
                while (!stream.eof()) {
                    stream.read(&c, 1);

                    if (c == '}') { break; }
                    if (!std::isspace(c)) { break; }

                    off++;
                    stream.seekg(-off, std::ios::end);
                }

                stream.seekg(0, std::ios::beg);
                if (c == '}') { return AssetDataFormat::JSON; }
            }
        }
        return AssetDataFormat::Unknown;
    }

    void enumerateFiles(const std::string& path, const std::string& extension, std::function<const void(const std::string&)> callback, const bool fixPath, const bool reverse) {
        if (!dirExists(path)) {
            std::cout << "Path '" << path << "' was not found!\n";
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
                auto& str = entry.path().string();
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
                auto& str = entry.path().string();
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

    bool fileExists(const std::string& path) {
        std::ifstream f(path.c_str());
        return f.good();
    }

    bool dirExists(const std::string& path) {
        struct stat info;

        if (stat(path.c_str(), &info) != 0) { return 0; }
        else if (info.st_mode & S_IFDIR) { return 1; }

        return 0;
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
        if (!dirExists(path)) { return; }
        std::filesystem::remove_all(path);
    }

    void copyRecursively(const std::filesystem::path& src, const std::filesystem::path& target) noexcept {
        try {
            std::filesystem::copy(src, target, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
        }
        catch (std::exception& e) {
            std::cout << e.what() << "\n";
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
}