#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Math.h>
#include <cassert>

namespace JEngine {
    const JMatrix4f JMatrix4f::Zero = JMatrix4f({
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f });

    const JMatrix4f JMatrix4f::Identity = JMatrix4f({
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 });

    JMatrix4f::JMatrix4f() : _mat{ 0.0f } {
        _mat[0] = 1.0f;
        _mat[5] = 1.0f;
        _mat[10] = 1.0f;
        _mat[15] = 1.0f;
    }

    JMatrix4f::JMatrix4f(const float* input) {
        memcpy(_mat, input, 16 * sizeof(float));
    }

    JMatrix4f::JMatrix4f(const std::array<float, 16>& input) {
        memcpy(_mat, input.data(), 16 * sizeof(float));
    }

    JMatrix4f::JMatrix4f(const JVector2f& position, const float rotation, const JVector2f& scale) : JMatrix4f(JMatrix4f::Identity) {
        this->translate(position);
        this->rotate(rotation);
        this->scale(scale);
    }

    JMatrix4f& JMatrix4f::setTRS(const JVector2f& position, const float rotation, const JVector2f& scale) {
        *this = JMatrix4f(JMatrix4f::Identity);

        this->translate(position);
        this->rotate(rotation);
        this->scale(scale);

        return *this;
    }

    const float* JMatrix4f::getMatrix() const {
        return _mat;
    }

    JMatrix4f JMatrix4f::getInverse() const {
        float det = _mat[0] * (_mat[15] * _mat[5] - _mat[7] * _mat[13]) -
            _mat[1] * (_mat[15] * _mat[4] - _mat[7] * _mat[12]) +
            _mat[3] * (_mat[13] * _mat[4] - _mat[5] * _mat[12]);

        if (det != 0.0f) {
            const float mult = 1.0f / det;

            return JMatrix4f(
                (_mat[15] * _mat[5] - _mat[7] * _mat[13]) * mult,
                -(_mat[15] * _mat[4] - _mat[7] * _mat[12]) * mult,
                (_mat[13] * _mat[4] - _mat[5] * _mat[12]) * mult,
                -(_mat[15] * _mat[1] - _mat[3] * _mat[13]) * mult,
                (_mat[15] * _mat[0] - _mat[3] * _mat[12]) * mult,
                -(_mat[13] * _mat[0] - _mat[1] * _mat[12]) * mult,
                (_mat[7] * _mat[1] - _mat[3] * _mat[5]) * mult,
                -(_mat[7] * _mat[0] - _mat[3] * _mat[4]) * mult,
                (_mat[5] * _mat[0] - _mat[1] * _mat[4]) * mult);
        }
        return Identity;

    }

    JMatrix4f::JMatrix4f(
        const float a00, const float a01, const float a02,
        const float a10, const float a11, const float a12,
        const float a20, const float a21, const float a22) {
        _mat[0] = a00; _mat[4] = a01; _mat[8] = 0.f; _mat[12] = a02;
        _mat[1] = a10; _mat[5] = a11; _mat[9] = 0.f; _mat[13] = a12;
        _mat[2] = 0.f; _mat[6] = 0.f; _mat[10] = 1.f; _mat[14] = 0.f;
        _mat[3] = a20; _mat[7] = a21; _mat[11] = 0.f; _mat[15] = a22;
    }

    float& JMatrix4f::operator[](const int32_t i) {
        assert(i > -1 && i < 16 && "Index out of range of matrix!");
        return _mat[i];
    }

    const float& JMatrix4f::operator[](const int32_t i) const {
        assert(i > -1 && i < 16 && "Index out of range of matrix!");
        return _mat[i];
    }

    Span<float> JMatrix4f::asSpan() {
        return Span<float>(_mat, 16);
    }

    const ConstSpan<float> JMatrix4f::asConstSpan() const {
        return ConstSpan<float>(_mat, 16);
    }

    float& JMatrix4f::at(const int32_t column, const int32_t row) {
        assert(column > -1 && column < 4 && "Column outside the range of matrix!");
        assert(row > -1 && row < 4 && "Row outside the range of matrix!");
        return (*this)[row * 4 + column];
    }

