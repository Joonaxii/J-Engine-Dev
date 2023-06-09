#include <JEngine/Assets/AssetMetaData.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Cryptography/UUIDFactory.h>

namespace JEngine {
    AssetMetaData::AssetMetaData() : _uuid(UUID8::Empty), _assetFlags(0) { }
    AssetMetaData::AssetMetaData(const UUID8& uuid) : _uuid(uuid), _assetFlags(0) { }
    AssetMetaData::AssetMetaData(const std::string& name, const UUID8& uuid) : _uuid(uuid), _assetFlags(0) { }

    const void AssetMetaData::setUUID(const UUID8& uuid) {
        _uuid = uuid;
    }

    const UUID8& AssetMetaData::getUUID() const {
        return _uuid;
    }

    const void AssetMetaData::setFlags(const UI8Flags& flags) {
        _assetFlags = flags;
    }

    const void AssetMetaData::setFlags(const uint8_t& flags) {
        _assetFlags = UI8Flags(flags);
    }

    const UI8Flags& AssetMetaData::getFlags() const {
        return _assetFlags;
    }

    void AssetMetaData::releaseUUID() {
        UUIDFactory::removeUUID<IAsset>(_uuid);
    }
}