#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>

namespace JEngine {
	template<typename TWeight>
    class IWeighted : public ISerializable<IWeighted<TWeight>, sizeof(TWeight)> {
	public:
		IWeighted() : _weight(0) {}
		IWeighted(const TWeight weight) : _weight(weight) {}

		const TWeight getWeight() const { return _weight; }
		const void setWeight(const TWeight weight) const { _weight = weight; }

		const bool serializeJSON(json& jsonF) const {
			jsonF["weight"] = _weight;
			return true;
		}

		const bool deserializeJSON(json& jsonF) {
			_weight = jsonF.value("weight", 0);
			return true;
		}
	protected:
		TWeight _weight;
    };
}