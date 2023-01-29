#include <JEngine/Helpers/StringExtensions.h>
#include <JEngine/Helpers/TypeHelpers.h>
#include <JEngine/IO/Helpers/FileExtensions.h>
#include <JEngine/IO/VFS/FileEntry.h>
#include <stack>
#include <fstream>

namespace JEngine {
    FileEntry::FileEntry() : FileEntry(nullptr, std::string_view(), EntryData()) { }
    FileEntry::FileEntry(FileEntry* parent, const std::string& name, const EntryData& data) : _parent(parent), _items(0), _name(name), _data(data) { }
    FileEntry::FileEntry(FileEntry* parent, const std::string_view& name, const EntryData& data) : _parent(parent), _items(0), _name(name), _data(data) { }

    const std::string& FileEntry::getName() const {
        return _name;
    }

    FileEntry* FileEntry::findOrAdd(std::string_view& path, const EntryData& data, bool& addNew) {
        int32_t indOf = indexOf(path);
        *&addNew = indOf < 0;
        if (addNew) {
            indOf = int32_t(_items.size());
            _items.push_back(FileEntry(this, path, data));
        }
        return &_items[indOf];
    }

    FileEntry* FileEntry::findByPath(const std::string& path) {
        std::string pth = path;
        const size_t last = pth.rfind("../");
        if (last != std::string::npos) {
            pth = pth.substr(last + 3, pth.length() - last - 3);
        }

        std::vector<std::string_view> strs;
        auto prev = this;
        Helpers::strSplit(pth, strs, "/");
        if (strs.size() < 1) {
            return nullptr;
        }

        FileEntry* entry = this;
        for (size_t i = 0; i < strs.size(); i++) {
            auto ind = entry->indexOf(strs[i]);
            if (ind < 0) { return nullptr; }
            entry = &entry->_items[i];
        }
        return entry;
    }

    IAsset* FileEntry::findAssetByUUID(const UUID& uuid) {
        for (size_t i = 0; i < _items.size(); i++) {
            auto& item = _items[i];
            if (item._data.getAsset && (uuid == item._metadata.getUUID())) { return item._data.getAsset(); }

            auto asset = item.findAssetByUUID(uuid);
            if (asset) { return asset; }
        }
        return nullptr;
    }

    void FileEntry::eraseEntry(const bool eraseRoot) {
        for (size_t i = 0; i < _items.size(); i++) {
            _items[i].eraseEntry(false);
        }

        if (_parent && eraseRoot && _parent->_items.size() > 0) {
            for (size_t i = _parent->_items.size() - 1; i >= 0; i--) {
                auto& ent = _parent->_items[i];
                if (ent._name == _name) {
                    _parent->_items.erase(_parent->_items.begin() + i);
                }
            }
        }
    }

    void FileEntry::erasePath(PathEntry& path) {
        std::vector<std::string_view> strs;

        auto& st = path.path;
        const size_t last = st.rfind("../");
        if (last != std::string::npos) {
            st = st.substr(last + 3, st.length() - last - 3);
        }

        Helpers::strSplit(st, strs, "/");
        if (strs.size() < 1) {
            return;
        }

        int32_t id = -1;
        FileEntry& entry = *this;
        for (size_t i = 0; i < strs.size(); i++) {
            id = entry.indexOf(strs[i]);
            if (id < 0) { return; }
            entry = _items[id];
        }
        entry.eraseEntry(true);
    }

    void FileEntry::addPath(PathEntry& path, FileEntry*& entry, bool& addNew) {
        std::vector<std::string_view> strs;
        addPath(strs, path, entry, addNew);
    }

    void FileEntry::addPath(std::vector<std::string_view>& strs, PathEntry& path, FileEntry*& entry, bool& addNew) {
        auto& st = path.path;
        const size_t last = st.rfind("../");
        if (last != std::string::npos) {
            st = st.substr(last + 3, st.length() - last - 3);
        }

        auto prev = this;
        Helpers::strSplit(st, strs, "/");
        if (strs.size() < 1) {
            return;
        }

        for (size_t i = 0; i < strs.size(); i++) {
            const bool isLast = (i >= strs.size() - 1);
            auto item = strs[i] == _name ? this : prev->findOrAdd(strs[i], isLast ? path.data : EntryData(ET_FOLDER), addNew);

            if (!item) { break; }
            prev = item;
        }
        *&entry = prev;
    }

