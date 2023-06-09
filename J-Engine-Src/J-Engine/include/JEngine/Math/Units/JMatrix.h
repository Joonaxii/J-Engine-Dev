#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Math/Units/JVector.h>
#include <array>

namespace JEngine {
    struct JMatrix4f {
    public:
        static const JMatrix4f Zero;
        static const JMatrix4f Identity;

        JMatrix4f();
        JMatrix4f(const float a00, const float a01, const float a02,
                const float a10, const float a11, const float a12,
                const float a20, const float a21, const float a22);

        JMatrix4f(const float* input);
        JMatrix4f(const std::array<float, 16>& input);

        JMatrix4f(const JVector2f& position, const float rotation, const JVector2f& scale);

        JMatrix4f& setTRS(const JVector2f& position, const float rotation, const JVector2f& scale);

        float& operator[](const int32_t i);
        const float& operator[](const int32_t i) const;

        Span<float> asSpan();
        const ConstSpan<float> asConstSpan() const;

        const float* getMatrix() const;

        JMatrix4f getInverse() const;

        float& at(const int32_t column, const int32_t row);
        const float& at(const int32_t column, const int32_t row) const;

        JMatrix4f& combine(const JMatrix4f& matrix);

        JVector2f transformPoint(const float x, const float y) const;
        JVector2f transformPoint(const JVector2f vec) const;

        JRectf transformRect(const JVector2f& min, const JVector2f& max) const;
        JRectf transformRect(const JRectf& rectangle) const;

        void decompose(JVector2f& position, float& rotation, JVector2f& scale) const;

        JRectf& transformRect(JRectf& rectangle) const;

        JMatrix4f& translate(const float x, const float y);
        JMatrix4f& translate(const JVector2f& offset);

        JMatrix4f& rotate(const float angle);
        JMatrix4f& rotate(const float angle, const float centerX, const float centerY);
        JMatrix4f& rotate(const float angle, const JVector2f& offset);

        JMatrix4f& scale(const float scaleX, const float scaleY);
        JMatrix4f& scale(const JVector2f& scale);

        JMatrix4f& scale(const float scaleX, const float scaleY, const float centerX, const float centerY);
        JMatrix4f& scale(const float scaleX, const float scaleY, const JVector2f& center);

        JMatrix4f& scale(const JVector2f& scale, const float centerX, const float centerY);
        JMatrix4f& scale(const JVector2f& scale, const JVector2f& center);

        static JMatrix4f ortho(const float left, const float right, const float bottom, const float top);
        static JMatrix4f ortho(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);

    private:
        friend struct Serializable<JMatrix4f>;
        friend struct YAML::convert<JMatrix4f>;
        float _mat[16];
    };

    const bool operator ==(const JMatrix4f& lhs, const JMatrix4f& rhs);
    const bool operator !=(const JMatrix4f& lhs, const JMatrix4f& rhs);

    const JVector2f operator *(const JMatrix4f& lhs, const JVector2f& rhs);

    const JMatrix4f operator *(const JMatrix4f& lhs, const JMatrix4f& rhs);
    JMatrix4f& operator *=(JMatrix4f& lhs, const JMatrix4f& rhs);

}


#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JMatrix4f& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq;

        const float* mat = reinterpret_cast<const float*>(&itemRef);
        for (size_t i = 0; i < 16; i++) {
            yamlOut << mat[i];
        }
        yamlOut << YAML::EndSeq;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::JMatrix4f> {
        static Node encode(const JEngine::JMatrix4f& rhs) {
            Node node;
            for (size_t i = 0; i < 16; i++) {
                node.push_back(rhs._mat[i]);
            }
            return node;
        }

        static bool decode(const Node& node, JEngine::JMatrix4f& rhs) {
            if (!node.IsSequence() || node.size() < 16) { return false; }
            for (size_t i = 0; i < 16; i++) {
                rhs._mat[i] = node[i].as<float>();
            }
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<JMatrix4f>::deserializeJson(JMatrix4f& itemRef, json& jsonF, const JMatrix4f& defaultValue) {
        itemRef = defaultValue;
        const size_t size = jsonF.size() > 16 ? 16 : jsonF.size();
        if (size > 0) {
            for (size_t i = 0; i < size; i++) {
                itemRef._mat[i] = jsonF.at(i).get<float>();
            }
        }
        return true;
    }

    template<>
    inline bool Serializable<JMatrix4f>::serializeJson(const JMatrix4f& itemRef, json& jsonF) {
        for (size_t i = 0; i < 16; i++) {
            jsonF.push_back(itemRef._mat[i]);
        }
        return true;
    }
}
#pragma endregion