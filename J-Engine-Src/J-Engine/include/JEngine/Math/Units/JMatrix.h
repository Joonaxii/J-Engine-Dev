#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Math/Units/JVector.h>
#include <array>

namespace JEngine {
    namespace detail {
        inline uint32_t getSIMDMaskAND(const __m128& simd) {
            return simd.m128_u32[0] & simd.m128_u32[1] & simd.m128_u32[2] & simd.m128_u32[3];
        }
    }

    struct alignas(16) JMatrix4f {
    public:
        constexpr JMatrix4f() : _mat{
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
        } { }

        constexpr JMatrix4f(float v) : _mat{
            v, 0, 0, 0,
            0, v, 0, 0,
            0, 0, v, 0,
            0, 0, 0, v,
        } { }
        constexpr JMatrix4f(
            float a00, float a01, float a02,
            float a10, float a11, float a12,
            float a20, float a21, float a22) : _mat {
                a00, a10, 0.f, a20,
                a01, a11, 0.f, a21,
                0.f, 0.f, 1.f, 0.f,
                a02, a12, 0.f, a22,
        } { }
        
        constexpr JMatrix4f(
            float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33) : _mat {
                a00, a10, a20, a30,
                a01, a11, a21, a31,
                a02, a12, a22, a32,
                a03, a13, a23, a33,
        } { }

        JMatrix4f(const JVector2f& position, float rotation, const JVector2f& scale);
        JMatrix4f(const JVector3f& position, const JVector3f& rotation, const JVector3f& scale);

        JMatrix4f& setTRS(const JVector2f& position, float rotation, const JVector2f& scale);
        JMatrix4f& setTRS(const JVector3f& position, const JVector3f& rotation, const JVector3f& scale);

        float& operator[](int32_t i);
        const float& operator[](int32_t i) const;

        float& at(int32_t column, int32_t row);
        const float& at(int32_t column, int32_t row) const;

        Span<float> asSpan() { return Span<float>(_mat, 16); }
        ConstSpan<float> asConstSpan() const { return ConstSpan<float>(_mat, 16); }

        const float* getMatrix() const { return _mat; }

        bool operator==(const JMatrix4f& rhs) const {
            return (
                detail::getSIMDMaskAND(_mm_cmpeq_ps(_matVec[0], rhs._matVec[0])) &
                detail::getSIMDMaskAND(_mm_cmpeq_ps(_matVec[1], rhs._matVec[1])) &
                detail::getSIMDMaskAND(_mm_cmpeq_ps(_matVec[2], rhs._matVec[2])) &
                detail::getSIMDMaskAND(_mm_cmpeq_ps(_matVec[3], rhs._matVec[3]))
                ) == 0xFFFFFFFFU;
        }

        bool operator!=(const JMatrix4f& rhs) const {
            return !(*this == rhs);
        }

        JMatrix4f getInverse() const;

        JMatrix4f& combine(const JMatrix4f& matrix);

        JVector2f transformPoint(float x, float y) const;
        JVector2f transformPoint(const JVector2f& vec) const;

        JVector3f transformPoint(float x, float y, float z) const;
        JVector3f transformPoint(const JVector3f& vec) const;

        JVector2f transformVector(float x, float y) const;
        JVector2f transformVector(const JVector2f& vec) const;

        JVector3f transformVector(float x, float y, float z) const;
        JVector3f transformVector(const JVector3f& vec) const;

        JRectf transformRect(const JVector2f& min, const JVector2f& max) const;
        JRectf transformRect(const JRectf& rectangle) const;

        JRectf& transformRect(JRectf& rectangle) const;

        JMatrix4f& translate(float x, float y);
        JMatrix4f& translate(const JVector2f& offset);

        JMatrix4f& translate(float x, float y, float z);
        JMatrix4f& translate(const JVector3f& offset);

        JMatrix4f& rotate(float angle);
        JMatrix4f& rotate(float rotX, float rotY, float rotZ);
        JMatrix4f& rotate(const JVector3f& eulers);

        JMatrix4f& scale(float scaleX, float scaleY);
        JMatrix4f& scale(const JVector2f& scale);

        JMatrix4f& scale(float scaleX, float scaleY, float scaleZ);
        JMatrix4f& scale(const JVector3f& scale);

        static JMatrix4f ortho(float left, float right, float bottom, float top);
        static JMatrix4f ortho(float left, float right, float bottom, float top, float zNear, float zFar);

    private:
        friend struct Serializable<JMatrix4f>;
        friend struct YAML::convert<JMatrix4f>;

        union {
            float _mat[16];
            __m128 _matVec[4];
        };
    };

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
    inline bool Serializable<JMatrix4f>::deserializeJson(JMatrix4f& itemRef, const json& jsonF, const JMatrix4f& defaultValue) {
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