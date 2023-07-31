#pragma once
#include <JEngine/Assets/IAssetSerializer.h>
#include <JEngine/Assets/DataAsset.h>

namespace JEngine {
    struct DataAssetSerializer : public IAssetSerializer<DataAsset> {
    public:
        void deserializeImpl(FileEntry* entry, IAsset* asset, const Stream& stream, uint8_t flags) override;
        void deserializeImpl(FileEntry* entry, IAsset* asset, const json& jsonF, uint8_t flags) override;
        void deserializeImpl(FileEntry* entry, IAsset* asset, const yamlNode& node, uint8_t flags) override;

        void serializeImpl(FileEntry* entry, const IAsset* asset, const Stream& stream, uint8_t flags) override;
        void serializeImpl(FileEntry* entry, const IAsset* asset, json& jsonF, uint8_t flags) override;
        void serializeImpl(FileEntry* entry, const IAsset* asset, yamlEmit& emit, uint8_t flags) override;
    };

    template<>
    inline static IAssetSerializer<DataAsset>* IAssetSerializer<DataAsset>::getSerializer() {
        static DataAssetSerializer serializer{};
        return &serializer;
    }

    template<>
    inline static IAsset* IAssetSerializer<DataAsset>::createNewAsset(const char* name, UUID8 uuid, uint8_t flags) {
        DataAsset* asset = getAllocator().allocate();
        if (asset) {
            asset->initObj(name, uuid, flags);
        }
        return asset;
    }

    template<>
    inline static IAsset* IAssetSerializer<DataAsset>::destroyAsset(IAsset* asset) {
        if (asset && getAllocator().deallocate(reinterpret_cast<DataAsset*>(asset))) {
            return nullptr;
        }
        return asset;
    }
}