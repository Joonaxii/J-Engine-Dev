#pragma once
#include <JEngine/Utility/Flags.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Cryptography/UUIDFactory.h>
#include <JEngine/Assets/AssetDataType.h>
#include <JEngine/Utility/DataFormatUtils.h>

namespace JEngine {
    class AssetMetaData {
    public:
        static constexpr uint8_t NO_SERIALIZE_FLAG = 0x1;

        AssetMetaData();
        AssetMetaData(const UUID8& uuid);
        AssetMetaData(const std::string& name, const UUID8& uuid);

        const std::string& setName(const char* newName) { return _name = newName; }
        const std::string& setName(std::string_view& newName) { return _name = newName; }
        const std::string& getName() const { return _name; }
        const char* getNameCStr() const { return _name.c_str(); }
        const void setUUID(const UUID8& uuid);
        const UUID8& getUUID() const;

        const void setFlags(const UI8Flags& flags);
        const void setFlags(const uint8_t& flags);
        const UI8Flags& getFlags() const;

        DataFormat getDataFormat() const { return _assetDataFormat; }

        void releaseUUID();

    private:
        friend struct Serializable<AssetMetaData>;
        friend struct YAML::convert<AssetMetaData>;

        std::string _name;
        UUID8 _uuid;
        UI8Flags _assetFlags;
        DataFormat _assetDataFormat;
    };

}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::AssetMetaData& itemRef) {
        yamlOut << YAML::BeginMap;

        yamlOut << YAML::Key << "uuid"   << YAML::Value << itemRef.getUUID();
        yamlOut << YAML::Key << "flags"  << YAML::Value << itemRef.getFlags();
        yamlOut << YAML::Key << "format" << YAML::Value << itemRef.getDataFormat();

        yamlOut << YAML::EndMap;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::AssetMetaData> {
        static Node encode(const JEngine::AssetMetaData& rhs) {
            Node node;
            //node["uuid"] = rhs._uuid;
           // node["flags"] = rhs._assetFlags;
            //node["formats"] = rhs._assetDataFormat;
            return node;
        }

        static bool decode(const Node& node, JEngine::AssetMetaData& rhs) {
            if (!node.IsMap()) { return false; }
            //rhs._uuid = node["uuid"].as<JEngine::UUID8>();
           // rhs._assetFlags = node["flags"].as<UI8Flags>();
            //rhs._assetDataFormat = node["format"].as<JEngine::AssetDataFormat>();
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<AssetMetaData>::deserializeJson(AssetMetaData& itemRef, json& jsonF, const AssetMetaData& defaultVal) {
        Serialization::deserialize(itemRef._uuid, jsonF["uuid"], defaultVal._uuid);
        Serialization::deserialize(itemRef._assetFlags, jsonF["flags"], defaultVal._assetFlags);
        Serialization::deserialize(itemRef._assetDataFormat, jsonF["dataFormat"], defaultVal._assetDataFormat);
        return true;
    }

    template<>
    inline bool Serializable<AssetMetaData>::serializeJson(const AssetMetaData& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._uuid, jsonF["uuid"]);
        Serialization::serialize(itemRef._assetFlags, jsonF["flags"]);
        Serialization::serialize(itemRef._assetDataFormat, jsonF["dataFormat"]);
        return true;
    }

    template<>
    inline bool Serializable<AssetMetaData>::deserializeBinary(AssetMetaData& itemRef, const Stream& stream) {
        Serialization::deserialize(itemRef._uuid, stream);
        Serialization::deserialize(itemRef._assetFlags, stream);
        Serialization::deserialize(itemRef._assetDataFormat, stream);
        return true;
    }

    template<>
    inline bool Serializable<AssetMetaData>::serializeBinary(const AssetMetaData& itemRef, const Stream& stream) {
        Serialization::serialize(itemRef._uuid, stream);
        Serialization::serialize(itemRef._assetFlags, stream);
        Serialization::serialize(itemRef._assetDataFormat, stream);
        return true;
    }
}
#pragma endregion