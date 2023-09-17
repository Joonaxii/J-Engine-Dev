#pragma once
#include <JEngine/Cryptography/UUIDFactory.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/IAssetSerializer.h>
#include <JEngine/IO/VFS/VFS.h>

namespace JEngine {
    struct ResourceRoot {
        enum RootType : uint8_t {
            RT_EDITOR       = 0x01,
            RT_GAME         = 0x02,
            RT_GAME_ADD     = 0x04,
            RT_GAME_RUNTIME = 0x08,

            RT_ANY_GAME     = RT_GAME | RT_GAME_ADD | RT_GAME_RUNTIME,
            RT_ANY          = 0xFF,
        };

        std::string root{ "" };
        RootType type{};

        ResourceRoot() : root(), type() {}
        ResourceRoot(const char* root, RootType type) : root(root), type(type) {}
        ResourceRoot(const std::string& root, RootType type) : root(root), type(type) {}
    };

    class AssetDB {
    public:
        enum : uint8_t {
            REFRESH_SOFT,
            REFRESH_FULL,
        };

        AssetDB(const ResourceRoot* roots, size_t rootCount);
        ~AssetDB();

        int32_t indexOfVFS(ResourceRoot::RootType type) const {
            for (int32_t i = 0; i < _allVfs.size(); i++) {
                if (_allVfs[i].type & type) { return i; }
            }
            return -1;
        }

        const VFS* getVFS(ResourceRoot::RootType type) const { 
            int32_t ind = indexOfVFS(type);
            return ind < 0 ? nullptr : _allVfs[ind].vfs;
        }
        void refresh(uint8_t refreshMode = REFRESH_SOFT);
        void buildVFS(uint32_t types);

        FileEntry* findFromVFS(const char* path, ResourceRoot::RootType vfsTypes = ResourceRoot::RT_ANY_GAME) const;

        IAsset* createAsset(const UUID16& type, const char* path, uint8_t flags, ResourceRoot::RootType vfsType = ResourceRoot::RT_GAME_RUNTIME);

        template<typename T>
        T* createAsset(const char* path, uint8_t flags, ResourceRoot::RootType vfsType = ResourceRoot::RT_GAME_RUNTIME) {
            detail::Asset* assetDet = detail::Asset::getAsset<T>();
            return assetDet ? createAsset(assetDet->type->hash, path, flags, vfsType) : nullptr;
        }

        template<typename T>
        bool deleteAsset(T* asset) {
            if (!asset) { return false; }



            return true;
        }

    private:
        struct VFSSource {
            ResourceRoot::RootType type{};
            VFS* vfs{nullptr};

            VFSSource() : type(), vfs(nullptr) {}
            VFSSource(ResourceRoot::RootType type, VFS* vfs) : type(type), vfs(vfs) {}
        };

        struct AssetInfo {
            IAsset* assetPtr{nullptr};
            VFS* vfs{ nullptr };
            FileEntry* vfsEntry{ nullptr };

            AssetInfo() : assetPtr{ nullptr }, vfs{nullptr}, vfsEntry{ nullptr }{};
            AssetInfo(IAsset* assetPtr, VFS* vfs, FileEntry* vfsEntry) : assetPtr{ assetPtr }, vfs{vfs}, vfsEntry{ vfsEntry }{};

            constexpr bool operator==(const AssetInfo& other) const { 
                return assetPtr == other.assetPtr && other.vfsEntry == vfsEntry; 
            }

            constexpr bool operator!=(const AssetInfo& other) const {
                return assetPtr != other.assetPtr || other.vfsEntry != vfsEntry;
            }
        };

        std::vector<VFSSource> _allVfs;
        ReferenceVector<AssetInfo> _assets;
    };
}