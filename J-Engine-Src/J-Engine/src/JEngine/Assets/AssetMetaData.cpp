#include <JEngine/Assets/AssetMetaData.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Global/UUIDFactory.h>
#include <JEngine/IO/Serialization/Serialization.h>

namespace JEngine {
    AssetMetaData::AssetMetaData() : _uuid(UUIDFactory::Empty), _assetFlags(0) { }
    AssetMetaData::AssetMetaData(const UUID& uuid) : _uuid(uuid), _assetFlags(0) { }
    AssetMetaData::AssetMetaData(const std::string& name, const UUID& uuid) : _uuid(uuid), _assetFlags(0) { }

    const void AssetMetaData::setUUID(const UUID& uuid) {
        _uuid = uuid;
    }

    const UUID& AssetMetaData::getUUID() const {
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