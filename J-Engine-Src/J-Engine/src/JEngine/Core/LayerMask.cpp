#include <JEngine/Core/LayerMask.h>
#include <JEngine/IO/Serialization/Serialization.h>

namespace JEngine {
    LayerMask::LayerMask() : _value(0) {}
    LayerMask::LayerMask(const uint32_t value) : _value(value) {}

    LayerMask::operator uint32_t() const { return _value; }
    LayerMask::operator size_t() const { return _value; }
    const LayerMask LayerMask::operator~() const { return LayerMask(~_value); }
    LayerMask::operator bool() const { return bool(_value); }

    const bool LayerMask::serializeJson(json& jsonF) const {
        jsonF = _value;
        return true;
    }    
    
    const bool LayerMask::deserializeJson(json& jsonF) {
        _value = jsonF.get<uint32_t>();
        return true;
    }

    const bool LayerMask::serializeBinary(std::ostream& stream) const {
        Serialization::serializeBinary(stream, _value);
        return true;
    }

    const bool LayerMask::deserializeBinary(std::istream& stream) {
        Serialization::deserializeBinary(stream, _value);
        return true;
    }
}