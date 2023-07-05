#pragma once
#include <JEngine/Cryptography/UUIDFactory.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/IO/VFS/VFS.h>

namespace JEngine {
    struct ResourceRoot {
        enum RootType {
            RT_GAME = 0x01,
            RT_EDITOR = 0x02,
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

        const VFS* getVFS(ResourceRoot::RootType type) const { 
            auto find = _vfs.find(type);
            return find == _vfs.end() ? nullptr : _allVfs[find->second];
        }
        void refresh(uint8_t refreshMode = REFRESH_SOFT);

        void buildVFS(uint32_t types);

    private:
        struct AssetInfo {
            IAsset* assetPtr{nullptr};
            UUID8 vfsEntry{ UUID8::Empty };

            AssetInfo() : assetPtr{ nullptr }, vfsEntry{ -1 }{};
            AssetInfo(IAsset* assetPtr, int32_t entryIndex) : assetPtr{ assetPtr }, vfsEntry{ entryIndex }{};

            constexpr bool operator==(const AssetInfo& other) const { 
                return assetPtr == other.assetPtr && other.vfsEntry == vfsEntry; 
            }

            constexpr bool operator!=(const AssetInfo& other) const {
                return assetPtr != other.assetPtr || other.vfsEntry != vfsEntry;
            }
        };

        std::vector<VFS*> _allVfs;
        std::unordered_map<uint32_t, int32_t> _vfs;
        ReferenceVector<AssetInfo> _assets;
    };
}