    void FileEntry::addPaths(std::vector<PathEntry>& paths) {
        std::vector<FileEntry*> entries;
        addPaths(paths, entries);
    }

    void FileEntry::addPaths(std::vector<PathEntry>& paths, std::vector<FileEntry*>& entries) {
        std::vector<std::string_view> strs;

        FileEntry* ptr = nullptr;
        bool add = false;
        for (size_t i = 0; i < paths.size(); i++) {
            ptr = nullptr;
            addPath(strs, paths[i], ptr, add);
            if (ptr) {
                entries.push_back(ptr);
            }
            strs.clear();
        }
    }

    const void FileEntry::getPath(std::string& path) {
        std::stack<std::string*> paths;
        paths.push(&_name);
        size_t len = 0;

        auto parent = _parent;
        while (parent) {
            paths.push(&parent->_name);
            len += parent->_name.length() + 1;
            parent = parent->_parent;
            if (parent && !parent->_parent) { break; }
        }

        path.clear();
        path.reserve(len);
        while (!paths.empty()) {
            path.append("/");
            path.append(*paths.top());
            paths.pop();
        }
    }

    FileEntry* FileEntry::getParent() {
        return _parent;
    }

    void FileEntry::loadFromAssetDB(std::istream& stream) {
        char hdr[4]{ 0 };
        stream.read(hdr, 4);

        if (strncmp(hdr, ASSET_CACHE_HDR, 4) != 0) { return; }
        uint32_t count = 0;
        Serialization::deserializeBinary(stream, count);
        std::vector<std::string_view> strs;

        for (size_t i = 0; i < count; i++) {
            readMetaDataFromDB(stream, strs);
        }
    }

    void FileEntry::saveToAssetDB(std::ostream& stream) {
        stream.write(ASSET_CACHE_HDR, 4);
        uint32_t count = 0;
        auto pos = stream.tellp();
        Serialization::serializeBinary(stream, count);

        count = writeMetaDataToDB(stream);
        auto end = stream.tellp();

        stream.seekp(pos, std::ios::beg);
        Serialization::serializeBinary(stream, count);
        stream.seekp(end, std::ios::beg);
    }

    void FileEntry::loadFromPath(const std::string& path) {
        if (!IO::dirExists(path)) {
            std::cout << "Directory '" << path << "' doesn't exists or cannot be accessed!\n";
            return;
        }

        std::vector<PathEntry> files{};
        IO::enumerateFiles(path, ".meta",
            [&](const std::string& path) -> const void {
                files.push_back(PathEntry(path, EntryData(ET_FILE)));
            }, true, true);
        addPaths(files);
    }

    void FileEntry::validateMetaData(const std::string& root) {
        std::vector<FileEntry*> gen;
        validateMetaDataRecurse(root, gen);
    }

    void FileEntry::clear() {
        _items.clear();
        std::vector<FileEntry>().swap(_items);
        _name = "";
    }

    const uint8_t FileEntry::getEntryType() const {
        return _data.type;
    }

    const uint32_t FileEntry::getDataPosition() const {
        return _data.position;
    }

    const int32_t FileEntry::indexOf(const std::string& name) const {
        for (size_t i = 0; i < _items.size(); i++) {
            if (_items[i]._name == name) { return int32_t(i); }
        }
        return -1;
    }

    const int32_t FileEntry::indexOf(const std::string& name, const uint8_t type) const {
        for (size_t i = 0; i < _items.size(); i++) {
            if (_items[i]._data.type == type && _items[i]._name == name) { return int32_t(i); }
        }
        return -1;
    }

    const int32_t FileEntry::indexOf(const std::string_view& name) const {
        for (size_t i = 0; i < _items.size(); i++) {
            if (_items[i]._name == name) { return int32_t(i); }
        }
        return -1;
    }

    const int32_t FileEntry::indexOf(const std::string_view& name, const uint8_t type) const {
        for (size_t i = 0; i < _items.size(); i++) {
            if (_items[i]._data.type == type && _items[i]._name == name) { return int32_t(i); }
        }
        return -1;
    }

