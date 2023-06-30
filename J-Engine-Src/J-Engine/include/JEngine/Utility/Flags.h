#pragma once

#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Math.h>
#include <JEngine/Helpers/Helpers.h>

namespace JEngine {
    template<typename T>
    class Flags {
    public:
        Flags() : _value(0) {}
        Flags(T value) : _value(value) {}

        T operator &(const T& other) const { return _value & other; }
        Flags<T>& operator &=(const T& other) {
            _value &= other;
            return *this;
        }

        T operator |(const T& other) const { return _value | other; }
        Flags<T>& operator |=(const T& other) {
            _value |= other;
            return *this;
        }

        T operator ^(const T& other) const { return _value ^ other; }
        Flags<T>& operator ^=(const T& other) {
            _value ^= other;
            return *this;
        }

        T operator ~() const { return ~_value; }

        operator T() const { return _value; }
        explicit operator bool() const { return bool(_value); }

        bool operator ==(const Flags<T>& other) const { return _value == other._value; }
        bool operator !=(const Flags<T>& other) const { return _value != other._value; }

        bool operator ==(const T other) const { return _value == other; }
        bool operator !=(const T other) const { return _value != other; }

        const T& getValue() const { return _value; }

        T select(const T mask) const {
            return (_value & mask) >> Math::findFirstLSB(mask);
        }

        void setValue(const T value, const T mask) {
            const T bits = Math::findFirstLSB(mask);
            _value &= ~mask;
            _value |= (value << bits) & mask;
        }

        const bool isBitSet(const int32_t flag) const {
            return _value & flag;
        }

        void setBit(const int32_t flag, const bool value) {
            if (value) {
                _value |= flag;
                return;
            }
            _value &= ~flag;
        }

        void clear() { _value = 0; }
    private:
        friend struct Serializable<Flags<T>>;
        friend struct YAML::convert<Flags<T>>;
        T _value;

    };
}

#pragma region Serialization
//YAML
namespace YAML {
    template<typename T>
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::Flags<T>& itemRef) {
        yamlOut << YAML::Hex << uint64_t(itemRef.getValue());
        return yamlOut;
    }   

    template<typename T>
    struct convert<JEngine::Flags<T>> {
        static Node encode(const JEngine::Flags<T>& rhs) {
            Node node;
            node.push_back(YAML::Hex);
            node.push_back(uint64_t(rhs._value));
            return node;
        }

        static bool decode(const Node& node, JEngine::Flags<T>& rhs) {
            rhs._value = T(node.as<uint64_t>());
            return true;
        }
    };  
}

//JSON and Binary
namespace JEngine {
    template<typename T>
    struct Serializable<Flags<T>> {
        static bool deserializeJson(Flags<T>& itemRef, json& jsonF, const Flags<T>& defaultValue);
        static bool serializeJson(const Flags<T>& itemRef, json& jsonF);

        static bool deserializeBinary(Flags<T>& itemRef, const Stream& stream);
        static bool serializeBinary(const Flags<T>& itemRef, const Stream& stream);
    };

    template<typename T>
    inline bool Serializable<Flags<T>>::deserializeJson(Flags<T>& itemRef, json& jsonF, const Flags<T>& defaultValue) {
        return Serialization::deserialize(itemRef._value, jsonF, defaultValue._value);
    }

    template<typename T>
    inline bool Serializable<Flags<T>>::serializeJson(const Flags<T>& itemRef, json& jsonF) {
        return Serialization::serialize(itemRef._value, jsonF);
    }

    template<typename T>
    inline bool Serializable<Flags<T>>::deserializeBinary(Flags<T>& itemRef, const Stream& stream) {
        return Serialization::deserialize(itemRef._value, stream);
    }

    template<typename T>
    inline bool Serializable<Flags<T>>::serializeBinary(const Flags<T>& itemRef, const Stream& stream) {
        return Serialization::serialize(itemRef._value, stream);
    }
}
#pragma endregion

typedef JEngine::Flags<uint8_t> UI8Flags;
typedef JEngine::Flags<uint16_t> UI16Flags;
typedef JEngine::Flags<uint32_t> UI32Flags;
typedef JEngine::Flags<uint64_t> UI64Flags;