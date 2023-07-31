#include <JEngine/Core/AssetDB.h>

namespace JEngine {
    AssetDB::AssetDB(const ResourceRoot* roots, size_t rootCount) : _allVfs() {
        for (size_t i = 0; i < rootCount; i++) {
            const auto& root = roots[i];
            if (indexOfVFS(root.type) < 0) {
                _allVfs.emplace_back(root.type, new VFS(root.root));
            }
        }
    }
    AssetDB::~AssetDB() {
        for (auto& vfs : _allVfs) {
            delete vfs.vfs;
        }
        _allVfs.clear();
    }
    void AssetDB::refresh(uint8_t refreshMode) {
    }
    void AssetDB::buildVFS(uint32_t types) {
        for (uint32_t i = 0, j = 1; i < 2; i++, j <<=1) {
            if ((types & j)) {
                if (int32_t ind = indexOfVFS(ResourceRoot::RootType(j)) > -1) {
                    auto vfs = _allVfs[ind].vfs;
                    if (vfs->buildFromRoot()) {
                        vfs->removeMissing();
                    }
                }
            }
        }
    }

    IAsset* AssetDB::createAsset(const UUID16& type, const char* path, uint8_t flags, ResourceRoot::RootType vfsType) {
        int32_t ind = indexOfVFS(vfsType);
        if (ind < 0) {
            JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Couldn't create asset '{0}', given VFS type wasn't found!", path);
            return nullptr;
        }

        VFS* vfs = _allVfs[ind].vfs;
        FileEntry* entry = vfs->getFileEntry(vfs->indexOfFileEntry(path));

        if (entry) {
            JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Asset with path '{0}' already exists!", path);
            return nullptr;
        }
        bool addedNew = false;

        ind = vfs->addEntry(-1, true, path, RUNTIME_EINF, &addedNew);
        if (ind < 0) {
            JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Failed to add path '{0}' to VFS!", path);
            return nullptr;
        }

        if (!addedNew) {
            JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Asset with path '{0}' already exists in VFS!", path);
            return nullptr;
        }

        FileEntry* fEntry = vfs->getFileEntry(ind);
        const char* namePtr = fEntry->name.c_str();
        char* temp = nullptr;
        ind = ConstSpan<char>(namePtr, fEntry->name.length()).indexOfLast('.');

        if (ind > -1) {
            size_t newLen = ind + 1;
            temp = reinterpret_cast<char*>(_malloca(newLen));
            if (temp) {
                memcpy(temp, namePtr, newLen);
                temp[ind] = 0;
                namePtr = temp;
            }
        }
        detail::Asset* assetDet = detail::Asset::getAssetByHash(type);
        if (!assetDet) {
            JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Failed to add asset '{0}', asset type doesn't seem to be registered!", path);
            return nullptr;
        }

        UUID8 uuid{};
        UUIDFactory::generateUUID<IAsset>(uuid);

        IAsset* asset = assetDet->createAsset(namePtr, fEntry->hash, flags);
        if (!asset) {
            JENGINE_CORE_WARN("[J-Engine - AssetDB] Warning: Failed to create asset '{0}', something went wrong when creating a new instance!", path);
            UUIDFactory::removeUUID<IAsset>(uuid);
            return nullptr;
        }

        _assets.pushArgs(asset, vfs, fEntry);
        if (temp) {
            _freea(temp);
        }
        return asset;
    }

    FileEntry* AssetDB::findFromVFS(const char* path, ResourceRoot::RootType types) const {
        if (!path) { return nullptr; }
        size_t len = strlen(path);

        if (len == 0) { return nullptr; }

        char* pathBuf = reinterpret_cast<char*>(_malloca(len + 1));
        if (pathBuf) {
            memcpy(pathBuf, path, len);
            pathBuf[len] = 0;
            IO::fixPath(pathBuf, len);

            ConstSpan<char> pathSpn(pathBuf, len);
            for (auto& vfs : _allVfs) {
                if (vfs.type & types) {
                    int32_t ind = vfs.vfs->indexOfFileEntry(vfs.vfs->pathToHash(pathSpn));
                    if (ind > -1) {
                        _freea(pathBuf);
                        return vfs.vfs->getFileEntry(ind);
                    }
                }
            }
            _freea(pathBuf);
        }
        return nullptr;
    }
}