    const void FileEntry::print() {
        printSub(0);
        for (size_t i = 0; i < _items.size(); i++) {
            _items[i].printSub(1);
        }
        return void();
    }

    const std::vector<FileEntry>& FileEntry::getItems() {
        return _items;
    }

    const std::string FileEntry::entryTypeToStr(uint8_t input) {
        switch (input)
        {
        default: return "UNKNOWN";
        case ET_FOLDER: return NAMEOF(ET_FOLDER);
        case ET_FILE: return NAMEOF(ET_FILE);
        case ET_PAK_FILE: return NAMEOF(ET_PAK_FILE);
        case ET_ASSET: return NAMEOF(ET_ASSET);
        }
    }

    AssetMetaData& FileEntry::getMetaData() {
        return _metadata;
    }

    const AssetMetaData& FileEntry::getConstMetaData() const {
        return _metadata;
    }

    FileEntry::EntryData& FileEntry::getEntryData() {
        return _data;
    }

    const void FileEntry::printSub(int depth) {
        std::cout << "|-" << std::string(depth << 1U, '-') << _name << " [" << entryTypeToStr(_data.type) << "]\n";
        for (size_t i = 0; i < _items.size(); i++) {
            _items[i].printSub(depth + 1);
        }
    }

    void FileEntry::readMetaDataFromDB(std::istream& stream, std::vector<std::string_view>& strs) {
        std::string path = Serialization::deserializeStringBinary(stream);
        EntryData data;
        data.deserializeBinary(stream);

        AssetMetaData meta;
        meta.deserializeBinary(stream);
        FileEntry* entry;
        bool addNew = false;

        auto pth = PathEntry(path, data);
        addPath(strs, pth, entry, addNew);
        strs.clear();

        if (entry) {
            entry->_metadata = meta;
        }
    }

    uint32_t FileEntry::writeMetaDataToDB(std::ostream& stream) {
        uint32_t c = 0;
        if (_data.type != ET_FOLDER) {
            std::string path = "";
            getPath(path);
            Serialization::serializeStringBinary(stream, path);
            _data.serializeBinary(stream);
            UUID uuid = _metadata.getUUID();
            if (uuid == UUIDFactory::UUID_EMPTY) {
                UUIDFactory::generateUUID<IAsset>(uuid);
                _metadata.setUUID(uuid);
            }
            _metadata.serializeBinary(stream);
            c++;
        }

        for (size_t i = 0; i < _items.size(); i++) {
            c += _items[i].writeMetaDataToDB(stream);
        }
        return c;
    }

    void FileEntry::writeMetadataToJson(const std::string& root) {
        std::string pth;
        getPath(pth);
        std::string metaPath = root + pth + ".meta";

        json jsonF;
        _metadata.serializeJson(jsonF);
        try {
            std::ofstream outStr(metaPath, std::ofstream::out | std::ofstream::trunc);
            outStr.exceptions(std::ios_base::badbit | std::ios_base::failbit);

            outStr << jsonF.dump(4) << std::endl;
            outStr.close();
        }
        catch (std::ios_base::failure exc) {
            std::cout << "Failed to write JSON '" << exc.what() << "'!\n";
        }
    }

    void FileEntry::validateMetaDataRecurse(const std::string& root, std::vector<FileEntry*>& generate) {
        if (_data.type != ET_FOLDER) {
            std::string pth;
            getPath(pth);

            std::string metaPath = root + pth + ".meta";

            bool valid = IO::fileExists(metaPath);
            std::ifstream strm;
            if (valid) {
                strm.open(metaPath);
                valid = Serialization::checkFileType(strm, "NONE") == Serialization::JSON_FILE;
            }

            if (valid) {
                strm.seekg(0, std::ios::beg);
                json& jsonF = json::parse(strm);
                _metadata.deserializeJson(jsonF);
                strm.close();
                UUIDFactory::addUUID<IAsset>(_metadata.getUUID());
            }
            else {
                generate.push_back(this);
            }
            strm.close();
        }

        for (size_t i = 0; i < _items.size(); i++) {
            _items[i].validateMetaDataRecurse(root, generate);
        }
    }
}