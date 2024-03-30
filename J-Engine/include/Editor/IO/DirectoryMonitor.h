#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>

namespace JEngine {
    struct DirectoryChange {
        enum ChangeType : uint8_t {
            TYPE_NONE,
            TYPE_RENAME,
            TYPE_DELETE,
            TYPE_CREATE,
            TYPE_MODIFIED,
        };

        ChangeType type{};
        std::string pathFrom{};
        std::string pathTo{};

        DirectoryChange() : type{}, pathFrom{}, pathTo{}{}
        DirectoryChange(ChangeType type, const std::string& path) : type(type), pathFrom(path), pathTo{}{}
        DirectoryChange(const std::string& from, const std::string& to) : type(TYPE_RENAME), pathFrom(from), pathTo(to) {}
    };

    class DirectoryMonitor {
    public:
        DirectoryMonitor();
        ~DirectoryMonitor();

        void close();
        bool tryOpen(const std::string& path);
        bool isOpen() const { return (_flags & FLAG_OPEN) != 0; }

        bool poll(std::vector<DirectoryChange>& changes);

    private:
        static constexpr size_t CHANGE_BUFFER_SIZE = 2048;

        enum : uint8_t {
            FLAG_NONE = 0x00,
            FLAG_OPEN = 0x01,
        };
        uint8_t _flags;
        HANDLE _handle;
        OVERLAPPED _overlapped;
        char _changeBuffer[CHANGE_BUFFER_SIZE];
    };
}