#include <JEngine/Assets/Graphics/Material/MaterialProperty.h>
#include <JEngine/Utility/DataUtilities.h>

namespace JEngine {
    MaterialProperty::MaterialProperty() : _crc(), _name(""), _type(), _propData{} { }

    MaterialProperty::MaterialProperty(const std::string& name, const PropertyType type) :
        _crc(), _name(name), _type(type), _propData{} {
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const PropertyType type, const void* data) :
        _crc(), _name(name), _type(type), _propData{}  {
        if (data) {
            memcpy(_propData, data, getPropertySize(type));
        }
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const PropertyType type, const void* data, const size_t size) :
        _crc(), _name(name), _type(type), _propData{} {
        if (data) {
            memcpy(_propData, data, size > sizeof(_propData) ? sizeof(_propData) : size);
        }
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const ObjectRef<Texture>& value) :
        _crc(0), _name(name), _type(T_Texture), _propData() {
        asTexture() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const uint32_t value) :
        _crc(0), _name(name), _type(T_UInt), _propData() {
        asUInt() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const float value) :
        _crc(0), _name(name), _type(T_Float), _propData() {
        asFloat() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector2f& value) :
        _crc(0), _name(name), _type(T_Vector2f), _propData() {
        asVector2f() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector2i& value) :
        _crc(0), _name(name), _type(T_Vector2i), _propData() {
        asVector2i() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector3f& value) :
        _crc(0), _name(name), _type(T_Vector2f), _propData() {
        asVector3f() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector3i& value) :
        _crc(0), _name(name), _type(T_Vector2i), _propData() {
        asVector3i() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector4f& value) :
        _crc(0), _name(name), _type(T_Vector4f), _propData() {
        asVector4f() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector4i& value) :
        _crc(0), _name(name), _type(T_Vector4i), _propData() {
        asVector4i() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JColor32& value) :
        _crc(0), _name(name), _type(T_Color32), _propData() {
        asColor32() = value;
        applyChanges();
    }


    MaterialProperty::MaterialProperty(const std::string& name, const JColor24& value) :
        _crc(0), _name(name), _type(T_Color24), _propData() {
        asColor24() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JColor& value) :
        _crc(0), _name(name), _type(T_Color), _propData() {
        asColor() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JMatrix4f& value) :
        _crc(0), _name(name), _type(T_Matrix4f), _propData() {
        asMatrix4f() = value;
        applyChanges();
    }

    void MaterialProperty::setType(const PropertyType type) {
        _type = type;
    }
    MaterialProperty::PropertyType MaterialProperty::getType() const {
        return _type;
    }

    bool MaterialProperty::operator==(const MaterialProperty& other) const {
        if (_crc != other._crc || _type != other._type || _name != other._name) { return false; }
        return memcmp(_propData, other._propData, getPropertySize(_type)) == 0;
    }

    bool MaterialProperty::operator!=(const MaterialProperty& other) const {
        return !(*this == other);
    }

    const std::string& MaterialProperty::getName() const {
        return _name;
    }

    ObjectRef<Texture>& MaterialProperty::asTexture() { return reinterpret_cast<ObjectRef<Texture>&>(_propData); }
    uint32_t& MaterialProperty::asUInt() { return reinterpret_cast<uint32_t&>(_propData); }
    float& MaterialProperty::asFloat() { return reinterpret_cast<float&>(_propData); }

    JVector2f& MaterialProperty::asVector2f() { return reinterpret_cast<JVector2f&>(_propData); }
    JVector2i& MaterialProperty::asVector2i() { return reinterpret_cast<JVector2i&>(_propData); }

    JVector3f& MaterialProperty::asVector3f() { return reinterpret_cast<JVector3f&>(_propData); }
    JVector3i& MaterialProperty::asVector3i() { return reinterpret_cast<JVector3i&>(_propData); }

    JVector4f& MaterialProperty::asVector4f() { return reinterpret_cast<JVector4f&>(_propData); }
    JVector4i& MaterialProperty::asVector4i() { return reinterpret_cast<JVector4i&>(_propData); }

    JColor32& MaterialProperty::asColor32() { return reinterpret_cast<JColor32&>(_propData); }
    JColor24& MaterialProperty::asColor24() { return reinterpret_cast<JColor24&>(_propData); }
    JColor& MaterialProperty::asColor() { return reinterpret_cast<JColor&>(_propData); }

    JMatrix4f& MaterialProperty::asMatrix4f() { return reinterpret_cast<JMatrix4f&>(_propData); }

    const ObjectRef<Texture>& MaterialProperty::asTexture() const { return reinterpret_cast<const ObjectRef<Texture>&>(_propData); }
    const uint32_t& MaterialProperty::asUInt() const { return reinterpret_cast<const uint32_t&>(_propData); }
    const float& MaterialProperty::asFloat() const { return reinterpret_cast<const float&>(_propData); }

    const JVector2f& MaterialProperty::asVector2f() const { return reinterpret_cast<const JVector2f&>(_propData); }
    const JVector2i& MaterialProperty::asVector2i() const { return reinterpret_cast<const JVector2i&>(_propData); }

    const JVector3f& MaterialProperty::asVector3f() const { return reinterpret_cast<const JVector3f&>(_propData); }
    const JVector3i& MaterialProperty::asVector3i() const { return reinterpret_cast<const JVector3i&>(_propData); }

    const JVector4f& MaterialProperty::asVector4f() const { return reinterpret_cast<const JVector4f&>(_propData); }
    const JVector4i& MaterialProperty::asVector4i() const { return reinterpret_cast<const JVector4i&>(_propData); }

    const JColor32& MaterialProperty::asColor32() const { return reinterpret_cast<const JColor32&>(_propData); }
    const JColor24& MaterialProperty::asColor24() const { return reinterpret_cast<const JColor24&>(_propData); }
    const JColor& MaterialProperty::asColor()     const { return reinterpret_cast<const JColor&>(_propData); }

    const JMatrix4f& MaterialProperty::asMatrix4f() const { return reinterpret_cast<const JMatrix4f&>(_propData); }

    void MaterialProperty::applyChanges() {
        _crc = 0xFFFFFFFFU;
        _crc = Data::updateCRC(_crc, _name.c_str(), _name.size());
        _crc = Data::updateCRC(_crc, &_type, sizeof(_type));
        _crc = Data::updateCRC(_crc, &_propData, sizeof(_propData));
        _crc ^= 0xFFFFFFFFU;
    }
}