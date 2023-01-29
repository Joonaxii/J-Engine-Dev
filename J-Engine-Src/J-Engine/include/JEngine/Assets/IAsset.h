#pragma once
#include <JEngine/Global/UUIDFactory.h>
#include <JEngine/Assets/AssetType.h>
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Assets/AssetMetaData.h>
#include <cstdint>
#include <JEngine/Collections/IRef.h>

namespace JEngine {

    template<typename T>
    struct AssetID {
        static const AssetType TYPE;
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
        const FileEntry* getConstFileEntry() const;
        void setFileEntry(FileEntry* fEntry);

        void setDatabaseFlag(const bool flag);
        const bool isInDatabase() const;

        const void setName(const std::string& name);
        const std::string& getName() const;

        const UUID& getUUID() const;
        AssetMetaData& getMetadata();

        virtual const bool serializeMetaJson(json& jsonF) const;
        virtual const bool deserializeMetaJson(json& jsonF);

        virtual const bool serializeMetaBinary(std::ostream& stream) const;
        virtual const bool deserializeMetaBinary(std::istream& stream, const size_t size);

        virtual const bool serializeJson(json& jsonF) const = 0;
        virtual const bool deserializeJson(json& jsonF) = 0;

        virtual const bool serializeBinary(std::ostream& stream) const = 0;
        virtual const bool deserializeBinary(std::istream& stream, const size_t size) = 0;

        virtual const bool jsonToBinary(json& jsonF, std::ostream& stream);
    
    protected:
        UI8Flags _flags;
        std::string _name;
        FileEntry* _fEntry;
      
        AssetMetaData _meta;
    };
}
REGISTER_UUID_FACTORY(JEngine::IAsset)


#define REGISTER_ASSET(T, ATYPE) \
template<> inline int32_t JEngine::AssetID<T>::CUR_ID = 0; \
template<> inline const JEngine::AssetType JEngine::AssetID<T>::TYPE = JEngine::AssetType::ATYPE; \


//#define REGISTER_ASSET_SER(T, ATYPE) REGISTER_ASSET(T, ATYPE, JEngine::AssetFileType::Binary | JEngine::AssetFileType::JSON)
//#define REGISTER_ASSET_ALL(T, ATYPE) REGISTER_ASSET(T, ATYPE, JEngine::AssetFileType::All)
//#define REGISTER_ASSET_SRC(T, ATYPE) REGISTER_ASSET(T, ATYPE, JEngine::AssetFileType::Source)
//#define REGISTER_ASSET_BIN(T, ATYPE) REGISTER_ASSET(T, ATYPE, JEngine::AssetFileType::Binary | JEngine::AssetFileType::Source)