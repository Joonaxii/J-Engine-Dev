#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Helpers/TypeHelpers.h>
#include <JEngine/Collections/PoolAllocator.h>
#include <JEngine/Utility/DataFormatUtils.h>
#include <JEngine/IO/VFS/VFS.h>
#include <JEngine/Core/Ref.h>

namespace JEngine {

    enum SerializerType {
        SER_TYPE_Binary,
        SER_TYPE_JSON,
        SER_TYPE_YAML,
    };

    template<typename T>
    struct AssetSerializerInfo {
        enum SerializerFlags {
            SER_F_None = 0x0,
            SER_F_NoSerializer = 0x1,
        };
        static constexpr SerializerFlags Flags{ SER_F_None };
    };

    template<typename T>
    struct IAssetSerializer {
    public:
        enum AssetSerializeFlags {
            SER_None = 0x0,
            SER_MetaData = 0x1,
            SER_FullData = 0x2,

            SER_All = SER_MetaData | SER_FullData
        };

        virtual void deserializeImpl(FileEntry* entry, IAsset* asset, const Stream& stream, uint8_t flags) = 0;
        virtual void deserializeImpl(FileEntry* entry, IAsset* asset, const json& jsonF, uint8_t flags) = 0;
        virtual void deserializeImpl(FileEntry* entry, IAsset* asset, const yamlNode& node, uint8_t flags) = 0;

        virtual void serializeImpl(FileEntry* entry, const IAsset* asset, const Stream& stream, uint8_t flags) = 0;
        virtual void serializeImpl(FileEntry* entry, const IAsset* asset, json& jsonF, uint8_t flags) = 0;
        virtual void serializeImpl(FileEntry* entry, const IAsset* asset, yamlEmit& emit, uint8_t flags) = 0;

        static PoolAllocator<T, 0>& getAllocator() {
            static PoolAllocator<T, 0> allocator{};
            return allocator;
        }

        static void getSerializerTypes(SerializerType types[3]) {
            types[SER_TYPE_Binary] = SER_TYPE_Binary;
            types[SER_TYPE_JSON] = SER_TYPE_JSON;
            types[SER_TYPE_YAML] = SER_TYPE_YAML;
        }

        static IAssetSerializer<T>* getSerializer() {
            static_assert("Must be implemented for given type!");
            return nullptr;
        }

        static IAsset* createNewAsset(const char* name, UUID8 uuid, uint8_t flags) {
            static_assert("Must be implemented for given type!");
            return nullptr;
        }

        static IAsset* destroyAsset(IAsset* asset) {
            static_assert("Must be implemented for given type!");
            return asset;
        }

        static void deserializeBin(FileEntry* entry, IAsset* asset, const Stream& stream, uint8_t flags) {
            getSerializer()->deserializeImpl(entry, asset, stream, flags);       
        }
        static void deserializeJson(FileEntry* entry, IAsset* asset, const json& jsonF, uint8_t flags) {
            getSerializer()->deserializeImpl(entry, asset, jsonF, flags);      
        }
        static void deserializeYaml(FileEntry* entry, IAsset* asset, const yamlNode& node, uint8_t flags) {
            getSerializer()->deserializeImpl(entry, asset, node, flags);
        }

        static void serializeBin(FileEntry* entry, const IAsset* asset, const Stream& stream, uint8_t flags) {
            getSerializer()->serializeImpl(entry, asset, stream, flags);
        }
        static void serializeJson(FileEntry* entry, const IAsset* asset, json& jsonF, uint8_t flags) {
            getSerializer()->serializeImpl(entry, asset, jsonF, flags);
        }
        static void serializeYaml(FileEntry* entry, const IAsset* asset, yamlEmit& emit, uint8_t flags) {
            getSerializer()->serializeImpl(entry, asset, emit, flags);
        }

    protected:
        T* asAsset(IAsset* asset) const {
            return reinterpret_cast<T*>(asset);
        }

        const T* asAsset(const IAsset* asset) const {
            return reinterpret_cast<const T*>(asset);
        }
    };

    namespace detail {
        struct Asset {
            Type* type = nullptr;
            AssetRef (*createAsset)(uint32_t, uint8_t) = nullptr;
            AssetRef (*destroyAsset)(AssetRef) = nullptr;

            SerializerType serializeTypes[3]{};

            void (*serBin)(FileEntry*, AssetRef, const Stream&, uint8_t) = nullptr;
            void (*serJSON)(FileEntry*, AssetRef, json&, uint8_t) = nullptr;
            void (*serYAML)(FileEntry*, AssetRef, yamlEmit&, uint8_t) = nullptr;

            void (*deserBin)(FileEntry*, AssetRef, const Stream&, uint8_t) = nullptr;
            void (*deserJSON)(FileEntry*, AssetRef, const json&, uint8_t) = nullptr;
            void (*deserYAML)(FileEntry*, AssetRef, const yamlNode&, uint8_t) = nullptr;

            template<typename T> static Asset* getAsset();
            static std::vector<Asset*>& getAssets() {
                static std::vector<Asset*> assets{};
                return assets;
            }

            static Asset* getAssetByHash(uint32_t hash) {
                auto& assets = getAssets();
                for (size_t i = 0; i < assets.size(); i++) {
                    if (assets[i]->type->hash == hash) {
                        return assets[i];
                    }
                }
                return nullptr;
            }

            template<typename T>
            static void addAsset(Asset* asset, Type* type) {
                if (asset->type != nullptr) { return; }
                asset->type = type;

                //IAssetSerializer<T>::getSerializerTypes(asset->serializeTypes);

                //asset->createAsset = IAssetSerializer<T>::createNewAsset;
                //asset->destroyAsset = IAssetSerializer<T>::destroyAsset;
                //
                //asset->serBin = IAssetSerializer<T>::serializeBin;
                //asset->serJSON = IAssetSerializer<T>::serializeJson;
                //asset->serYAML = IAssetSerializer<T>::serializeYaml;
                //
                //asset->deserBin = IAssetSerializer<T>::deserializeBin;
                //asset->deserJSON = IAssetSerializer<T>::deserializeJson;
                //asset->deserYAML = IAssetSerializer<T>::deserializeYaml;
                getAssets().push_back(asset);
            }
        };

        template<typename T>
        struct ValidatedAsset {
            static const Asset* Value;
        };
        template<typename T>
        inline const Asset* ValidatedAsset<T>::Value = nullptr;
    }

}

#define REGISTER_ASSET(TYPE)\
DEFINE_TYPE(TYPE); \
template<> \
inline JEngine::detail::Asset* JEngine::detail::Asset::getAsset<TYPE>() { \
  static JEngine::detail::Asset asset; \
  /*JEngine::detail::Asset::addAsset<TYPE>(&asset, JEngine::TypeHelpers::getType<TYPE>());*/ \
  return &asset; \
}\
template<> \
inline const JEngine::detail::Asset* JEngine::detail::ValidatedAsset<TYPE>::Value = JEngine::detail::Asset::getAsset<TYPE>(); \
