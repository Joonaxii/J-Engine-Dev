#pragma once
#include <JEngine/Utility/Flags.h>
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/Global/UUIDFactory.h>
#include <JEngine/Assets/AssetDataType.h>

namespace JEngine {
    class AssetMetaData : public ISerializable<AssetMetaData> {
    public:
        static constexpr uint8_t NO_SERIALIZE_FLAG = 0x1;

        AssetMetaData();
        AssetMetaData(const UUID& uuid);
        AssetMetaData(const std::string& name, const UUID& uuid);

        const bool serializeJson(json& jsonF) const;
        const bool deserializeJson(json& jsonF);

        const bool serializeBinary(std::ostream& stream) const;
        const bool deserializeBinary(std::istream& stream);

        static const bool jsonToBinary(json& jsonF, std::ostream& stream);

        const void setUUID(const UUID& uuid);
        const UUID& getUUID() const;

        const void setFlags(const UI8Flags& flags);
        const void setFlags(const uint8_t& flags);
        const UI8Flags& getFlags() const;

        void releaseUUID();

    private:
        UI8Flags _assetFlags;
        UUID _uuid;
        AssetDataFormat _assetDataFormat;
        AssetDataType _assetDataType;
    };
}
