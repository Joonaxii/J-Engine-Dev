#include <JEngine/Assets/AssetMetaData.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Global/UUIDFactory.h>
#include <JEngine/IO/Serialization/Serialization.h>

namespace JEngine {
    AssetMetaData::AssetMetaData() : _uuid(UUIDFactory::UUID_EMPTY), _assetFlags(0) { }
    AssetMetaData::AssetMetaData(const UUID& uuid) : _uuid(uuid), _assetFlags(0) { }
    AssetMetaData::AssetMetaData(const std::string& name, const UUID& uuid) : _uuid(uuid), _assetFlags(0) { }

    const bool AssetMetaData::serializeJson(json& jsonF) const {
        jsonF["uuid"] = UUIDFactory::getUUIDStr(_uuid);
        _assetFlags.serializeJson(jsonF["flags"]);
        return true;
    }

    const bool AssetMetaData::deserializeJson(json& jsonF) {
        UUIDFactory::parseUUID(jsonF.value("uuid", ""), _uuid);
        _assetFlags.deserializeJson(jsonF["flags"]);
        return true;
    }

    const bool AssetMetaData::serializeBinary(std::ostream& stream) const {
        Serialization::serializeBinary(stream, _uuid);
        _assetFlags.serializeBinary(stream);
        return true;
    }

    const bool AssetMetaData::deserializeBinary(std::istream& stream) {
        Serialization::deserializeBinary(stream, _uuid);
        _assetFlags.deserializeBinary(stream);
        return true;
    }

    const bool AssetMetaData::jsonToBinary(json& jsonF, std::ostream& stream) {
        AssetMetaData temp;
        temp.deserializeJson(jsonF);
        return temp.serializeBinary(stream);
    }

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