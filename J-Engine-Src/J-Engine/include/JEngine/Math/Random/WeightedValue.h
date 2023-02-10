#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Random/IWeighted.h>

namespace JEngine {
    template<typename TValue, typename TWeight>
    class WeightedValue : public IWeighted<TWeight> {
    public:

        WeightedValue() : IWeighted<TWeight>(), _value() { }
        WeightedValue(const TValue& value, const TWeight& weight) : IWeighted<TWeight>(weight), _value(value) {}

        void setValue(const TValue& value) { _value = value; }
        const TValue& getValue() const { return _value; }

    private:
        friend struct Serializable<WeightedValue<TValue, TWeight>>;
        friend struct YAML::convert<WeightedValue<TValue, TWeight>>;
        TValue _value;
    };
}

#pragma region Serialization
//YAML
namespace YAML {
    template<typename TValue, typename TWeight>
    yamlOut& operator<<(yamlOut& yamlOut, const JEngine::WeightedValue<TValue, TWeight>& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << itemRef.getWeight() << itemRef.getValue() << YAML::EndSeq;
        return yamlOut;
    }

    template<typename TValue, typename TWeight>
    struct convert<JEngine::WeightedValue<TValue, TWeight>> {
        static Node encode(const JEngine::WeightedValue<TValue, TWeight>& rhs) {
            Node node;
            node.push_back(rhs.getWeight());
            node.push_back(rhs.getValue());
            return node;
        }

        static bool decode(const Node& node, JEngine::WeightedValue<TValue, TWeight>& rhs) {
            if (!node.IsSequence() || node.size() < 2) { return false; }
            rhs._weight = node[0].as<TWeight>();
            rhs._value  = node[1].as<TValue>();
            return true;
        }
    };
}

//JSON and Binary
namespace JEngine {
    template<typename TValue, typename TWeight>
    struct Serializable<WeightedValue<TValue, TWeight>> {
        static bool deserializeJson(WeightedValue<TValue, TWeight>& itemRef, json& jsonF, const WeightedValue<TValue, TWeight>& defaultValue);
        static bool serializeJson(const WeightedValue<TValue, TWeight>& itemRef, json& jsonF);

        static bool deserializeBinary(WeightedValue<TValue, TWeight>& itemRef, std::istream& stream);
        static bool serializeBinary(const WeightedValue<TValue, TWeight>& itemRef, std::ostream& stream);
    };

    template<typename TValue, typename TWeight>
    inline bool Serializable<WeightedValue<TValue, TWeight>>::deserializeJson(WeightedValue<TValue, TWeight>& itemRef, json& jsonF, const WeightedValue<TValue, TWeight>& defaultValue) {
        Serialization::deserialize(reinterpret_cast<IWeighted<TWeight>&>(itemRef), jsonF["weight"]);
        Serialization::deserialize(itemRef._value, jsonF["value"]);
        return true;
    }

    template<typename TValue, typename TWeight>
    inline bool Serializable<WeightedValue<TValue, TWeight>>::serializeJson(const WeightedValue<TValue, TWeight>& itemRef, json& jsonF) {
        Serialization::serialize(reinterpret_cast<const IWeighted<TWeight>&>(itemRef), jsonF["weight"]);
        Serialization::serialize(itemRef._value, jsonF["value"]);
        return true;
    }

    template<typename TValue, typename TWeight>
    inline bool Serializable<WeightedValue<TValue, TWeight>>::deserializeBinary(WeightedValue<TValue, TWeight>& itemRef, std::istream& stream) {
        Serialization::deserialize(reinterpret_cast<IWeighted<TWeight>&>(itemRef), stream);
        Serialization::deserialize(itemRef._value, stream);
        return true;
    }

    template<typename TValue, typename TWeight>
    inline bool Serializable<WeightedValue<TValue, TWeight>>::serializeBinary(const WeightedValue<TValue, TWeight>& itemRef, std::ostream& stream) {
        Serialization::serialize(reinterpret_cast<const IWeighted<TWeight>&>(itemRef), stream);
        Serialization::serialize(itemRef._value, stream);
        return true;
    }
}
#pragma endregion