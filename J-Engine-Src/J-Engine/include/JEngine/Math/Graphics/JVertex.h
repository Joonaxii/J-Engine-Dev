#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Units/JVector.h>

namespace JEngine {
    template<typename T>
    struct JVertex {
    public:
        T position;
        JColor32 color;
        JVector2f uv;

        JVertex();
        JVertex(const T& position, const JColor32& color, const JVector2f& uv);
    };

    template<typename T> inline JVertex<T>::JVertex() : position(), color(JColor32::White), uv(0.0f, 0.0f) { }

    template<typename T>
    inline JVertex<T>::JVertex(const T& position, const JColor32& color, const JVector2f& uv) : position(position), color(color), uv(uv) { }

}

#pragma region Serialization
//YAML
namespace YAML {
    template<typename T>
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JVertex<T>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << itemRef.position << itemRef.color << itemRef.uv << YAML::EndSeq;
        return yamlOut;
    }

    template<typename T>
    struct convert<JEngine::JVertex<T>> {
        static Node encode(const JEngine::JVertex<T>& rhs) {
            Node node;
            node.push_back(rhs.position);
            node.push_back(rhs.color);
            node.push_back(rhs.uv);
            return node;
        }

        static bool decode(const Node& node, JEngine::JVertex<T>& rhs) {
            if (!node.IsSequence() || node.size() < 3) { return false; }
            rhs.position = node[0].as<T>();
            rhs.color = node[0].as<JEngine::JColor32>();
            rhs.uv = node[0].as<JVector2f>();
            return true;
        }
    };
}

//JSON/Binary
namespace JEngine {
    template<typename T>
    struct Serializable<JVertex<T>> {
        static bool deserializeJson(JVertex<T>& itemRef, json& jsonF, const JVertex<T>& defaultValue);
        static bool serializeJson(const JVertex<T>& itemRef, json& jsonF);

        static bool deserializeBinary(JVertex<T>& itemRef, const Stream& stream);
        static bool serializeBinary(const JVertex<T>& itemRef, const Stream& stream);
    };

    template<typename T>
    inline bool Serializable<JVertex<T>>::deserializeJson(JVertex<T>& itemRef, json& jsonF, const JVertex<T>& defaultValue) {
        Serialization::deserialize(itemRef.position, jsonF["position"], defaultValue.position);
        Serialization::deserialize(itemRef.color, jsonF["color"], defaultValue.color);
        Serialization::deserialize(itemRef.uv, jsonF["uv"], defaultValue.uv);
        return true;
    }

    template<typename T>
    inline bool Serializable<JVertex<T>>::serializeJson(const JVertex<T>& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.position, jsonF["position"]);
        Serialization::serialize(itemRef.color, jsonF["color"]);
        Serialization::serialize(itemRef.uv, jsonF["uv"]);
        return true;
    }

    template<typename T>
    inline bool Serializable<JVertex<T>>::deserializeBinary(JVertex<T>& itemRef, const Stream& stream) {
        Serialization::deserialize(itemRef.position, stream);
        Serialization::deserialize(itemRef.color, stream);
        Serialization::deserialize(itemRef.uv, stream);
        return true;
    }

    template<typename T>
    inline bool Serializable<JVertex<T>>::serializeBinary(const JVertex<T>& itemRef, const Stream& stream) {
        Serialization::serialize(itemRef.position, stream);
        Serialization::serialize(itemRef.color, stream);
        Serialization::serialize(itemRef.uv, stream);
        return true;
    }
}
#pragma endregion

typedef JEngine::JVertex<JVector2f> JVertex2f;
typedef JEngine::JVertex<JVector3f> JVertex3f;