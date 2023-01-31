#include <JEngine/Core/LayerMask.h>
#include <JEngine/IO/Serialization/Serialization.h>

namespace JEngine {
    LayerMask::LayerMask() : _value(0) {}
    LayerMask::LayerMask(const uint32_t value) : _value(value) {}

    LayerMask::operator uint32_t() const { return _value; }
    LayerMask::operator size_t() const { return _value; }
    const LayerMask LayerMask::operator~() const { return LayerMask(~_value); }
    LayerMask::operator bool() const { return bool(_value); }
}