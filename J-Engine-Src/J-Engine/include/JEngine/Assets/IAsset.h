#pragma once
#include <JEngine/Global/UUIDFactory.h>
#include <JEngine/Assets/AssetType.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Assets/AssetMetaData.h>
#include <cstdint>
#include <JEngine/Collections/IRef.h>

namespace JEngine {

    template<typename T>
    struct AssetID {
        static int32_t CUR_ID;
    };

    class FileEntry;
    class IAsset : public ISerializable<IAsset>, public IRef {
    public:
        static constexpr uint8_t ASSET_IS_IN_DB = 0x1;

        static constexpr uint32_t A_REF_NAME = 0x0;
        static constexpr uint32_t A_REF_PATH = 0x1;
        static constexpr uint32_t A_REF_UUID = 0x2;

        IAsset() : IRef(-1), _fEntry(nullptr), _flags() {}
        IAsset(const int32_t& id) : IRef(id), _name(""),  _fEntry(nullptr), _flags() {}
        IAsset(const std::string& name, const int32_t& id) : IRef(id), _name(name), _fEntry(nullptr), _flags() {}
        IAsset(const std::string& name) : IRef(-1), _name(name), _fEntry(nullptr), _flags() {}

        FileEntry* getFileEntry();
        const FileEntry* getFileEntry() const;
        void setFileEntry(FileEntry* fEntry);

        void setDatabaseFlag(const bool flag);
        bool isInDatabase() const;

        void setName(const std::string& name);
        const std::string& getName() const;

        const UUID& getUUID() const;
        const AssetMetaData& getMetadata() const;

        virtual bool serializeMetaJson(json& jsonF) const;
        virtual bool deserializeMetaJson(json& jsonF);

        virtual bool serializeMetaBinary(std::ostream& stream) const;
        virtual bool deserializeMetaBinary(std::istream& stream, const size_t size);

        virtual bool serializeJson(json& jsonF) const override = 0;
        virtual bool deserializeJson(json& jsonF) override = 0;

        virtual bool serializeBinary(std::ostream& stream) const override = 0;
        virtual bool deserializeBinary(std::istream& stream, const size_t size) = 0;

    protected:
        UI8Flags _flags;
        std::string _name;
        FileEntry* _fEntry;
      
        AssetMetaData _meta;

        virtual bool jsonToBinaryImpl(json& jsonF, std::ostream& stream) const override = 0;
    };
}
REGISTER_UUID_FACTORY(JEngine::IAsset)


#define REGISTER_ASSET(T) \
template<> inline int32_t JEngine::AssetID<T>::CUR_ID = 0; \
