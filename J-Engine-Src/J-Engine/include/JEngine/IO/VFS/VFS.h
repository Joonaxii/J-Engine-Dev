#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <JEngine/Collections/PoolAllocator.h>
#include <JEngine/Collections/ReferenceVector.h>
#include <JEngine/IO/Helpers/IOUtils.h>
#include <JEngine/IO/FileStream.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Cryptography/UUID.h>

namespace JEngine {
    struct EntryInfo {
        static constexpr size_t NPOS = SIZE_MAX;

        size_t position{ 0 };
        size_t size{ NPOS };

        constexpr EntryInfo() noexcept : position(0), size(NPOS) {}
        constexpr EntryInfo(size_t pos, size_t size) noexcept : position(pos), size(size) {}

        constexpr bool isFolder() const { return (position & size) == NPOS; }
        constexpr bool isFile() const { return (position != NPOS && size == NPOS); }

        constexpr bool operator==(const EntryInfo & other) const { return position == other.position && size == other.position; }
        constexpr bool operator!=(const EntryInfo & other) const { return position != other.position || size != other.position; }
    };
    static constexpr EntryInfo FOLDER_EINF(EntryInfo::NPOS, EntryInfo::NPOS);
    static constexpr EntryInfo FILE_EINF(0, EntryInfo::NPOS);
    static constexpr EntryInfo RUNTIME_EINF(EntryInfo::NPOS, 0);

    struct FileEntry {
        static constexpr size_t PATH_HASH_BLOCK_SIZE = 8;

        FileEntry* parent{nullptr};
        UUID8 hash{};
        int32_t source{-1};
        int32_t index{-1};
        EntryInfo dataInfo{};
        std::string name{};
        std::vector<FileEntry*> children{};

        bool isFolder() const { return dataInfo.isFolder(); }
        bool isFile() const { return dataInfo.isFile(); }

        void setup(FileEntry* parent, ConstSpan<char> name, UUID8 hash, int32_t index, int32_t source, EntryInfo info) {
            this->parent = parent;
            this->source = source;
            this->hash = hash;
            this->index = index;
            this->dataInfo = info;
            this->name = std::string(name.get(), name.length());
            children.clear();
        }

        FileEntry* findInChild(ConstSpan<char> span) const {
            for (FileEntry* child : children) {
                if (child->isSame(span)) { return child; }
            }
            return nullptr;
        }

        bool isSame(ConstSpan<char> span) const {
            return IO::pathsAreEqual(span, ConstSpan<char>(name.c_str(), name.size()));
        }

        size_t getPathLength(bool includeRoot) const {
            size_t len = 0;
            const FileEntry* entry = this;
            while (entry) {
                if (!includeRoot && entry->parent == nullptr) { break; }
                len += entry->name.length() + 1;
                entry = entry->parent;
            }
            return len;
        }

        bool isSubFile() const {
            const FileEntry* entry = parent;
            while (entry) {
                if (entry->isFile()) { return true; }
                entry = entry->parent;
            }
            return false;
        }

        size_t getDepth(bool includeRoot) const {
            size_t depth = 0;
            const FileEntry* entry = this;
            while (entry) {
                if (!includeRoot && entry->parent == nullptr && depth > 0) { break; }
                depth++;
                entry = entry->parent;
            }
            return depth - 1;
        }

        size_t buildPath(bool includeRoot, char* buffer) const {
            size_t depth = getDepth(includeRoot);
            const FileEntry** eBuf = reinterpret_cast<const FileEntry**>(_malloca((depth + 1) * sizeof(FileEntry*)));
            if (!eBuf) { return 0; }

            size_t pos = depth;
            const FileEntry* entry = this;
            while (entry) {
                if (!includeRoot && entry->parent == nullptr) { break; }
                eBuf[pos--] = entry;
                entry = entry->parent;
            }

            pos = 0;
            for (size_t i = 0; i <= depth; i++) {
                *buffer++ = '/';
                const std::string& eName = eBuf[i]->name;
                memcpy(buffer, eName.c_str(), eName.length());
                buffer += eName.length();
                pos += eName.length() + 1;
            }
            *buffer = 0;
            _freea(eBuf);
            return pos;
        }

