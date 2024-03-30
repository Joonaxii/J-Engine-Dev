#pragma once
#include <JEngine/Assets/IAssetSerializer.h>
#include <JEngine/Assets/Graphics/Shader/Shader.h>

namespace JEngine {
    class ShaderSerializer : public IAssetSerializer<Shader> {
    public:
        void deserializeImpl(FileEntry* entry, IAsset* asset, const Stream& stream, uint8_t flags) override;
        void deserializeImpl(FileEntry* entry, IAsset* asset, const json& jsonF, uint8_t flags) override {}
        void deserializeImpl(FileEntry* entry, IAsset* asset, const yamlNode& node, uint8_t flags) override{}

        void serializeImpl(FileEntry* entry, const IAsset* asset, const Stream& stream, uint8_t flags) override;
        void serializeImpl(FileEntry* entry, const IAsset* asset, json& jsonF, uint8_t flags) override{}
        void serializeImpl(FileEntry* entry, const IAsset* asset, yamlEmit& emit, uint8_t flags) override{}
    };

    template<>
    inline static void IAssetSerializer<Shader>::getSerializerTypes(SerializerType types[3]) {
        types[SER_TYPE_Binary] = SER_TYPE_Binary;
        types[SER_TYPE_JSON] = SER_TYPE_Binary;
        types[SER_TYPE_YAML] = SER_TYPE_Binary;
    }

    template<>
    inline static IAssetSerializer<Shader>* IAssetSerializer<Shader>::getSerializer() {
        static ShaderSerializer serializer{};
        return &serializer;
    }

    template<>
    inline static IAsset* IAssetSerializer<Shader>::createNewAsset(const char* name, UUID8 uuid, uint8_t flags) {
        Shader* asset = getAllocator().allocate();
        if (asset) {
            asset->initObj(name, uuid, flags);
        }
        return asset;
    }

    template<>
    inline static IAsset* IAssetSerializer<Shader>::destroyAsset(IAsset* asset) {
        if (asset && getAllocator().deallocate(reinterpret_cast<Shader*>(asset))) {
            return nullptr;
        }
        return asset;
    }
}