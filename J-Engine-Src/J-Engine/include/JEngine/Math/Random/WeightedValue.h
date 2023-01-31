#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Random/IWeighted.h>

namespace JEngine {
    template<typename TValue, typename TWeight>
    class WeightedValue : public IWeighted<TWeight> {
    public:

        WeightedValue() : IWeighted<TWeight>(), _value() { }
        WeightedValue(const TValue& value, const TWeight& weight) : IWeighted<TWeight>(weight), _value(value) {}

        void setValue(const TValue& value) {
            _value = value;
        }

        const TValue& getValue() const { return _value; }

    private:
        friend struct Serializable<WeightedValue<TValue, TWeight>>;
        TValue _value;
    };

#pragma region Serialization
    template<typename TValue, typename TWeight>
    struct Serializable<WeightedValue<TValue, TWeight>> {
        static const bool deserializeJson(WeightedValue<TValue, TWeight>& itemRef, json& jsonF, const WeightedValue<TValue, TWeight>& defaultValue);
        static const bool serializeJson(const WeightedValue<TValue, TWeight>& itemRef, json& jsonF);

        static const bool deserializeBinary(WeightedValue<TValue, TWeight>& itemRef, std::istream& stream);
        static const bool serializeBinary(const WeightedValue<TValue, TWeight>& itemRef, std::ostream& stream);
    };

    template<typename TValue, typename TWeight>
    inline const bool Serializable<WeightedValue<TValue, TWeight>>::deserializeJson(WeightedValue<TValue, TWeight>& itemRef, json& jsonF, const WeightedValue<TValue, TWeight>& defaultValue) {
        Serialization::deserialize(reinterpret_cast<IWeighted<TWeight>&>(itemRef), jsonF["weight"]);
        Serialization::deserialize(itemRef._value, jsonF["value"]);
        return true;
    }

    template<typename TValue, typename TWeight>
    inline const bool Serializable<WeightedValue<TValue, TWeight>>::serializeJson(const WeightedValue<TValue, TWeight>& itemRef, json& jsonF) {
        Serialization::serialize(reinterpret_cast<const IWeighted<TWeight>&>(itemRef), jsonF["weight"]);
        Serialization::serialize(itemRef._value, jsonF["value"]);
        return true;
    }

    template<typename TValue, typename TWeight>
    inline const bool Serializable<WeightedValue<TValue, TWeight>>::deserializeBinary(WeightedValue<TValue, TWeight>& itemRef, std::istream& stream) {
        Serialization::deserialize(reinterpret_cast<IWeighted<TWeight>&>(itemRef), stream);
        Serialization::deserialize(itemRef._value, stream);
        return true;
    }

    template<typename TValue, typename TWeight>
    inline const bool Serializable<WeightedValue<TValue, TWeight>>::serializeBinary(const WeightedValue<TValue, TWeight>& itemRef, std::ostream& stream) {
        Serialization::serialize(reinterpret_cast<const IWeighted<TWeight>&>(itemRef), stream);
        Serialization::serialize(itemRef._value, stream);
        return true;
    }
#pragma endregion
}