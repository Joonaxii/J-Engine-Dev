#pragma once
#include <string>
#include <vector>
#include <JEngine/Assets/AssetMetaData.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/IO/Helpers/IOUtils.h>

namespace JEngine {
    class FileEntry {
    public:
        struct EntryData {
            uint8_t type;
            int8_t pak;
            uint32_t position;
            std::function<IAsset*(void)> getAsset;

            EntryData() : position(0), type(0), pak(-1), getAsset(nullptr) {}
            EntryData(const uint8_t type) : position(0), type(type), pak(-1), getAsset(nullptr){}
            EntryData(const uint8_t type, IAsset*(*get)(void)) : position(0), type(type), pak(-1), getAsset(get){}
        };

        struct PathEntry {
            std::string path;
            EntryData data;

            PathEntry() : path(""), data() {}
            PathEntry(const std::string& path, const EntryData& data) : path(path), data(data) {}
        };

        static constexpr const char* ASSET_CACHE_HDR = "ASDB";
        static constexpr const char* ASSET_CACHE_EXT = ".resdb";

        static constexpr uint8_t ET_FOLDER = 0x0;
        static constexpr uint8_t ET_FILE = 0x1;
        static constexpr uint8_t ET_PAK_FILE = 0x2;
        static constexpr uint8_t ET_ASSET = 0x3;

        FileEntry();
        FileEntry(FileEntry* parent, const std::string& name, const EntryData& data);
        FileEntry(FileEntry* parent, const std::string_view& name, const EntryData& data);

        const std::string& getName() const;
        FileEntry* findOrAdd(std::string_view& path, const EntryData& data, bool& addNew);

        FileEntry* findByPath(const std::string& path);
        IAsset* findAssetByUUID(const UUID8& uuid);

        void eraseEntry(const bool eraseRoot);
        void erasePath(PathEntry& path);

        void addPath(PathEntry& path, FileEntry*& entry, bool& addNew);
        void addPath(std::vector<std::string_view>& strs, PathEntry& path, FileEntry*& entry, bool& addNew);

        void addPaths(std::vector<PathEntry>& paths);
        void addPaths(std::vector<PathEntry>& paths, std::vector<FileEntry*>& entries);
        void getPath(std::string& path);
        FileEntry* getParent();

        void loadFromAssetDB(const Stream& stream);
        void saveToAssetDB(const Stream& stream);

        void loadFromPath(const std::string& path);
        void validateMetaData(const std::string& root);

        void clear();

        uint8_t getEntryType() const;
        uint32_t getDataPosition() const;

        int32_t indexOf(const std::string& name) const;
        int32_t indexOf(const std::string& name, const uint8_t type) const;

        int32_t indexOf(const std::string_view& name) const;
        int32_t indexOf(const std::string_view& name, const uint8_t type) const;
        void print();

        const std::vector<FileEntry>& getItems();

        static inline constexpr const char* entryTypeToStr(const uint8_t input) {
            switch (input)
            {
                default: return "ET_UNKNOWN";
                case ET_FOLDER: return "ET_FOLDER";
                case ET_FILE: return "ET_FILE";
                case ET_PAK_FILE: return "ET_PAK_FILE";
                case ET_ASSET: return "ET_ASSET";
            }
        }

        AssetMetaData& getMetaData();
        const AssetMetaData& getMetaData() const;
        FileEntry::EntryData& getEntryData();

    private:
        FileEntry* _parent;
        std::string _name;
        EntryData _data;
        AssetMetaData _metadata;
        std::vector<FileEntry> _items;

        void printSub(int depth);
        void readMetaDataFromDB(const Stream& stream, std::vector<std::string_view>& strs);
        uint32_t writeMetaDataToDB(const Stream& stream);

        void writeMetadataToJson(const std::string& root);
        void validateMetaDataRecurse(const std::string& root, std::vector<FileEntry*>& generate);
    };
}



#pragma region Serialization
//Binary
namespace JEngine {
    template<>
    inline bool Serializable<FileEntry::EntryData>::deserializeBinary(FileEntry::EntryData& itemRef, const Stream& stream) {
        Serialization::deserialize(itemRef.type, stream);
        Serialization::deserialize(itemRef.pak, stream);
        Serialization::deserialize(itemRef.position, stream);
        return true;
    }

    template<>
    inline bool Serializable<FileEntry::EntryData>::serializeBinary(const FileEntry::EntryData& itemRef, const Stream& stream) {
        Serialization::serialize(itemRef.type, stream);
        Serialization::serialize(itemRef.pak, stream);
        Serialization::serialize(itemRef.position, stream);
        return true;
    }
}
#pragma endregion