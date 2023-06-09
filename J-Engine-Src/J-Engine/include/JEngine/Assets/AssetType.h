#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    enum class AssetImporterType : uint32_t {
        Unknown         = 0x00,

        TextAsset       = 0x01,

        GLInclude       = 0x02,
        Shader          = 0x03,
        Material        = 0x04,

        Sprite          = 0x06,
        Atlas           = 0x07,

        Texture2D       = 0x08,
        RenderTexture   = 0x09,

        Prefab          = 0x18,
        Scene           = 0x19,

        Audio           = 0x30,

        InputConfig     = 0x38,
        
        
        ProjectSettings = 0x42,

        END_OF_BUILT_IN = 0x1000U,
    };
}
#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::AssetImporterType& itemRef) {
        yamlOut << YAML::Dec << static_cast<const uint32_t>(itemRef);
        return yamlOut;
    }

    template<>
    struct convert<JEngine::AssetImporterType> {
        static Node encode(const JEngine::AssetImporterType& rhs) {
            Node node;
            node.push_back(static_cast<const uint32_t>(rhs));
            return node;
        }

        static bool decode(const Node& node, JEngine::AssetImporterType& rhs) {
            rhs = JEngine::AssetImporterType(node.as<uint32_t>());
            return true;
        }
    };
}
//JSON
namespace JEngine {
    template<>
    inline bool Serializable<AssetImporterType>::deserializeJson(AssetImporterType& itemRef, json& jsonF, const AssetImporterType& defaultVal) {
        itemRef = jsonF.is_number() ? AssetImporterType(jsonF.get<uint32_t>()) : defaultVal;
        return true;
    }

    template<>
    inline bool Serializable<AssetImporterType>::serializeJson(const AssetImporterType& itemRef, json& jsonF) {
        jsonF = uint32_t(itemRef);
        return true;
    }
}
#pragma endregion