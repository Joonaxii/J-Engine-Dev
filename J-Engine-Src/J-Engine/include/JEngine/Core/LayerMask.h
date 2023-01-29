#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>

namespace JEngine {
    struct LayerMask : public ISerializable<LayerMask, sizeof(uint32_t)> {
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

        const bool serializeJson(json& jsonF) const;
        const bool deserializeJson(json& jsonF);

        const bool serializeBinary(std::ostream& stream) const;
        const bool deserializeBinary(std::istream& stream);

    private:
        uint32_t _value;
    };
}