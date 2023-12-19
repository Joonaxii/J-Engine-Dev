#include <JEngine/Core/AssetDB.h>
#include <JEngine/Core.h>

namespace JEngine {

    AssetDB::AssetDB() : _allSources{} { }
//
//#ifdef JENGINE_EDITOR
//#else
//    AssetDB::AssetDB(const std::string roots[SRC_COUNT]) : _allSources{} {
//        for (size_t i = 0; i < MAX_ASSET_SOURCES; i++) {
//            const auto& root = roots[i];
//            auto& source = _allSources[i];
//            if (root.length() > 3 && IO::exists(root)) {
//
//                continue;
//            }
//        }
//}
//#endif
//

    AssetDB::~AssetDB() {
        for (auto& vfs : _allSources) {

        }
    }

    void AssetDB::refresh(uint8_t source, uint8_t refreshMode) {
    }
    void AssetDB::buildVFS(uint32_t types) {
        for (uint32_t i = 0, j = 1; i < AssetSourceType::SRC_COUNT; i++, j <<= 1) {
            if ((types & j)) {
                //auto& vfs = _allSources[i].vfs;
                //if (vfs->buildFromRoot()) {
                //    vfs->removeMissing();
                //}
            }
        }
    }

    AssetRef AssetDB::createAsset(const UUID16& type, ConstSpan<char> path, uint8_t flags, uint8_t source) {
       //if (source >= AssetSourceType::SRC_COUNT) {
       //    JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Couldn't create asset '{0}', given VFS type was invalid!", path);
       //    return nullptr;
       //}
       //
       //VFS& vfs = _allSources[source].vfs;
       //FileEntry* entry = vfs.getEntryByPath(path);
       //
       //if (entry) {
       //    JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Asset with path '{0}' already exists!", path);
       //    return nullptr;
       //}
       //bool addedNew = false;
       //
       //FileID ind = vfs.addEntry(-1, true, path, RUNTIME_EINF, &addedNew);
       //if (ind < 0) {
       //    JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Failed to add path '{0}' to VFS!", path);
       //    return nullptr;
       //}
       //
       //if (!addedNew) {
       //    JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Asset with path '{0}' already exists in VFS!", path);
       //    return nullptr;
       //}
       //
       //FileEntry* fEntry = vfs.getFileEntry(ind);
       //const char* namePtr = fEntry->name.c_str();
       //char* temp = nullptr;        ind = ConstSpan<char>(namePtr, fEntry->name.length()).indexOfLast('.');
       //
       //if (ind > -1) {
       //    size_t newLen = ind + 1;
       //    temp = reinterpret_cast<char*>(_malloca(newLen));
       //    if (temp) {
       //        memcpy(temp, namePtr, newLen);
       //        temp[ind] = 0;
       //        namePtr = temp;
       //    }
       //}
       //detail::Asset* assetDet = detail::Asset::getAssetByHash(type);
       //if (!assetDet) {
       //    JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Failed to add asset '{0}', asset type doesn't seem to be registered!", path);
       //    return nullptr;
       //}
       //
       //IAsset* asset = assetDet->createAsset(, flags);
       //if (!asset) {
       //    JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Failed to create asset '{0}', something went wrong when creating a new instance!", path);
       //    return nullptr;
       //}
       //
       ////_allSources[source].assets.pushArgs(asset, vfs, fEntry);
       //if (temp) {
       //    _freea(temp);
       //}
       //return asset;
        return nullptr;
    }

    const FileEntry* AssetDB::findFromVFS(ConstSpan<char> path, uint8_t source) const {
        if (source >= AssetSourceType::SRC_COUNT) { return nullptr; }
        if (path.length() < 1) { return nullptr; }
        size_t len = path.length();

        if (len == 0) { return nullptr; }

        char* pathBuf = reinterpret_cast<char*>(_malloca(len + 1));
        if (pathBuf) {
            memcpy(pathBuf, path.get(), len);
            pathBuf[len] = 0;
            IO::fixPath({ pathBuf, len });

            auto& src = _allSources[source].vfs;
            FileID ind = src.indexOfFileEntry({ pathBuf, len });
            _freea(pathBuf);
            return src.getEntryByRef(ind);
        }
        return nullptr;
    }