    const float& JMatrix4f::at(const int32_t column, const int32_t row) const {
        assert(column > -1 && column < 4 && "Column outside the range of matrix!");
        assert(row > -1 && row < 4 && "Row outside the range of matrix!");
        return (*this)[row * 4 + column];
    }

    const bool operator==(const JMatrix4f& lhs, const JMatrix4f& rhs) {
        auto matLhs = lhs.getMatrix();
        auto matRhs = rhs.getMatrix();
        for (size_t i = 0; i < 16; i++)
        {
            if (matLhs[i] != matRhs[i]) { return false; }
        }
        return true;
    }

    const bool operator!=(const JMatrix4f& lhs, const JMatrix4f& rhs) {
        return !(lhs == rhs);
    }

    const JVector2f operator*(const JMatrix4f& lhs, const JVector2f& rhs) {
        return lhs.transformPoint(rhs);
    }

    const JMatrix4f operator*(const JMatrix4f& lhs, const JMatrix4f& rhs) {
        return JMatrix4f(lhs).combine(rhs);
    }

    JMatrix4f& operator*=(JMatrix4f& lhs, const JMatrix4f& rhs) {
        return lhs.combine(rhs);
    }

    JMatrix4f& JMatrix4f::combine(const JMatrix4f& matrix) {
        const float* a = _mat;
        const float* b = matrix._mat;

        *this = JMatrix4f(
            a[0] * b[0 ] + a[4] * b[1 ] + a[12] * b[3],
            a[0] * b[4 ] + a[4] * b[5 ] + a[12] * b[7],
            a[0] * b[12] + a[4] * b[13] + a[12] * b[15],
            a[1] * b[0 ] + a[5] * b[1 ] + a[13] * b[3],
            a[1] * b[4 ] + a[5] * b[5 ] + a[13] * b[7],
            a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
            a[3] * b[0 ] + a[7] * b[1 ] + a[15] * b[3],
            a[3] * b[4 ] + a[7] * b[5 ] + a[15] * b[7],
            a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);
        return *this;
    }

    JVector2f JMatrix4f::transformPoint(const float x, const float y) const {
        return JVector2f(_mat[0] * x + _mat[4] * y + _mat[12],
                         _mat[1] * x + _mat[5] * y + _mat[13]);
    }

    JVector2f JMatrix4f::transformPoint(const JVector2f vec) const {
        return transformPoint(vec.x, vec.y);
    }

    JRectf JMatrix4f::transformRect(const JVector2f& min, const JVector2f& max) const {
        const JVector2f points[4] {
            transformPoint(min),
            transformPoint(min.x, max.y),
            transformPoint(max),
            transformPoint(max.x, min.y),
        };

        JVector2f minO(points[0]);
        JVector2f maxO(points[0]);
        for (size_t i = 1; i < 4; i++) {
            const auto& vec = points[i];
            minO.x = vec.x < minO.x ? vec.x : minO.x;
            maxO.x = vec.x > maxO.x ? vec.x : maxO.x;

            minO.y = vec.y < minO.y ? vec.y : minO.y;
            maxO.y = vec.y > maxO.y ? vec.y : maxO.y;
        }
        return JRectf(minO, maxO);
    }

    JRectf JMatrix4f::transformRect(const JRectf& rectangle) const {
        return transformRect(rectangle.getMin(), rectangle.getMax());
    }

    void JMatrix4f::decompose(JVector2f& position, float& rotation, JVector2f& scale) const {
        position.x = _mat[12];
        position.y = _mat[13];

        const float m00 = _mat[0];
        const float m01 = _mat[1];

        const float m10 = _mat[4];
        const float m11 = _mat[5];

        scale.x = Math::sign(m00) * std::sqrtf((m00 * m00) + (m01 * m01));
        scale.y = Math::sign(m11) * std::sqrtf((m10 * m10) + (m11 * m11));

        rotation = std::atan2f(m10, m11);
    }

