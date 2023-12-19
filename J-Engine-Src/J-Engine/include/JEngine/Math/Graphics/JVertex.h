#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Units/JVector.h>

namespace JEngine {
#pragma pack(push, 1)
    struct JVertex {
        JVector3f position;
        JColor32 color;
        JVector2f uv;

        JVertex() : 
            position(), color(JColor32::White), uv(0.0f, 0.0f) {}
        JVertex(const JVector3f& position, const JColor32& color, const JVector2f& uv) : 
            position(position), color(color), uv(uv) { }
    };
#pragma pack(pop)
}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JVertex& itemRef) {
        yamlOut << 
            YAML::BeginMap << 
            YAML::Key << "position" << itemRef.position <<
            YAML::Key << "color" << itemRef.color <<
            YAML::Key << "uv" << itemRef.uv <<
            YAML::EndMap;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::JVertex> {
        static Node encode(const JEngine::JVertex& rhs) {
            Node node;
            node["position"] = rhs.position;
            node["color"] = rhs.color;
            node["uv"] = rhs.uv;
            return node;
        }

        static bool decode(const Node& node, JEngine::JVertex& rhs) {
            if (!node.IsMap() || node.size() < 3) { return false; }
            rhs.position = node["position"].as<JVector3f>();
            rhs.color = node["color"].as<JEngine::JColor32>();
            rhs.uv = node["uv"].as<JVector2f>();
            return true;
        }
    };
}

//JSON/Binary
namespace JEngine {

    template<>
    inline bool Serializable<JVertex>::deserializeJson(JVertex& itemRef, const json& jsonF, const JVertex& defaultValue) {
        Serialization::deserialize(itemRef.position, jsonF["position"], defaultValue.position);
        Serialization::deserialize(itemRef.color, jsonF["color"], defaultValue.color);
        Serialization::deserialize(itemRef.uv, jsonF["uv"], defaultValue.uv);
        return true;
    }

    template<>
    inline bool Serializable<JVertex>::serializeJson(const JVertex& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.position, jsonF["position"]);
        Serialization::serialize(itemRef.color, jsonF["color"]);
        Serialization::serialize(itemRef.uv, jsonF["uv"]);
        return true;
    }
}
#pragma endregion