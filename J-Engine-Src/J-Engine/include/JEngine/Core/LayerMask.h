#pragma once
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    struct LayerMask {
    public:
        LayerMask();
        LayerMask(const uint32_t value);

#pragma region Operators Self
        operator bool() const;

        LayerMask operator~() const;
#pragma endregion

#pragma region Operators UInt32/Size/Templated
        operator uint32_t() const;
        operator size_t() const;

        template<typename T>
        bool operator<(const T& other) const {
            return _value < other;
        }

        template<typename T>
        bool operator>(const T& other) const {
            return _value > other;
        }

        template<typename T>
        bool operator<=(const T& other) const {
            return _value <= other;
        }

        template<typename T>
        bool operator>=(const T& other) const {
            return _value >= other;
        }

        template<typename T>
        bool operator==(const T& other) const {
            return _value == other;
        }

        template<typename T>
        bool operator!=(const T& other) const {
            return _value != other;
        }

        template<typename T>
        LayerMask operator&(const T& other) const {
            return LayerMask(_value & other);
        }

        template<typename T>
        LayerMask operator|(const T& other) const {
            return LayerMask(_value | other);
        }

        template<typename T>
        LayerMask operator^(const T& other) const {
            return LayerMask(_value ^ other);
        }

        template<typename T>
        LayerMask& operator&=(const T& other) {
            _value &= other;
            return *this;
        }

        template<typename T>
        LayerMask& operator|=(const T& other) {
            _value |= other;
            return *this;
        }

        template<typename T>
        LayerMask& operator^=(const T& other) {
            _value ^= other;
            return *this;
        }


        template<>
        inline bool operator<(const LayerMask& other) const {
            return _value < other._value;
        }

        template<>
        inline bool operator>(const LayerMask& other) const {
            return _value > other._value;
        }

        template<>
        inline bool operator<=(const LayerMask& other) const {
            return _value <= other._value;
        }

        template<>
        inline bool operator>=(const LayerMask& other) const {
            return _value >= other._value;
        }

        template<>
        inline bool operator==(const LayerMask& other) const {
            return _value == other._value;
        }

        template<>
        inline bool operator!=(const LayerMask& other) const {
            return _value != other._value;
        }

        template<>
        inline LayerMask operator&(const LayerMask& other) const {
            return LayerMask(_value & other._value);
        }

        template<>
        inline LayerMask operator|(const LayerMask& other) const {
            return LayerMask(_value | other._value);
        }

        template<>
        inline LayerMask operator^(const LayerMask& other) const {
            return LayerMask(_value ^ other._value);
        }

        template<>
        inline LayerMask& operator&=(const LayerMask& other) {
            _value &= other._value;
            return *this;
        }

        template<>
        inline LayerMask& operator|=(const LayerMask& other) {
            _value |= other._value;
            return *this;
        }

        template<>
        inline LayerMask& operator^=(const LayerMask& other) {
            _value ^= other._value;
            return *this;
        }

#pragma endregion
    private:
        friend struct Serializable<LayerMask>;
        friend struct YAML::convert<LayerMask>;
        uint32_t _value;
    };
}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::LayerMask& itemRef) {
        yamlOut << YAML::Hex << static_cast<const uint32_t>(itemRef);
        return yamlOut;
    }

    template<>
    struct convert<JEngine::LayerMask> {
        static Node encode(const JEngine::LayerMask& rhs) {
            Node node;
            node.push_back(static_cast<const uint32_t>(rhs));
            return node;
        }

        static bool decode(const Node& node, JEngine::LayerMask& rhs) {
            rhs = JEngine::LayerMask(node.as<uint32_t>());
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<LayerMask>::deserializeJson(LayerMask& itemRef, const json& jsonF, const LayerMask& defaultValue) {
        itemRef._value = jsonF.is_number_integer() ? jsonF.get<uint32_t>() : defaultValue._value;
        return true;
    }

    template<>
    inline bool Serializable<LayerMask>::serializeJson(const LayerMask& itemRef, json& jsonF) {
        jsonF.update(itemRef._value);
        return true;
    }
}
#pragma endregion