    IAsset* AssetDB::getAssetByUUID(AssetRef uuid, bool fromDB) {
        if (!uuid.isValid()) { return nullptr; }

        uint32_t source = uuid.getSource();
        if (source >= SRC_COUNT) { return nullptr; }

        uint32_t index = uuid.getUUID();
        auto info = _allSources[source].assets.getAt(index);
        if (!info) { return nullptr; }
        return info->assetPtr;
    }

    VFS* AssetDB::getVFS(uint8_t index) {
        return index >= SRC_COUNT ? nullptr : &_allSources[index].vfs;
    }

    const VFS* AssetDB::getVFS(uint8_t index) const {
        return index >= SRC_COUNT ? nullptr : &_allSources[index].vfs;
    }

#ifdef JENGINE_EDITOR

    void AssetDB::initialize(const ConstSpan<char> roots[SRC_COUNT], const ConstSpan<char> dbRoots[SRC_COUNT]) {
        for (size_t i = 0; i < SRC_COUNT; i++) {
            if (i == SRC_RUNTIME) { continue; }
            fs::path root = std::string_view(roots[i].get(), roots[i].length());
            if (IO::exists_FS(root) && IO::isDir_FS(root) && IO::createDir(dbRoots[i])) {
                //TODO: Do actual VFS building/initialization etc
            }
        }
    }

    uint32_t AssetDB::packAssets(uint32_t sources, ConstSpan<char> destination) {
        if (!IO::exists(destination) || !IO::isDir(destination)) {
            JENGINE_CORE_ERROR("[AssetDB] Error: Cannot pack assets to '{0}', either it doesn't exist or it's not a directory path!", destination);
            return UINT32_MAX;
        }

        char pathTGT[513]{ 0 };
        size_t startLen = destination.length();
        memcpy(pathTGT, destination.get(), destination.length());
        IO::fixPath(Span<char>(pathTGT, startLen));
        if (pathTGT[startLen - 1] != '/') { pathTGT[startLen++] = '/'; }

        char* pakStart = pathTGT + startLen;
        size_t pakNameLen = sizeof(pathTGT) - startLen;

        int32_t count = 0;
        FileStream stream{};
        std::vector<uint32_t> assetsToWrite(512);
        for (uint32_t i = 0; i < AssetSourceType::SRC_COUNT; i++) {
            if ((1 << i) & sources) {
                assetsToWrite.clear();
                auto& src = _allSources[i];
            }
        }

        if (count < 1) {
            JENGINE_CORE_WARN("[AssetDB] Warning: Did not pack any asset sources!");
        }
        return count;
    }

    void AssetDB::VFSSource::selectPackableAssets(std::vector<uint32_t>& indices) {
        //for (size_t i = 0; i < length; i++) {
        //
        //}
    }

    void AssetDB::VFSSource::update() {
        static std::vector<DirectoryChange> changes{};
        if (dirMonitor.poll(changes)) {

        }
    }

    IAsset* AssetDB::getAssetByUUID(AssetRef uuid, bool fromDB) {
        if (!uuid.isValid()) { return nullptr; }

        uint32_t source = uuid.getSource();
        if (source >= SRC_COUNT) { return nullptr; }

        uint32_t index = uuid.getUUID();
        auto info = (fromDB ? _allSources[source].assetsDB : _allSources[source].assets).getAt(index);
        if (!info) { return nullptr; }
        return info->assetPtr;
    }

#else
    void JEngine::AssetDB::initialize(const std::string_view roots[SRC_COUNT]) {

    }

    IAsset* AssetDB::getAssetByUUID(AssetRef uuid) {
        if (!uuid.isValid()) { return nullptr; }

        uint32_t source = uuid.getSource();
        if (source >= SRC_COUNT) { return nullptr; }

        uint32_t index = uuid.getUUID();
        auto info = _allSources[source].assets.getAt(index);
        if (!info) { return nullptr; }
        return info->assetPtr;
    }

#endif

    void AssetDB::VFSSource::unload(bool fully) {

    }
}