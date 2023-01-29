#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Utility/ConstSpan.h>
#include <JEngine/Math/Units/JVector.h>
#include <array>

namespace JEngine {
    struct JMatrix : public ISerializable<JMatrix, sizeof(float) * 16> {
    public:
        static const JMatrix Zero;
        static const JMatrix Identity;

        JMatrix();
        JMatrix(const float a00, const float a01, const float a02,
                const float a10, const float a11, const float a12,
                const float a20, const float a21, const float a22);

        JMatrix(const float* input);
        JMatrix(const std::array<float, 16>& input);

        JMatrix(const JVector2f& position, const float rotation, const JVector2f& scale);

        JMatrix& setTRS(const JVector2f& position, const float rotation, const JVector2f& scale);

        float& operator[](const int32_t i);
        const float& operator[](const int32_t i) const;

        Span<float> asSpan();
        const ConstSpan<float> asConstSpan() const;

        const float* getMatrix() const;

        JMatrix getInverse() const;

        float& at(const int32_t column, const int32_t row);
        const float& at(const int32_t column, const int32_t row) const;

        JMatrix& combine(const JMatrix& matrix);

        const JVector2f transformPoint(const float x, const float y) const;
        const JVector2f transformPoint(const JVector2f vec) const;

        const JRectf transformRect(const JVector2f& min, const JVector2f& max) const;
        const JRectf transformRect(const JRectf& rectangle) const;

        const void decompose(JVector2f& position, float& rotation, JVector2f& scale) const;

        JRectf& transformRect(JRectf& rectangle) const;

        JMatrix& translate(const float x, const float y);
        JMatrix& translate(const JVector2f& offset);

        JMatrix& rotate(const float angle);
        JMatrix& rotate(const float angle, const float centerX, const float centerY);
        JMatrix& rotate(const float angle, const JVector2f& offset);

        JMatrix& scale(const float scaleX, const float scaleY);
        JMatrix& scale(const JVector2f& scale);

        JMatrix& scale(const float scaleX, const float scaleY, const float centerX, const float centerY);
        JMatrix& scale(const float scaleX, const float scaleY, const JVector2f& center);

        JMatrix& scale(const JVector2f& scale, const float centerX, const float centerY);
        JMatrix& scale(const JVector2f& scale, const JVector2f& center);

        static JMatrix ortho(const float left, const float right, const float bottom, const float top);
        static JMatrix ortho(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json& jsonF) const;
    
    private:
        float _mat[16];
    };

    const bool operator ==(const JMatrix& lhs, const JMatrix& rhs);
    const bool operator !=(const JMatrix& lhs, const JMatrix& rhs);

    const JVector2f operator *(const JMatrix& lhs, const JVector2f& rhs);

    const JMatrix operator *(const JMatrix& lhs, const JMatrix& rhs);
    JMatrix& operator *=(JMatrix& lhs, const JMatrix& rhs);
}

typedef JEngine::JMatrix JMatrix4f;