    JRectf& JMatrix4f::transformRect(JRectf& rectangle) const {
        auto& min = rectangle.getMin();
        auto& max = rectangle.getMax();
        const JVector2f points[4]{
             transformPoint(min),
             transformPoint(min.x, max.y),
             transformPoint(max),
             transformPoint(max.x, min.y),
        };

        JVector2f minO(points[0]);
        JVector2f maxO(points[0]);
        for (size_t i = 1; i < 4; i++) {
            const auto& vec = points[i];
            minO.x = vec.x < minO.x ? vec.x : minO.x;
            maxO.x = vec.x > maxO.x ? vec.x : maxO.x;

            minO.y = vec.y < minO.y ? vec.y : minO.y;
            maxO.y = vec.y > maxO.y ? vec.y : maxO.y;
        }
        return rectangle.setMinMax(minO, maxO);
    }

    JMatrix4f& JMatrix4f::translate(const float x, const float y) {
        JMatrix4f translation(1, 0, x,
                            0, 1, y,
                            0, 0, 1);
        return combine(translation);
    }

    JMatrix4f& JMatrix4f::translate(const JVector2f& offset) {
        return translate(offset.x, offset.y);
    }

    JMatrix4f& JMatrix4f::rotate(const float angle) {
        const float rad = angle * DEG_2_RAD;
        const float cos = std::cos(rad);
        const float sin = std::sin(rad);

        JMatrix4f rotation(cos, -sin, 0,
                         sin, cos , 0,
                         0  , 0   , 1);

        return combine(rotation);
    }

    JMatrix4f& JMatrix4f::rotate(const float angle, const float centerX, const float centerY) {
        const float rad = angle * DEG_2_RAD;
        const float cos = std::cos(rad);
        const float sin = std::sin(rad);

        JMatrix4f rotation(cos, -sin, centerX * (1 - cos) + centerY * sin,
                         sin,  cos, centerY * (1 - cos) - centerX * sin,
                         0, 0, 1);
        return combine(rotation);
    }

    JMatrix4f& JMatrix4f::rotate(const float angle, const JVector2f& offset) {
        return rotate(angle, offset.x, offset.y);
    }

    JMatrix4f& JMatrix4f::scale(const float scaleX, const float scaleY) {
        JMatrix4f scaling(scaleX, 0,      0,
                        0,      scaleY, 0,
                        0,      0,      1);
        return combine(scaling);
    }

    JMatrix4f& JMatrix4f::scale(const JVector2f& scale) {
        return this->scale(scale.x, scale.y);
    }

    JMatrix4f& JMatrix4f::scale(const float scaleX, const float scaleY, const float centerX, const float centerY) {
        JMatrix4f scaling(scaleX, 0,      centerX * (1 - scaleX),
                        0,      scaleY, centerY * (1 - scaleY),
                        0,      0,      1);

        return combine(scaling);
    }

    JMatrix4f& JMatrix4f::scale(const float scaleX, const float scaleY, const JVector2f& center) {
        return scale(scaleX, scaleY, center.x, center.y);
    }

    JMatrix4f& JMatrix4f::scale(const JVector2f& scale, const float centerX, const float centerY) {
        return this->scale(scale.x, scale.y, centerX, centerY);
    }

    JMatrix4f& JMatrix4f::scale(const JVector2f& scale, const JVector2f& center) {
        return this->scale(scale.x, scale.y, center.x, center.y);
    }

    JMatrix4f JMatrix4f::ortho(const float left, const float right, const float bottom, const float top) {
        JMatrix4f mat = JMatrix4f::Identity;

        mat.at(0, 0) = 2.0f / (right - left);
        mat.at(1, 1) = 2.0f / (top - bottom);
        mat.at(2, 2) = 1.0f;
        mat.at(3, 0) = (right + left) / (right - left);
        mat.at(3, 1) = (top + bottom) / (top - bottom);
        return mat;
    }

    JMatrix4f JMatrix4f::ortho(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar) {
        JMatrix4f mat = JMatrix4f::Identity;

        mat.at(0, 0) = 2.0f / (right - left);
        mat.at(1, 1) = 2.0f / (top - bottom);
        mat.at(2, 2) = -2.0f / (zFar - zNear);
        mat.at(3, 0) = (right + left) / (right - left);
        mat.at(3, 1) = (top + bottom) / (top - bottom);
        mat.at(3, 2) = zNear / (zFar - zNear);
        return mat;
    }
}