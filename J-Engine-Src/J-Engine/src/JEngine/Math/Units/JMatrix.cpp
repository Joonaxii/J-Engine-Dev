#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Math.h>
#include <xmmintrin.h>

namespace JEngine {
    JMatrix4f::JMatrix4f(const JVector2f& position, float rotation, const JVector2f& scale) : JMatrix4f(1.0f) {
        this->translate(position);
        this->rotate(rotation);
        this->scale(scale);
    }

    JMatrix4f::JMatrix4f(const JVector3f& position, const JVector3f& rotation, const JVector3f& scale) : JMatrix4f(1.0f) {
        this->translate(position);
        this->rotate(rotation);
        this->scale(scale);
    }

    JMatrix4f& JMatrix4f::setTRS(const JVector2f& position, float rotation, const JVector2f& scale) {
        *this = JMatrix4f(1.0f);
        this->translate(position);
        this->rotate(rotation);
        this->scale(scale);
        return *this;
    }

    JMatrix4f& JMatrix4f::setTRS(const JVector3f& position, const JVector3f& rotation, const JVector3f& scale) {
        *this = JMatrix4f(1.0f);
        this->translate(position);
        this->rotate(rotation);
        this->scale(scale);
        return *this;
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
        return JMatrix4f(1.0f);
    }

    float& JMatrix4f::operator[](int32_t i) {
        JENGINE_CORE_ASSERT(i > -1 && i < 16 && "Index out of range of matrix!");
        return _mat[i];
    }
    const float& JMatrix4f::operator[](int32_t i) const {
        JENGINE_CORE_ASSERT(i > -1 && i < 16 && "Index out of range of matrix!");
        return _mat[i];
    }

    float& JMatrix4f::at(int32_t column, int32_t row) {
        JENGINE_CORE_ASSERT(column > -1 && column < 4 && "Column outside the range of matrix!");
        JENGINE_CORE_ASSERT(row > -1 && row < 4 && "Row outside the range of matrix!");
        return _mat[row * 4 + column];
    }
    const float& JMatrix4f::at(int32_t column, int32_t row) const {
        JENGINE_CORE_ASSERT(column > -1 && column < 4 && "Column outside the range of matrix!");
        JENGINE_CORE_ASSERT(row > -1 && row < 4 && "Row outside the range of matrix!");
        return _mat[row * 4 + column];
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
        //__m128 row1 = _mm_load_ps(matrix._mat + 0);
        //__m128 row2 = _mm_load_ps(matrix._mat + 4);
        //__m128 row3 = _mm_load_ps(matrix._mat + 8);
        //__m128 row4 = _mm_load_ps(matrix._mat + 12);
        //for (int i = 0, j = 0; i < 4; i++, j += 4) {
        //    __m128 brod1 = _mm_set1_ps(_mat[j + 0]);
        //    __m128 brod2 = _mm_set1_ps(_mat[j + 1]);
        //    __m128 brod3 = _mm_set1_ps(_mat[j + 2]);
        //    __m128 brod4 = _mm_set1_ps(_mat[j + 3]);
        //    __m128 row = _mm_add_ps(
        //        _mm_add_ps(
        //            _mm_mul_ps(brod1, row1),
        //            _mm_mul_ps(brod2, row2)),
        //        _mm_add_ps(
        //            _mm_mul_ps(brod3, row3),
        //            _mm_mul_ps(brod4, row4)));
        //    _mm_store_ps(&_mat[j], row);
        //}

       const float* a = _mat;
       const float* b = matrix._mat;
       
       *this = JMatrix4f(
           a[0x0] * b[0x0] + a[0x4] * b[0x1] + a[0xC] * b[0x3],
           a[0x0] * b[0x4] + a[0x4] * b[0x5] + a[0xC] * b[0x7],
           a[0x0] * b[0xC] + a[0x4] * b[0xD] + a[0xC] * b[0xF],
           a[0x1] * b[0x0] + a[0x5] * b[0x1] + a[0xD] * b[0x3],
           a[0x1] * b[0x4] + a[0x5] * b[0x5] + a[0xD] * b[0x7],
           a[0x1] * b[0xC] + a[0x5] * b[0xD] + a[0xD] * b[0xF],
           a[0x3] * b[0x0] + a[0x7] * b[0x1] + a[0xF] * b[0x3],
           a[0x3] * b[0x4] + a[0x7] * b[0x5] + a[0xF] * b[0x7],
           a[0x3] * b[0xC] + a[0x7] * b[0xD] + a[0xF] * b[0xF]);
        return *this;
    }

    JVector2f JMatrix4f::transformPoint(float x, float y) const {
        return {
            _mat[0x0] * x + _mat[0x1] * y + _mat[0x3],
            _mat[0x4] * x + _mat[0x5] * y + _mat[0x7]
        };
    }
    JVector2f JMatrix4f::transformPoint(const JVector2f& vec) const {
        return transformPoint(vec.x, vec.y);
    }

