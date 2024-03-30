#include <JEngine/Assets/Serializers/DataAssetSerializer.h>

namespace JEngine {
    void DataAssetSerializer::deserializeImpl(FileEntry* entry, IAsset* asset, const Stream& stream, uint8_t flags) {
        if (!asset) { return; }
        auto dataAsset = asAsset(asset);

        if (flags & SER_MetaData) {
            stream.readValue(dataAsset->_type, false);
            stream.readValue(dataAsset->_length, false);
        }
        else { stream.seek(5, SEEK_CUR); }

        if (flags & SER_FullData) {
            dataAsset->resize(dataAsset->_length);
            stream.read(dataAsset->_data, dataAsset->_length, false);
        }
    }
    void DataAssetSerializer::deserializeImpl(FileEntry* entry, IAsset* asset, const json& jsonF, uint8_t flags) {
        if (!asset) { return; }
        auto dataAsset = asAsset(asset);

        if (flags & SER_MetaData) {
            Serialization::deserialize(dataAsset->_type, jsonF["DataType"], DataAsset::DataType::T_Binary);
            dataAsset->_length = jsonF["Data"].size();
        }

        if (flags & SER_FullData) {
            auto& data = jsonF["Data"].get_binary();
            dataAsset->resize(data.size());
            memcpy(dataAsset->_data, data.data(), data.size());
        }
    }
    void DataAssetSerializer::deserializeImpl(FileEntry* entry, IAsset* asset, const yamlNode& node, uint8_t flags) {
        if (!asset) { return; }
        auto dataAsset = asAsset(asset);

        if (flags & SER_MetaData) {
            Serialization::deserialize(dataAsset->_type, node["DataType"], DataAsset::DataType::T_Binary);
            dataAsset->_length = node["Data"].as<YAML::Binary>().size();
        }

        if (flags & SER_FullData) {
            auto& data = node["Data"].as<YAML::Binary>();
            dataAsset->resize(data.size());
            memcpy(dataAsset->_data, data.data(), data.size());
        }
    }

    void DataAssetSerializer::serializeImpl(FileEntry* entry, const IAsset* asset, const Stream& stream, uint8_t flags) {
        if (!asset) { return; }
        auto dataAsset = asAsset(asset);

        if (flags & SER_MetaData) {
            stream.writeValue(dataAsset->_type);
            stream.writeValue(dataAsset->_length);
        }

        if (flags & SER_FullData) {
            if (dataAsset->_data) {
                stream.write(dataAsset->_data, dataAsset->_length, false);
            }
        }
    }
    void DataAssetSerializer::serializeImpl(FileEntry* entry, const IAsset* asset, json& jsonF, uint8_t flags) {
        if (!asset) { return; }
        auto dataAsset = asAsset(asset);

        if (flags & SER_MetaData) {
            Serialization::serialize(dataAsset->_type, jsonF["DataType"]);
        }

        if (flags & SER_FullData) {
            if (dataAsset->_data) {
                jsonF["Data"] = json::array_t(dataAsset->_data, dataAsset->_data + dataAsset->_length);
            }
            else {
                jsonF["Data"] = json::array_t();
            }
        }
    }
    void DataAssetSerializer::serializeImpl(FileEntry* entry, const IAsset* asset, yamlEmit& emit, uint8_t flags) {
        if (!asset) { return; }
        auto dataAsset = asAsset(asset);

        if (flags & SER_MetaData) {
            Serialization::serialize("DataType", dataAsset->_type, emit);
        }

        if (flags & SER_FullData) {
            emit << YAML::Key << "Data" << YAML::Value;
            if (dataAsset->_data) {
                emit << YAML::Binary(dataAsset->_data, dataAsset->_length);
            }
            else {
                emit << YAML::Null;
            }
        }
    }

}