#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/Math/Random/IWeighted.h>

namespace JEngine {
    template<typename TValue, typename TWeight>
    class WeightedValue : public IWeighted<TWeight>,  public ISerializable<WeightedValue<TValue, TWeight>, sizeof(TValue) + sizeof(TWeight)> {
    public:

        WeightedValue() : IWeighted<TWeight>(), _value() { }
        WeightedValue(const TValue& value, const TWeight& weight) : IWeighted<TWeight>(weight), _value(value) {}

        void setValue(const TValue& value) {
            _value = value;
        }

        const TValue& getValue() const { return _value; }

        const bool serializeJSON(json& jsonF) const {
            IWeighted<TWeight>::serializeJSON(jsonF);
            jsonF["value"] = _value;
            return true;
        }

        const bool deserializeJSON(json& jsonF) {
            IWeighted<TWeight>::deserializeJSON(jsonF);
            _value = jsonF.value("value", TValue());
            return true;
        }
    private:
        TValue _value;
    };
}