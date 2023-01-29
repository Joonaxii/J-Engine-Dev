#pragma once

#include <cstdint>
#include <JEngine/Math/Math.h>
#include <JEngine/Helpers/Helpers.h>

namespace JEngine {
    template<typename T>
    class Flags : public ISerializable<Flags<T>, sizeof(T)> {
    public:
        Flags() : _value(0) {}
        Flags(T value) : _value(value) {}

        const T operator &(const T& other) const {
            return _value & other;
        }

        const T operator |(const T& other) const {
            return _value | other;
        }

        Flags<T>& operator &=(const T& other) {
            _value &= other;
            return *this;
        }

        Flags<T>& operator |=(const T& other) {
            _value |= other;
            return *this;
        }

        const T operator ^(const T& other) const {
            return _value ^ other;
        }

        Flags<T>& operator ^=(const T& other) {
            _value ^= other;
            return *this;
        }

        const T operator ~() const {
            return ~_value;
        }

        operator T() const {
            return _value;
        }

        explicit operator bool() const {
            return bool(_value);
        }

        const bool operator ==(const Flags<T>& other) const {
            return _value == other._value;
        }

        const bool operator !=(const Flags<T>& other) const {
            return _value != other._value;
        }

        const bool operator ==(const T other) const {
            return _value == other;
        }

        const bool operator !=(const T other) const {
            return _value != other;
        }

        const T& getValue() const {
            return _value;
        }

        const T select(const T mask) const {
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

        void clear() {
            _value = 0;
        }

        const bool deserializeJson(json& jsonF) {
            _value = jsonF.value("flags", 0);
            return true;
        }

        const bool serializeJson(json& jsonF) const {
            jsonF["flags"] = _value;
            return true;
        }

        const bool deserializeJson(json& jsonF, std::string* names, const int count) {
            if (!names || count < 1) {
                *&jsonF = _value;
                return true;
            }

            _value = jsonF.value("flags", 0);
            return true;
        }

        const bool serializeJson(json& jsonF, std::string* names, const int count) const {
            if (!names || count < 1) {
                _value = jsonF.get<T>();
                return true;
            }
            return true;
        }

        const bool deserializeBinary(std::istream& stream) {
            stream.read(reinterpret_cast<char*>(&_value), sizeof(T));
            return true;
        }

        const bool serializeBinary(std::ostream& stream) const {
            stream.write(reinterpret_cast<const char*>(&_value), sizeof(T));
            return true;
        }

    private:
        T _value;

    };
}

typedef JEngine::Flags<uint8_t> UI8Flags;
typedef JEngine::Flags<uint16_t> UI16Flags;
typedef JEngine::Flags<uint32_t> UI32Flags;
typedef JEngine::Flags<uint64_t> UI64Flags;