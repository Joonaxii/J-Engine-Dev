#pragma once
#include <JEngine/Cryptography/UUIDFactory.h>
#include <JEngine/Collections/IndexStack.h>
#include <JEngine/Core/Ref.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/IAssetSerializer.h>
#include <JEngine/IO/VFS/VFS.h>

#ifdef JENGINE_EDITOR
#include <Editor/IO/DirectoryMonitor.h>
#endif


namespace JEngine {
    class AssetDB {
    public:
        enum AssetSourceType : uint8_t {
            SRC_BUILT_IN = 0x00,
            SRC_EDITOR = 0x01,
            SRC_GAME = 0x02,
            SRC_RUNTIME = 0x03,
            SRC_OVERRIDE = 0x04,
            SRC_COUNT
        };

        AssetDB();

        ~AssetDB();
#ifdef JENGINE_EDITOR
        void initialize(const ConstSpan<char> roots[SRC_COUNT], const ConstSpan<char> dbRoots[SRC_COUNT]);

        //void deserializeEditorAsset(FileEntry* entry);
        //void serializeEditorAsset(AssetRef asset);

        uint32_t packAssets(uint32_t sources, ConstSpan<char> destination);
        IAsset* getAssetByUUID(AssetRef uuid, bool fromDB);
#else
        void initialize(const ConstSpan<char> roots[SRC_COUNT]);
        IAsset* getAssetByUUID(AssetRef uuid);
#endif
        VFS* getVFS(uint8_t index);
        const VFS* getVFS(uint8_t index) const;

        void refresh(uint8_t source, uint8_t refreshMode);
        void buildVFS(uint32_t types);

        const FileEntry* findFromVFS(ConstSpan<char> path, uint8_t source) const;

        AssetRef createAsset(const UUID16& type, ConstSpan<char> path, uint8_t flags, uint8_t source = SRC_RUNTIME);

        template<typename T>
        TAssetRef<T> createAsset(ConstSpan<char> path, uint8_t flags, uint8_t source) {
            detail::Asset* assetDet = detail::Asset::getAsset<T>();
            return assetDet ? createAsset(assetDet->type->hash, path, flags, vfsType) : nullptr;
        }

        template<typename T>
        bool deleteAsset(TAssetRef<T> asset) {
            if (!asset) { return false; }

            return true;
        }



    private:
        struct AssetInfo {
            enum : uint16_t {
                FLG_NONE = 0x00,
                FLG_CHANGED = 0x01,
                FLG_NO_META = 0x02,
                FLG_NEED_IMPORT = 0x04,
            };

            uint16_t flags{};
            IAsset* assetPtr{ nullptr };
            FileEntry* vfsEntry{ nullptr };

            AssetInfo() : assetPtr{ nullptr }, vfsEntry{ nullptr }{};
            AssetInfo(IAsset* assetPtr, FileEntry* vfsEntry) : assetPtr{ assetPtr }, vfsEntry{ vfsEntry }{};

            constexpr bool operator==(const AssetInfo& other) const {
                return assetPtr == other.assetPtr && other.vfsEntry == vfsEntry;
            }

            constexpr bool operator!=(const AssetInfo& other) const {
                return assetPtr != other.assetPtr || other.vfsEntry != vfsEntry;
            }
        };

        struct VFSSource {
        public:
            VFS vfs{};
            ChunkedLUT<AssetInfo> assets{};

#ifdef JENGINE_EDITOR
            VFS vfsDB{};
            ChunkedLUT<AssetInfo> assetsDB{};
            DirectoryMonitor dirMonitor{};

            void setup(ConstSpan<char> source, ConstSpan<char> db) {
                vfs.changeRoot(source);
                vfsDB.changeRoot(db);
            }
            void selectPackableAssets(std::vector<uint32_t>& indices);

            void update();
#else
            void setup(ConstSpan<char> source) {
                vfs.changeRoot(source);
            }
#endif
            VFSSource() {}
            void unload(bool fully);

        private:
        };
        VFSSource _allSources[AssetSourceType::SRC_COUNT];
    };
}