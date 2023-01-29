#pragma once
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Units/JVector.h>

namespace JEngine {
    template<typename T>
    struct JVertex : public ISerializable<JVertex<T>, sizeof(JColor32) + sizeof(JVector2f) + sizeof(T)> {
    public:
        T position;
        JColor32 color;
        JVector2f uv;

        JVertex();
        JVertex(const T& position, const JColor32& color, const JVector2f& uv);
     
        const bool deserializeBinary(std::istream& stream);
        const bool serializeBinary(std::ostream& stream) const;

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json& jsonF) const;
        static const bool jsonToBinary(json& jsonF, std::ostream& stream);
    };

    template<typename T> inline JVertex<T>::JVertex() : position(), color(0, 0, 0), uv(0.0f, 0.0f) { }

    template<typename T>
    inline JVertex<T>::JVertex(const T& position, const JColor32& color, const JVector2f& uv) : position(position), color(color), uv(uv) { }

    template<typename T> inline const bool JVertex<T>::deserializeBinary(std::istream& stream) {
        position.deserializeBinary(stream);
        color.deserializeBinary(stream);
        uv.deserializeBinary(stream);
        return true;
    }

    template<typename T> inline const bool JVertex<T>::serializeBinary(std::ostream& stream) const {
        position.serializeBinary(stream);
        color.serializeBinary(stream);
        uv.serializeBinary(stream);
        return true;
    }

    template<typename T> inline const bool JVertex<T>::deserializeJson(json& jsonF) {
        position.deserializeJson(jsonF["position"]);
        color.deserializeJson(jsonF["color"]);
        uv.deserializeJson(jsonF["uv"]);
        return true;
    }

    template<typename T> inline const bool JVertex<T>::serializeJson(json& jsonF) const {
        position.serializeJson(jsonF["position"]);
        color.serializeJson(jsonF["color"]);
        uv.serializeJson(jsonF["uv"]);
        return true;
    }

    template<typename T> inline const bool JVertex<T>::jsonToBinary(json& jsonF, std::ostream& stream) {
        JVertex temp;
        temp.deserializeJson(jsonF);
        return temp.serializeBinary(stream);
    }
}

typedef JEngine::JVertex<JVector2f> JVertex2f;
typedef JEngine::JVertex<JVector3f> JVertex3f;