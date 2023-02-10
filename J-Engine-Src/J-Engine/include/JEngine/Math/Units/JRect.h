#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Units/JVector.h>

namespace JEngine {
    template<typename T>
    struct JRect {
    public:
        static const JRect Zero;

        JRect();
        JRect(const T x, const T y, const T w, const T h);
        JRect(const JVector2<T>& min, const JVector2<T>& max);

        bool operator==(const JRect<T>& other) const;
        bool operator!=(const JRect<T>& other) const;

        JRect<T>& setMinMax(const JVector2<T>& min, const JVector2<T>& max);

        JVector2<T> getSize() const;
        JVector2<T> getCenter() const;

        JVector2<T>& getMin();
        JVector2<T>& getMax();

        const JVector2<T>& getMin() const;
        const JVector2<T>& getMax() const;

        bool contains(const JVector2<T>& point) const;
        bool intersects(const JRect& rect) const;

    private:
        friend struct Serializable<JRect<T>>;
        friend struct YAML::convert<JRect<T>>;
        JVector2<T> _min;
        JVector2<T> _max;
    };

    template<typename T> inline const JRect<T> JEngine::JRect<T>::Zero = JRect<T>();
    template<typename T> inline JEngine::JRect<T>::JRect() : _min(0, 0), _max(0, 0) { }
    template<typename T> inline JEngine::JRect<T>::JRect(const T x, const T y, const T w, const T h) : _min(x, y), _max(x + w, y + h) {  }
    template<typename T> inline JEngine::JRect<T>::JRect(const JVector2<T>& min, const JVector2<T>& max) : _min(min), _max(max) { }

    template<typename T>
    inline bool JRect<T>::operator==(const JRect<T>& other) const {
        return _min == other._min && _max == other._max;
    }

    template<typename T>
    inline bool JRect<T>::operator!=(const JRect<T>& other) const {
        return !(*this == other);
    }

    template<typename T>
    inline JRect<T>& JRect<T>::setMinMax(const JVector2<T>& min, const JVector2<T>& max) {
        _min = min;
        _max = max;
        return *this;
    }

    template<typename T> inline JVector2<T> JEngine::JRect<T>::getSize() const { return JVector2<T>((_max.x - _min.x), (_max.y - _min.y)); }
    template<typename T> inline JVector2<T> JEngine::JRect<T>::getCenter() const { return T(_min.x + (_max.x - _min.x) / static_cast<TB>(2), _min.y + (_max.y - _min.y) / static_cast<TB>(2)); }

    template<typename T> inline JVector2<T>& JEngine::JRect<T>::getMin() { return _min; }
    template<typename T> inline JVector2<T>& JEngine::JRect<T>::getMax() { return _max; }

    template<typename T> inline const JVector2<T>& JEngine::JRect<T>::getMin() const { return _min; }
    template<typename T> inline const JVector2<T>& JEngine::JRect<T>::getMax() const { return _max; }

    template<typename T>
    inline bool JEngine::JRect<T>::contains(const JVector2<T>& point) const {
        return point.x >= _min.x && point.x <= _max.x && point.y >= _min.y && point.y <= _max.y;
    }

    template<typename T>
    inline bool JEngine::JRect<T>::intersects(const JRect<T>& rect) const {
        return !(_min.x > rect._max.x || _min.y > rect._max.y || _max.x < rect._min.x || _max.y < rect._min.y);
    }
}

#pragma region Serialization
//YAML
namespace YAML {
    template<typename T>
    yamlOut& operator<<(yamlOut& yamlOut, const JEngine::JRect<T>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << itemRef.getMin() << itemRef.getMax() << YAML::EndSeq;
        return yamlOut;
    }

    template<typename T>
    struct convert<JEngine::JRect<T>> {
        static Node encode(const JEngine::JRect<T>& rhs) {
            Node node;
            node.push_back(rhs._min);
            node.push_back(rhs._max);
            return node;
        }

        static bool decode(const Node& node, JEngine::JRect<T>& rhs) {
            if (!node.IsSequence() || node.size() < 2) { return false; }
            rhs._min = node[0].as<JEngine::JRect<T>>();
            rhs._max = node[1].as<JEngine::JRect<T>>();
            return true;
        }
    };
}

//JSON and Binary
namespace JEngine {
    template<typename T>
    struct Serializable<JRect<T>> {
        static bool deserializeJson(JRect<T>& itemRef, json& jsonF, const JRect<T>& defaultValue);
        static bool serializeJson(const JRect<T>& itemRef, json& jsonF);

        static bool deserializeBinary(JRect<T>& itemRef, std::istream& stream);
        static bool serializeBinary(const JRect<T>& itemRef, std::ostream& stream);
    };

    template<typename T>
    inline bool Serializable<JRect<T>>::deserializeJson(JRect<T>& itemRef, json& jsonF, const JRect<T>& defaultValue) {
        Serialization::deserialize(itemRef._min, jsonF["min"], defaultValue._min);
        Serialization::deserialize(itemRef._max, jsonF["max"], defaultValue._max);
        return true;
    }

    template<typename T>
    inline bool Serializable<JRect<T>>::serializeJson(const JRect<T>& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._min, jsonF["min"]);
        Serialization::serialize(itemRef._max, jsonF["max"]);
        return true;
    }

    template<typename T>
    inline bool Serializable<JRect<T>>::deserializeBinary(JRect<T>& itemRef, std::istream& stream) {
        Serialization::deserialize(itemRef._min, stream);
        Serialization::deserialize(itemRef._max, stream);
        return true;
    }

    template<typename T>
    inline bool Serializable<JRect<T>>::serializeBinary(const JRect<T>& itemRef, std::ostream& stream) {
        Serialization::serialize(itemRef._min, stream);
        Serialization::serialize(itemRef._max, stream);
        return true;
    }
}
#pragma endregion

typedef JEngine::JRect<int> JRecti;
typedef JEngine::JRect<float> JRectf;