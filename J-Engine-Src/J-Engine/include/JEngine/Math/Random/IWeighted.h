#pragma once
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
	template<typename TWeight>
    class IWeighted {
	public:
		IWeighted() : _weight(0) {}
		IWeighted(const TWeight weight) : _weight(weight) {}

		TWeight getWeight() const { return _weight; }
		void setWeight(const TWeight weight) const { _weight = weight; }

		const bool serializeJSON(json& jsonF) const {
			jsonF["weight"] = _weight;
			return true;
		}

		const bool deserializeJSON(json& jsonF) {
			_weight = jsonF.value("weight", 0);
			return true;
		}
	protected:
        friend struct Serializable<IWeighted<TWeight>>;
		TWeight _weight;
    };

#pragma region Serialization
    template<typename TWeight>
    struct Serializable<IWeighted<TWeight>> {
        static bool deserializeJson(IWeighted<TWeight>& itemRef, json& jsonF, const IWeighted<TWeight>& defaultValue);
        static bool serializeJson(const IWeighted<TWeight>& itemRef, json& jsonF);

        static bool deserializeBinary(IWeighted<TWeight>& itemRef, std::istream& stream);
        static bool serializeBinary(const IWeighted<TWeight>& itemRef, std::ostream& stream);
    };

    template<typename TWeight>
    inline bool Serializable<IWeighted<TWeight>>::deserializeJson(IWeighted<TWeight>& itemRef, json& jsonF, const IWeighted<TWeight>& defaultValue) {
        return Serialization::deserialize(itemRef._weight, jsonF, defaultValue._weight);
    }

    template<typename TWeight>
    inline bool Serializable<IWeighted<TWeight>>::serializeJson(const IWeighted<TWeight>& itemRef, json& jsonF) {
        return Serialization::serialize(itemRef._weight, jsonF);
    }

    template<typename TWeight>
    inline bool Serializable<IWeighted<TWeight>>::deserializeBinary(IWeighted<TWeight>& itemRef, std::istream& stream) {
        return Serialization::deserialize(itemRef._weight, stream);
    }

    template<typename TWeight>
    inline bool Serializable<IWeighted<TWeight>>::serializeBinary(const IWeighted<TWeight>& itemRef, std::ostream& stream) {
        return Serialization::serialize(itemRef._weight, stream);
    }
#pragma endregion
}