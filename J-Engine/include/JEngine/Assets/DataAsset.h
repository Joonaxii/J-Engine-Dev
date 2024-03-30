#pragma once
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/IAssetSerializer.h>

namespace JEngine {
    struct DataAssetSerializer;
    class DataAsset : public IAsset {
    public:
        enum DataType : uint8_t {
            T_Binary,
            T_Text,
            T_DefinedType,
        };

        virtual ~DataAsset() override;
        virtual bool unload() override;

        DataType getDataType() const { return _type; }
        size_t getDataLength() const { return _length; }

        uint8_t* getData() { return _data; }
        const uint8_t* getData() const { return _data; }

        template<typename T> T* getAs() { return reinterpret_cast<T*>(_data); }
        template<typename T> const T* getAs() const { return reinterpret_cast<const T*>(_data); }

        std::string_view asStringView() { return std::string_view(reinterpret_cast<char*>(_data), _length); }

        template<typename T> Span<T> asSpan() { return Span<T>(_data, _length); }
        template<typename T> ConstSpan<T> asConstSpan() const { return ConstSpan<T>(_data, _length); }

        void resize(size_t newSize);
        void setData(const void* data, size_t size, DataType type = DataType::T_Binary);

    protected:
        void destroyAsset() override {
            IAssetSerializer<DataAsset>::getAllocator().deallocate(this);
        }
    private:
        friend struct DataAssetSerializer;
        DataType _type;
        size_t _length{0};
        uint8_t* _data{nullptr};
    };
}
REGISTER_ASSET(JEngine::DataAsset);


#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::DataAsset::DataType& itemRef) {
        yamlOut << YAML::Dec << static_cast<const uint16_t>(itemRef);
        return yamlOut;
    }

    template<>
    struct convert<JEngine::DataAsset::DataType> {
        static Node encode(const JEngine::DataAsset::DataType& rhs) {
            Node node;
            node.push_back(static_cast<const uint16_t>(rhs));
            return node;
        }

        static bool decode(const Node& node, JEngine::DataAsset::DataType& rhs) {
            rhs = JEngine::DataAsset::DataType(node.as<uint16_t>());
            return true;
        }
    };
}
//JSON
namespace JEngine {
    template<>
    inline bool Serializable<DataAsset::DataType>::deserializeJson(DataAsset::DataType& itemRef, const json& jsonF, const DataAsset::DataType& defaultVal) {
        itemRef = jsonF.is_number() ? DataAsset::DataType(jsonF.get<uint8_t>()) : defaultVal;
        return true;
    }

    template<>
    inline bool Serializable<DataAsset::DataType>::serializeJson(const DataAsset::DataType& itemRef, json& jsonF) {
        jsonF = uint8_t(itemRef);
        return true;
    }
}
#pragma endregion