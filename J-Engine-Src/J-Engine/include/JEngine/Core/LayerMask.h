#pragma once
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    struct LayerMask {
    public:
        LayerMask();
        LayerMask(const uint32_t value);

#pragma region Operators Self
        operator bool() const;

        const LayerMask operator~() const;
#pragma endregion

#pragma region Operators UInt32/Size_t/Templated
        operator uint32_t() const;
        operator size_t() const;

        template<typename T>
        const bool operator<(const T& other) const {
            return _value < other;
        }

        template<typename T>
        const bool operator>(const T& other) const {
            return _value > other;
        }

        template<typename T>
        const bool operator<=(const T& other) const {
            return _value <= other;
        }

        template<typename T>
        const bool operator>=(const T& other) const {
            return _value >= other;
        }

        template<typename T>
        const bool operator==(const T& other) const {
            return _value == other;
        }

        template<typename T>
        const bool operator!=(const T& other) const {
            return _value != other;
        }

        template<typename T>
        const LayerMask operator&(const T& other) const {
            return LayerMask(_value & other);
        }

        template<typename T>
        const LayerMask operator|(const T& other) const {
            return LayerMask(_value | other);
        }

        template<typename T>
        const LayerMask operator^(const T& other) const {
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
        inline const bool operator<(const LayerMask& other) const {
            return _value < other._value;
        }

        template<>
        inline const bool operator>(const LayerMask& other) const {
            return _value > other._value;
        }

        template<>
        inline const bool operator<=(const LayerMask& other) const {
            return _value <= other._value;
        }

        template<>
        inline const bool operator>=(const LayerMask& other) const {
            return _value >= other._value;
        }

        template<>
        inline const bool operator==(const LayerMask& other) const {
            return _value == other._value;
        }

        template<>
        inline const bool operator!=(const LayerMask& other) const {
            return _value != other._value;
        }

        template<>
        inline const LayerMask operator&(const LayerMask& other) const {
            return LayerMask(_value & other._value);
        }

        template<>
        inline const LayerMask operator|(const LayerMask& other) const {
            return LayerMask(_value | other._value);
        }

        template<>
        inline const LayerMask operator^(const LayerMask& other) const {
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
        uint32_t _value;
    };

#pragma region Serialization
    template<>
    inline const bool Serializable<LayerMask>::deserializeJson(LayerMask& itemRef, json& jsonF, const LayerMask& defaultValue) {
        itemRef._value = jsonF.is_number_integer() ? jsonF.get<uint32_t>() : defaultValue;
        return true;
    }

    template<>
    inline const bool Serializable<LayerMask>::serializeJson(const LayerMask& itemRef, json& jsonF) {
        jsonF.update(itemRef._value);
        return true;
    }
#pragma endregion
}