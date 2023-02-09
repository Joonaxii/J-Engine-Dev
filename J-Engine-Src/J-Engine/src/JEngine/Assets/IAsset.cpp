#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/AssetMetaData.h>
#include <JEngine/IO/VFS/FileEntry.h>

namespace JEngine {
 
    void IAsset::setFileEntry(FileEntry* fEntry) { _fEntry = fEntry; }

    FileEntry* IAsset::getFileEntry() { return _fEntry; }
    const FileEntry* IAsset::getFileEntry() const { return _fEntry; }

    void IAsset::setDatabaseFlag(const bool flag) {
        _flags.setBit(ASSET_IS_IN_DB, flag);
    }

    bool IAsset::isInDatabase() const {
        return _flags.isBitSet(ASSET_IS_IN_DB);
    }

    void IAsset::setName(const std::string& name) { _name = name; }
    const std::string& IAsset::getName() const { return _name; }

    const AssetMetaData& IAsset::getMetadata() const { return _meta; }
    const UUID& IAsset::getUUID() const { return _meta.getUUID(); }

   bool IAsset::serializeMetaJson(json& jsonF) const {
       Serialization::serialize(_name, jsonF["name"]);
       Serialization::serialize(_flags, jsonF["assetFlags"]);
       Serialization::serialize(_meta, jsonF["meta"]);
       return true;
   }

   bool IAsset::deserializeMetaJson(json& jsonF) {

       return true;
   }

   bool IAsset::serializeMetaBinary(std::ostream& stream) const {

       return true;
   }

   bool IAsset::deserializeMetaBinary(std::istream& stream, const size_t size) {

       return true;
   }
}