        size_t findLongestPath(bool includeRoot, size_t initSize = 0) const {
            initSize = std::max(initSize, getPathLength(includeRoot));

            for (auto ch : children) {
                initSize = ch->findLongestPath(includeRoot, initSize);
            }
            return initSize;
        }

        void updateHash() {
            HashState state{};
            hash = parent ? parent->hash : UUID8::Empty;

            for (auto ch : children) {
                ch->updateHash();
            }
        }
    };

	class VFS {
	public:
        enum : uint8_t {
            F_ACT_NONE   = 0x00,
            F_ACT_REMOVE = 0x01,
            F_ACT_CLOSE  = 0x02,
        };

        VFS();
        VFS(const std::string& root);
        ~VFS();

        void changeRoot(const std::string& root);
        const std::string& getRootPath() const { return _rootPath; }
        const FileEntry* getRoot() const { return _root; }
        size_t getEntryCount() const { return _entries.size(); }
        size_t getFileCount() const { return _files.size(); }

        bool buildFromRoot();

        int32_t addEntry(int32_t source, bool isFile, const char* path, size_t length, EntryInfo dataInfo = EntryInfo(), bool* addedNew = nullptr);
        int32_t addEntry(int32_t source, bool isFile, const char* path, EntryInfo dataInfo = EntryInfo(), bool* addedNew = nullptr);
        int32_t addEntry(int32_t source, bool isFile, const std::string& path, EntryInfo dataInfo = EntryInfo(), bool* addedNew = nullptr);

        bool removeEntry(const char* path, size_t length, uint8_t fileAction = F_ACT_REMOVE);
        bool removeEntry(const char* path, uint8_t fileAction = F_ACT_REMOVE);
        bool removeEntry(const std::string& path, uint8_t fileAction = F_ACT_REMOVE);

        FileStream* getSourceFile(int32_t index) { return index < 0 || index >= _files.tail() ? nullptr : _files[index]; }
        const FileStream* getSourceFile(int32_t index) const { return index < 0 || index >= _files.tail() ? nullptr : _files[index]; }

        FileEntry* getFileEntry(int32_t index) { return index < 0 || index >= _entries.tail() ? nullptr : _entries[index]; }
        const FileEntry* getFileEntry(int32_t index) const { return index < 0 || index >= _entries.tail() ? nullptr : _entries[index]; }

        int32_t addSourceFile(const char* path, size_t length);
        int32_t addSourceFile(const char* path);
        int32_t addSourceFile(const std::string& path);

        bool removeSourceFile(const char* path, size_t length);
        bool removeSourceFile(const char* path);
        bool removeSourceFile(const std::string& path);
        bool removeSourceFile(int32_t index);

        void openSourceFile(int32_t index, const char* mode);
        void closeSourceFile(int32_t index);

        int32_t indexOfFileEntry(ConstSpan<char> path) const;
        int32_t indexOfFileEntry(UUID8 pathUUID) const;
        int32_t indexOfSourceFile(ConstSpan<char> path) const;

        bool pathExists(ConstSpan<char> path) const;
        bool pathExists(const char* path, size_t length) const;
        bool pathExists(const char* path) const;
        bool pathExists(const std::string& path) const;

        UUID8 pathToHash(ConstSpan<char> path, bool autoAppendRoot = true) const;
        UUID8 pathToCleanAndHash(ConstSpan<char> path, bool autoAppendRoot = true) const;

        void removeMissing();

	private:
        ConstSpan<char> _rootSpan;
        std::string _rootPath;
        FileEntry* _root;

        PoolAllocator<FileEntry> _entryAllocator;
        PoolAllocator<FileStream> _fileAllocator;

        ReferenceVector<FileEntry*> _entries;
        ReferenceVector<FileStream*> _files;
        std::unordered_map<UUID8, int32_t, std::hash<UUID8>> _fileEntryLUT;

        void removeEntry(FileEntry* entry, uint8_t fileAction);
    };
}