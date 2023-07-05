#include <JEngine/Core/AssetDB.h>

namespace JEngine {
    AssetDB::AssetDB(const ResourceRoot* roots, size_t rootCount) : _vfs() {
        for (size_t i = 0; i < rootCount; i++) {
            const auto& root = roots[i];
            if (_vfs.find(root.type) == _vfs.end()) {
                _vfs.insert(std::make_pair(root.type, uint32_t(_allVfs.size())));
                _allVfs.push_back(new VFS(root.root));
            }
        }
    }
    AssetDB::~AssetDB() {
        for (auto vfs : _allVfs) {
            delete vfs;
        }
        _allVfs.clear();
    }
    void AssetDB::refresh(uint8_t refreshMode) {
    }
    void AssetDB::buildVFS(uint32_t types) {
        for (uint32_t i = 0, j = 1; i < 2; i++, j <<=1) {
            auto find = _vfs.find(j);
            if ((types & j) && find != _vfs.end()) {
                auto vfs = _allVfs[find->second];
                if (vfs->buildFromRoot()) {
                    vfs->removeMissing();
                }
            }
        }
    }
}