#include <JEngine/Math/Math.h>
#include <iostream>

namespace JEngine::Math {

    const int32_t LOG2_TABLE_64[64] = {
63,  0, 58,  1, 59, 47, 53,  2,
60, 39, 48, 27, 54, 33, 42,  3,
61, 51, 37, 40, 49, 18, 28, 20,
55, 30, 34, 11, 43, 14, 22,  4,
62, 57, 46, 52, 38, 26, 32, 41,
50, 36, 17, 19, 29, 10, 13, 21,
56, 45, 25, 31, 35, 16,  9, 12,
44, 24, 15,  8, 23,  7,  6,  5 };


    const int32_t log2(uint64_t value) {
        if (value == 0) { return 0; }
        value |= value >> 1;
        value |= value >> 2;
        value |= value >> 4;
        value |= value >> 8;
        value |= value >> 16;
        value |= value >> 32;
        return LOG2_TABLE_64[((uint64_t)((value - (value >> 1)) * 0x07EDD5E59A4E28C2)) >> 58];
    }

    const int32_t findFirstLSB(const uint64_t value) {
        const uint64_t val = value ^ (value & (value - 1));
        return log2(val);
    }

    //constexpr uint8_t multiplyBytes(const uint8_t a, const uint8_t b) {
    //    return static_cast<uint8_t>((static_cast<uint16_t>(a) * static_cast<uint16_t>(b) + 0xFFU) >> 8);
    //}

    const float dot(const JVector2f& lhs, const JVector2f& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    const float sqrMagnitude(const JVector2f& vec) {
        return vec.x * vec.x + vec.y * vec.y;
    }

    const float magnitude(const JVector2f& vec) {
        return sqrtf(sqrMagnitude(vec));
    }

    const JVector2f smoothDamp(const JVector2f& current, const JVector2f& target, JVector2f& velocity, const float smoothTime, const float delta) {
        if (delta <= 0) { return current; }

        const float sTime = smoothTime < 0.00001f ? 0.00001f : smoothTime;
        const float omega = 2.0f / sTime;

        float x = omega * delta;
        float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

        float cX = current.x - target.x;
        float cY = current.y - target.y;

        JVector2f to = target;

        to.x = current.x - cX;
        to.y = current.y - cY;

        float tmpX = (velocity.x + omega * cX) * delta;
        float tmpY = (velocity.y + omega * cY) * delta;

        velocity.x = (velocity.x - omega * tmpX) * exp;
        velocity.y = (velocity.y - omega * tmpY) * exp;

        float oX = target.x + (cX + tmpX) * exp;
        float oY = target.y + (cY + tmpY) * exp;

        float origX = target.x - current.x;
        float origY = target.y - current.y;

        float outMinX = oX - target.x;
        float outMinY = oY - target.y;

        if (origX * outMinX + origY * outMinY > 0) {
            oX = target.x;
            oY = target.y;

            velocity.x = (oX - target.x) / delta;
            velocity.y = (oY - target.y) / delta;
        }
        return JVector2f(oX, oY);
    }

    //void generateMatrix(sf::Transform& tr, const sf::Vector2f& pos, const float rot, const sf::Vector2f& scale) {
    //    generateMatrix(reinterpret_cast<float*>(&tr), pos, rot, scale);
    //}

    //void generateMatrix(float* mat, const sf::Vector2f& pos, const float rot, const sf::Vector2f& scale) {
    //    mat[12] = pos.x;
    //    mat[13] = pos.y;

    //    float rotRad = rot * DEG_2_RAD;
    //    float sin = sinf(rotRad);
    //    float cos = cosf(rotRad);

    //    mat[0] = cos * scale.x;
    //    mat[1] = -sin * scale.x;

    //    mat[4] = sin * scale.y;
    //    mat[5] = cos * scale.y;
    //}

    const JVector2f rotateDeg(const JVector2f& lhs, const float deg) {
        return rotateRad(lhs, deg * DEG_2_RAD);
    }

    const JVector2f rotateRad(const JVector2f& lhs, const float rad) {
        const float sin = sinf(rad);
        const float cos = cosf(rad);
        return JVector2f(lhs.x * cos - lhs.y * sin, lhs.x * sin + lhs.y * cos);
    }

    const float angle(const JVector2f& lhs, const JVector2f& rhs) {
        float den = sqrtf(sqrMagnitude(lhs) * sqrMagnitude(rhs));
        if (den < 0.000001f) { return 0.0f; }

        float dotP = clamp(dot(lhs, rhs) / den, -1.0f, 1.0f);
        return acosf(dotP) * RAD_2_DEG;
    }

    const float signedAngle(const JVector2f& lhs, const JVector2f& rhs) {
        const float uAngle = angle(lhs, rhs);
        float signF = sign(lhs.x * rhs.y - lhs.y * rhs.x);
        return uAngle * signF;
    }

    const float invLerp(const JVector2f& a, const JVector2f& b, const JVector2f& v) {
        JVector2f ab = b - a;
        JVector2f av = v - a;

        float dotB = dot(ab, ab);
        return dotB == 0.0f ? 0.0f : dot(av, ab) / dotB;
    }
}