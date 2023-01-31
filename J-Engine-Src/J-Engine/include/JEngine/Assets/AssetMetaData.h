#pragma once
#include <JEngine/Utility/Flags.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Global/UUIDFactory.h>
#include <JEngine/Assets/AssetDataType.h>

namespace JEngine {
    class AssetMetaData {
    public:
        static constexpr uint8_t NO_SERIALIZE_FLAG = 0x1;

        AssetMetaData();
        AssetMetaData(const UUID& uuid);
        AssetMetaData(const std::string& name, const UUID& uuid);

        const void setUUID(const UUID& uuid);
        const UUID& getUUID() const;

        const void setFlags(const UI8Flags& flags);
        const void setFlags(const uint8_t& flags);
        const UI8Flags& getFlags() const;

        void releaseUUID();

    private:
        friend struct Serializable<AssetMetaData>;

        UI8Flags _assetFlags;
        UUID _uuid;
        AssetDataFormat _assetDataFormat;
        AssetDataType _assetDataType;
    };

#pragma region Serialization
    template<>
    inline const bool Serializable<AssetMetaData>::deserializeJson(AssetMetaData& itemRef, json& jsonF, const AssetMetaData& defaultVal) {
        Serialization::deserialize(itemRef._assetFlags,      jsonF["flags"], defaultVal._assetFlags);
        Serialization::deserialize(itemRef._uuid,            jsonF["uuid"], defaultVal._uuid);
        Serialization::deserialize(itemRef._assetDataFormat, jsonF["dataFormat"], defaultVal._assetDataFormat);
        Serialization::deserialize(itemRef._assetDataType,   jsonF["dataType"], defaultVal._assetDataType);
        return true;
    }

    template<>
    inline const bool Serializable<AssetMetaData>::serializeJson(const AssetMetaData& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._assetFlags, jsonF["flags"]);
        Serialization::serialize(itemRef._uuid, jsonF["uuid"]);
        Serialization::serialize(itemRef._assetDataFormat, jsonF["dataFormat"]);
        Serialization::serialize(itemRef._assetDataType, jsonF["dataType"]);
        return true;
    }

    template<>
    inline const bool Serializable<AssetMetaData>::deserializeBinary(AssetMetaData& itemRef, std::istream& stream) {
        Serialization::deserialize(itemRef._assetFlags, stream);
        Serialization::deserialize(itemRef._uuid, stream);
        Serialization::deserialize(itemRef._assetDataFormat, stream);
        Serialization::deserialize(itemRef._assetDataType, stream);
        return true;
    }

    template<>
    inline const bool Serializable<AssetMetaData>::serializeBinary(const AssetMetaData& itemRef, std::ostream& stream) {
        Serialization::serialize(itemRef._assetFlags, stream);
        Serialization::serialize(itemRef._uuid, stream);
        Serialization::serialize(itemRef._assetDataFormat, stream);
        Serialization::serialize(itemRef._assetDataType, stream);
        return true;
    }
#pragma endregion
}
