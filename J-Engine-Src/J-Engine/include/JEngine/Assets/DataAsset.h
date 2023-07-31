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

    private:
        friend struct DataAssetSerializer;
        DataType _type;
        size_t _length{0};
        uint8_t* _data{nullptr};
    };
}
REGISTER_ASSET(JEngine::DataAsset);