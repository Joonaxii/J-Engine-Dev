#pragma once
#include <JEngine/Math/Units/JVector.h>

namespace JEngine {
    template<typename T>
    struct JRect {
    public:
        constexpr JRect();
        constexpr JRect(const T x, const T y, const T w, const T h);
        constexpr JRect(const JVector2<T>& min, const JVector2<T>& max);

        constexpr bool operator==(const JRect<T>& other) const;
        constexpr bool operator!=(const JRect<T>& other) const;

        JRect<T>& setMinMax(const JVector2<T>& min, const JVector2<T>& max);

        constexpr JVector2<T> getSize() const;
        constexpr JVector2<T> getCenter() const;

        JVector2<T>& getMin();
        JVector2<T>& getMax();

        const JVector2<T>& getMin() const;
        const JVector2<T>& getMax() const;

        constexpr bool contains(const JVector2<T>& point) const;
        constexpr bool intersects(const JRect<T>& rect) const;

    private:
        JVector2<T> _min;
        JVector2<T> _max;
    };

    template<typename T> inline constexpr JEngine::JRect<T>::JRect() : _min(0, 0), _max(0, 0) { }
    template<typename T> inline constexpr JEngine::JRect<T>::JRect(const T x, const T y, const T w, const T h) : _min(x, y), _max(x + w, y + h) {  }
    template<typename T> inline constexpr JEngine::JRect<T>::JRect(const JVector2<T>& min, const JVector2<T>& max) : _min(min), _max(max) { }

    template<typename T>
    inline constexpr bool JRect<T>::operator==(const JRect<T>& other) const {
        return _min == other._min && _max == other._max;
    }

    template<typename T>
    inline constexpr bool JRect<T>::operator!=(const JRect<T>& other) const {
        return !(*this == other);
    }

    template<typename T>
    inline JRect<T>& JRect<T>::setMinMax(const JVector2<T>& min, const JVector2<T>& max) {
        _min = min;
        _max = max;
        return *this;
    }

    template<typename T> inline constexpr JVector2<T> JEngine::JRect<T>::getSize() const { return JVector2<T>((_max.x - _min.x), (_max.y - _min.y)); }
    template<typename T> inline constexpr JVector2<T> JEngine::JRect<T>::getCenter() const { return T(_min.x + (_max.x - _min.x) / T(2), _min.y + (_max.y - _min.y) / T(2)); }

    template<typename T> inline JVector2<T>& JEngine::JRect<T>::getMin() { return _min; }
    template<typename T> inline JVector2<T>& JEngine::JRect<T>::getMax() { return _max; }

    template<typename T> inline const JVector2<T>& JEngine::JRect<T>::getMin() const { return _min; }
    template<typename T> inline const JVector2<T>& JEngine::JRect<T>::getMax() const { return _max; }

    template<typename T>
    inline constexpr bool JEngine::JRect<T>::contains(const JVector2<T>& point) const {
        return point.x >= _min.x && point.x <= _max.x && point.y >= _min.y && point.y <= _max.y;
    }

    template<typename T>
    inline constexpr bool JEngine::JRect<T>::intersects(const JRect<T>& rect) const {
        return !(_min.x > rect._max.x || _min.y > rect._max.y || _max.x < rect._min.x || _max.y < rect._min.y);
    }
}

typedef JEngine::JRect<int> JRecti;
typedef JEngine::JRect<float> JRectf;