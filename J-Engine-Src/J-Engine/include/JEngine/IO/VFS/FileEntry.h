#pragma once
#include <string>
#include <vector>
#include <JEngine/Assets/AssetMetaData.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/IO/Serialization/Serializable.h>

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

        static constexpr char* ASSET_CACHE_HDR = "ASDB";
        static constexpr char* ASSET_CACHE_EXT = ".resdb";

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
        IAsset* findAssetByUUID(const UUID& uuid);

        void eraseEntry(const bool eraseRoot);
        void erasePath(PathEntry& path);

        void addPath(PathEntry& path, FileEntry*& entry, bool& addNew);
        void addPath(std::vector<std::string_view>& strs, PathEntry& path, FileEntry*& entry, bool& addNew);

        void addPaths(std::vector<PathEntry>& paths);
        void addPaths(std::vector<PathEntry>& paths, std::vector<FileEntry*>& entries);
        const void getPath(std::string& path);
        FileEntry* getParent();

        void loadFromAssetDB(std::istream& stream);
        void saveToAssetDB(std::ostream& stream);

        void loadFromPath(const std::string& path);
        void validateMetaData(const std::string& root);

        void clear();

        const uint8_t getEntryType() const;
        const uint32_t getDataPosition() const;

        const int32_t indexOf(const std::string& name) const;
        const int32_t indexOf(const std::string& name, const uint8_t type) const;

        const int32_t indexOf(const std::string_view& name) const;
        const int32_t indexOf(const std::string_view& name, const uint8_t type) const;
        const void print();

        const std::vector<FileEntry>& getItems();

        static const std::string entryTypeToStr(uint8_t input);

        AssetMetaData& getMetaData();
        const AssetMetaData& getConstMetaData() const;
        FileEntry::EntryData& getEntryData();

    private:
        FileEntry* _parent;
        std::string _name;
        EntryData _data;
        AssetMetaData _metadata;
        std::vector<FileEntry> _items;

        const void printSub(int depth);
        void readMetaDataFromDB(std::istream& stream, std::vector<std::string_view>& strs);
        uint32_t writeMetaDataToDB(std::ostream& stream);

        void writeMetadataToJson(const std::string& root);
        void validateMetaDataRecurse(const std::string& root, std::vector<FileEntry*>& generate);
    };


#pragma region Serialization
    template<>
    inline const bool Serializable<FileEntry::EntryData>::deserializeBinary(FileEntry::EntryData& itemRef, std::istream& stream) {
        Serialization::deserialize(itemRef.type, stream);
        Serialization::deserialize(itemRef.pak, stream);
        Serialization::deserialize(itemRef.position, stream);
        return true;
    }

    template<>
    inline const bool Serializable<FileEntry::EntryData>::serializeBinary(const FileEntry::EntryData& itemRef, std::ostream& stream) {
        Serialization::serialize(itemRef.type, stream);
        Serialization::serialize(itemRef.pak, stream);
        Serialization::serialize(itemRef.position, stream);
        return true;
    }
#pragma endregion
}