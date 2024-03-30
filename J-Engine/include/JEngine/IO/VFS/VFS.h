#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <JEngine/Collections/IndexStack.h>
#include <JEngine/IO/Helpers/IOUtils.h>
#include <JEngine/IO/FileStream.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Utility/DataFormatUtils.h>

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

    struct FileID {
    public:
        static constexpr uint32_t FOLDER_MASK = 0x80'00'00'00U;
        static constexpr uint32_t ID_MASK    = ~FOLDER_MASK;
        static constexpr uint32_t NULL_ID    = UINT32_MAX;

        constexpr FileID() : _id(ID_MASK) {}
        constexpr FileID(uint32_t id) : _id(id) {}
        constexpr FileID(uint32_t index, bool isFolder) : _id((index & ID_MASK) | (isFolder ? FOLDER_MASK : 0x00U)) {}

        constexpr bool isFolder() const { return (_id & FOLDER_MASK) != 0; }
        constexpr bool isValid() const { return (_id & ID_MASK) != ID_MASK; }
        constexpr uint32_t getID() const { return (_id & ID_MASK); }

        constexpr bool operator==(const FileID& other) const { return _id == other._id; }
        constexpr bool operator!=(const FileID& other) const { return _id != other._id; }

    private:
        uint32_t _id;
    };

    struct FileRef {
        FileID id{};
        uint32_t index{};

        constexpr FileRef() : id(), index(UINT32_MAX) {}
        constexpr FileRef(FileID parent, uint32_t index) : id(parent), index(index) {}
    };

    class VFS;
    struct FileEntry {
        static constexpr uint32_t NULL_SOURCE = UINT32_MAX;

        FileID parent{};
        FileID id{};
        uint32_t source{ NULL_SOURCE };
        EntryInfo dataInfo{};
        std::string name{};

        std::vector<FileID> children{};

        FileEntry() = default;

        bool isFolder() const { return id.isFolder(); }
        bool isFile() const { return !id.isFolder(); }

        void setup(FileID id, FileID parent, ConstSpan<char> name, uint32_t source, EntryInfo info) {
            this->parent = parent;
            this->id = id;
            this->source = source;
            this->dataInfo = info;
            this->name = std::string(name.get(), name.length());
            children.clear();
        }

        bool isSame(ConstSpan<char> span) const {
            return IO::pathsAreEqual(span, ConstSpan<char>(name));
        }

        FileID findInChild(const VFS& vfs, ConstSpan<char> span) const;
        size_t getPathLength(const VFS& vfs, bool includeRoot) const;

        bool isSubFile(const VFS& vfs) const;

        /// <summary>
        /// This mehod will also remove this item from it's parent
        /// </summary>
        void remove(VFS& vfs);

        size_t getDepth(const VFS& vfs, bool includeRoot) const;
        size_t buildPath(const VFS& vfs, bool includeRoot, char* buffer) const;
        size_t findLongestPath(const VFS& vfs, bool includeRoot, size_t initSize = 0) const;

        bool deleteIO(VFS& vfs);
        void getOrderedChildren(std::vector<FileID>& output) const;

    };

	class VFS {
	public:
        enum : uint32_t {
            VFS_FLAG_NONE = 0x00,
            VFS_FLAG_IS_VIRTUAL = 0x01,
        };

        enum : uint8_t {
            F_ACT_NONE   = 0x00,
            F_ACT_REMOVE = 0x01,
            F_ACT_CLOSE  = 0x02,
        };

        VFS();
        VFS(ConstSpan<char> root, uint32_t flags);
        ~VFS();

        void changeRoot(ConstSpan<char> root, uint32_t flags = VFS_FLAG_NONE);
        ConstSpan<char> getRootPath() const { return _rootPath; }

        FileEntry* getRoot() { return getEntryByRef(_root); }
        const FileEntry* getRoot() const { return getEntryByRef(_root); }

        size_t getEntryCount() const { return _entries.size(); }
        size_t getSourceCount() const { return _sources.size(); }

        bool buildFromRoot();

        FileID addEntry(uint32_t source, ConstSpan<char> path, EntryInfo dataInfo = EntryInfo(), bool* addedNew = nullptr);

        FileStream* getSourceFile(uint32_t index) { return index >= _sources.size() ? nullptr : &_sources[index]; }
        const FileStream* getSourceFile(uint32_t index) const { return index >= _sources.size() ? nullptr : &_sources[index]; }

        uint32_t addSourceFile(ConstSpan<char> path);

        bool removeSourceFile(ConstSpan<char> path);
        bool removeSourceFile(uint32_t index);

        void openSourceFile(uint32_t index, const char* mode);
        void closeSourceFile(uint32_t index);

        FileID indexOfFileEntry(ConstSpan<char> path) const;
        uint32_t indexOfSourceFile(ConstSpan<char> path) const;
 
        bool pathExists(ConstSpan<char> path) const;
        void removeMissing();

        FileEntry* getEntryByRef(FileID ref);
        const FileEntry* getEntryByRef(FileID ref) const;

        FileEntry* getEntryByPath(ConstSpan<char> path) { return getEntryByRef(indexOfFileEntry(path)); }
        const FileEntry* getEntryByPath(ConstSpan<char> path) const { return getEntryByRef(indexOfFileEntry(path)); }

        bool removeEntry(ConstSpan<char> path, uint8_t sourceAction = F_ACT_NONE);
        bool removeEntry(FileID entry, uint8_t sourceAction = F_ACT_NONE);

	private:

        uint32_t _flags;
        ConstSpan<char> _rootSpan;
        std::string _rootPath;
        FileID _root;

        IndexedLUT<FileEntry> _entries;
        IndexedLUT<FileStream> _sources;
    };
}