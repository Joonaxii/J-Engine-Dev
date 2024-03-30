#pragma once
#include <JEngine/Assets/IAssetSerializer.h>
#include <JEngine/Audio/AudioClip.h>

namespace JEngine {
    struct AudioClipSerializer : public IAssetSerializer<AudioClip> {
    public:

        static uint8_t parseLabelFile(const Stream& stream, uint32_t sampleRate, AudioSection sections[AudioClip::MAX_SECTION_COUNT], size_t size = SIZE_MAX);

        void deserializeImpl(FileEntry* entry, IAsset* asset, const Stream& stream, uint8_t flags) override;
        void deserializeImpl(FileEntry* entry, IAsset* asset, const json& jsonF, uint8_t flags) override;
        void deserializeImpl(FileEntry* entry, IAsset* asset, const yamlNode& node, uint8_t flags) override;

        void serializeImpl(FileEntry* entry, const IAsset* asset, const Stream& stream, uint8_t flags) override;
        void serializeImpl(FileEntry* entry, const IAsset* asset, json& jsonF, uint8_t flags) override;
        void serializeImpl(FileEntry* entry, const IAsset* asset, yamlEmit& emit, uint8_t flags) override;
    };

    template<>
    inline static IAssetSerializer<AudioClip>* IAssetSerializer<AudioClip>::getSerializer() {
        static AudioClipSerializer serializer{};
        return &serializer;
    }

    template<>
    inline static IAsset* IAssetSerializer<AudioClip>::createNewAsset(const char* name, UUID8 uuid, uint8_t flags) {
        AudioClip* asset = getAllocator().allocate();
        if (asset) {
            asset->initObj(name, uuid, flags);
        }
        return asset;
    }

    template<>
    inline static IAsset* IAssetSerializer<AudioClip>::destroyAsset(IAsset* asset) {
        if (asset && getAllocator().deallocate(reinterpret_cast<AudioClip*>(asset))) {
            return nullptr;
        }
        return asset;
    }
}