    JVector3f JMatrix4f::transformPoint(float x, float y, float z) const {
        return {
             _mat[0x0] * x + _mat[0x1] * y + _mat[0x2] * z + _mat[0x3],
             _mat[0x4] * x + _mat[0x5] * y + _mat[0x6] * z + _mat[0x7],
             _mat[0x8] * x + _mat[0x9] * y + _mat[0xA] * z + _mat[0xB],
        };
    }
    JVector3f JMatrix4f::transformPoint(const JVector3f& vec) const {
        return transformPoint(vec.x, vec.y, vec.z);
    }

    JVector2f JMatrix4f::transformVector(float x, float y) const {
        return {
            _mat[0x0] * x + _mat[0x1] * y,
            _mat[0x4] * x + _mat[0x5] * y
        };
    }
    JVector2f JMatrix4f::transformVector(const JVector2f& vec) const {
        return transformVector(vec.x, vec.y);
    }

    JVector3f JMatrix4f::transformVector(float x, float y, float z) const {
        return {
            _mat[0x0] * x + _mat[0x1] * y + _mat[0x2] * z,
            _mat[0x4] * x + _mat[0x5] * y + _mat[0x6] * z,
            _mat[0x8] * x + _mat[0x9] * y + _mat[0xA] * z
        };
    }
    JVector3f JMatrix4f::transformVector(const JVector3f& vec) const {
        return transformVector(vec.x, vec.y, vec.z);
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

    JMatrix4f& JMatrix4f::translate(float x, float y) {
        JMatrix4f translation(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, 0,
            0, 0, 0, 1);
        return combine(translation);
    }
    JMatrix4f& JMatrix4f::translate(const JVector2f& offset) {
        return translate(offset.x, offset.y);
    }

    JMatrix4f& JMatrix4f::translate(float x, float y, float z) {
        JMatrix4f translation(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1);
        return combine(translation);
    }
    JMatrix4f& JMatrix4f::translate(const JVector3f& offset) {
        return translate(offset.x, offset.y, offset.z);
    }

    JMatrix4f& JMatrix4f::rotate(float angle) {
        const float rad = angle * DEG_2_RAD;
        const float cos = std::cos(rad);
        const float sin = std::sin(rad);

        JMatrix4f rotation(
            cos, -sin, 0,
            sin, cos , 0,
            0  , 0   , 1);
        return combine(rotation);
    }

    JMatrix4f& JMatrix4f::rotate(float x, float y, float z) {
        float rad = x * DEG_2_RAD;
        float cos = std::cos(rad);
        float sin = std::sin(rad);

        JMatrix4f rotationX(
            1, 0, 0, 0,
            0, cos, -sin, 0,
            0, sin, cos, 0,
            0, 0, 0, 1);

        rad = y * DEG_2_RAD;
        cos = std::cos(rad);
        sin = std::sin(rad);

        JMatrix4f rotationY(
            cos, 0, -sin, 0,
            0, 1, 0, 0,
            sin, 0, cos, 0,
            0, 0, 0, 1);

        rad = z * DEG_2_RAD;
        cos = std::cos(rad);
        sin = std::sin(rad);

        JMatrix4f rotationZ(
            cos, -sin, 0, 0,
            sin, cos, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
        return combine(rotationX * rotationY * rotationZ);
    }
    JMatrix4f& JMatrix4f::rotate(const JVector3f& eulers) {
        return rotate(eulers.x, eulers.y, eulers.z);
    }

    JMatrix4f& JMatrix4f::scale(float scaleX, float scaleY) {
        JMatrix4f scaling(
            scaleX, 0, 0, 0,
            0, scaleY, 0, 0,
            0, 0, 1.0f, 0,
            0, 0, 0, 1);
        return combine(scaling);
    }
    JMatrix4f& JMatrix4f::scale(const JVector2f& scale) {
        return this->scale(scale.x, scale.y);
    }

    JMatrix4f& JMatrix4f::scale(float scaleX, float scaleY, float scaleZ) {
        JMatrix4f scaling(
            scaleX, 0, 0, 0,
            0, scaleY, 0, 0,
            0, 0, scaleZ, 0,
            0, 0, 0, 1);
        return combine(scaling);
    }
    JMatrix4f& JMatrix4f::scale(const JVector3f& scale) {
        return this->scale(scale.x, scale.y, scale.z);
    }

    JMatrix4f JMatrix4f::ortho(const float left, const float right, const float bottom, const float top) {
        JMatrix4f mat(1.0f);

        mat.at(0, 0) = 2.0f / (right - left);
        mat.at(1, 1) = 2.0f / (top - bottom);
        mat.at(2, 2) = 1.0f;
        mat.at(3, 0) = (right + left) / (right - left);
        mat.at(3, 1) = (top + bottom) / (top - bottom);
        return mat;
    }

    JMatrix4f JMatrix4f::ortho(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar) {
        JMatrix4f mat(1.0f);

        mat.at(0, 0) = 2.0f / (right - left);
        mat.at(1, 1) = 2.0f / (top - bottom);
        mat.at(2, 2) = -2.0f / (zFar - zNear);
        mat.at(3, 0) = (right + left) / (right - left);
        mat.at(3, 1) = (top + bottom) / (top - bottom);
        mat.at(3, 2) = zNear / (zFar - zNear);
        return mat;
    }
}