#include <JEngine/Assets/IAsset.h>
#include <JEngine/IO/Serialization/Serialization.h>
#include <JEngine/Assets/AssetMetaData.h>
#include <JEngine/IO/VFS/FileEntry.h>

namespace JEngine {
   AssetMetaData& IAsset::getMetadata() {
        return _meta;
    }

   const bool IAsset::serializeMetaJson(json& jsonF) const {
       jsonF["name"] = _name;
       _flags.serializeJson(jsonF["flags"]);
       _meta.serializeJson(jsonF);
       return true;
   }

   const bool IAsset::deserializeMetaJson(json& jsonF) {

       return true;
   }

   const bool IAsset::serializeMetaBinary(std::ostream& stream) const {

       return true;
   }

   const bool IAsset::deserializeMetaBinary(std::istream& stream, const size_t size) {

       return true;
   }

   const bool IAsset::jsonToBinary(json& jsonF, std::ostream& stream) {

       return false;
   }

    FileEntry* IAsset::getFileEntry() {
        return _fEntry;
    }

    const FileEntry* IAsset::getConstFileEntry() const {
        return _fEntry;
    }

    void IAsset::setFileEntry(FileEntry* fEntry) {
        _fEntry = fEntry;
    }

    void IAsset::setDatabaseFlag(const bool flag) {
        _flags.setBit(ASSET_IS_IN_DB, flag);
    }

    const bool IAsset::isInDatabase() const {
        return _flags.isBitSet(ASSET_IS_IN_DB);
    }

    const void IAsset::setName(const std::string& name) {
        _name = name;
    }

    const std::string& IAsset::getName() const {
        return _name;
    }

    const UUID& IAsset::getUUID() const {
        return _meta.getUUID();
    }
}

