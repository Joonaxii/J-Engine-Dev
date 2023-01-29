#include <JEngine/Assets/Graphics/Material/MaterialProperty.h>
#include <JEngine/Cryptography/CRC32.h>
#include <JEngine/IO/Serialization/Serialization.h>

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

    MaterialProperty::MaterialProperty(const std::string& name, const ObjectRef<Texture2D>& value) :
        _crc(0), _name(name), _type(Texture), _propData() {
        asTexture2D() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const uint32_t value) :
        _crc(0), _name(name), _type(UInt), _propData() {
        asUInt() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const float value) :
        _crc(0), _name(name), _type(Float), _propData() {
        asFloat() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector2f& value) :
        _crc(0), _name(name), _type(Vector2f), _propData() {
        asVector2f() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector2i& value) :
        _crc(0), _name(name), _type(Vector2i), _propData() {
        asVector2i() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector3f& value) :
        _crc(0), _name(name), _type(Vector2f), _propData() {
        asVector3f() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector3i& value) :
        _crc(0), _name(name), _type(Vector2i), _propData() {
        asVector3i() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector4f& value) :
        _crc(0), _name(name), _type(Vector4f), _propData() {
        asVector4f() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JVector4i& value) :
        _crc(0), _name(name), _type(Vector4i), _propData() {
        asVector4i() = value;
        applyChanges();
    }

    MaterialProperty::MaterialProperty(const std::string& name, const JMatrix& value) :
        _crc(0), _name(name), _type(Matrix4f), _propData() {
        asMatrix4f() = value;
        applyChanges();
    }

    void MaterialProperty::setType(const PropertyType type) {
        _type = type;
    }
    const MaterialProperty::PropertyType MaterialProperty::getType() const {
        return _type;
    }

    const bool MaterialProperty::operator==(const MaterialProperty& other) const {
        if (_crc != other._crc || _type != other._type || _name != other._name) { return false; }
        return memcmp(_propData, other._propData, getPropertySize(_type)) == 0;
    }

    const bool MaterialProperty::operator!=(const MaterialProperty& other) const {
        return !(*this == other);
    }

    const std::string& MaterialProperty::getName() const {
        return _name;
    }

    ObjectRef<Texture2D>& MaterialProperty::asTexture2D() { return reinterpret_cast<ObjectRef<Texture2D>&>(_propData); }
    uint32_t& MaterialProperty::asUInt() { return reinterpret_cast<uint32_t&>(_propData); }
    float& MaterialProperty::asFloat() { return reinterpret_cast<float&>(_propData); }

    JVector2f& MaterialProperty::asVector2f() { return reinterpret_cast<JVector2f&>(_propData); }
    JVector2i& MaterialProperty::asVector2i() { return reinterpret_cast<JVector2i&>(_propData); }

    JVector3f& MaterialProperty::asVector3f() { return reinterpret_cast<JVector3f&>(_propData); }
    JVector3i& MaterialProperty::asVector3i() { return reinterpret_cast<JVector3i&>(_propData); }

    JVector4f& MaterialProperty::asVector4f() { return reinterpret_cast<JVector4f&>(_propData); }
    JVector4i& MaterialProperty::asVector4i() { return reinterpret_cast<JVector4i&>(_propData); }

    JMatrix& MaterialProperty::asMatrix4f() { return reinterpret_cast<JMatrix&>(_propData); }

    const ObjectRef<Texture2D>& MaterialProperty::asTexture2D() const { return reinterpret_cast<const ObjectRef<Texture2D>&>(_propData); }
    const uint32_t& MaterialProperty::asUInt() const { return reinterpret_cast<const uint32_t&>(_propData); }
    const float& MaterialProperty::asFloat() const { return reinterpret_cast<const float&>(_propData); }

    const JVector2f& MaterialProperty::asVector2f() const { return reinterpret_cast<const JVector2f&>(_propData); }
    const JVector2i& MaterialProperty::asVector2i() const { return reinterpret_cast<const JVector2i&>(_propData); }

    const JVector3f& MaterialProperty::asVector3f() const { return reinterpret_cast<const JVector3f&>(_propData); }
    const JVector3i& MaterialProperty::asVector3i() const { return reinterpret_cast<const JVector3i&>(_propData); }

    const JVector4f& MaterialProperty::asVector4f() const { return reinterpret_cast<const JVector4f&>(_propData); }
    const JVector4i& MaterialProperty::asVector4i() const { return reinterpret_cast<const JVector4i&>(_propData); }

    const JMatrix& MaterialProperty::asMatrix4f() const { return reinterpret_cast<const JMatrix&>(_propData); }

    void MaterialProperty::applyChanges() {
        _crc = CRC32::compute(_name.c_str(), _name.size());
        _crc = CRC32::update(_crc, &_type, sizeof(_propData) + sizeof(_type));
    }

    const bool MaterialProperty::deserializeJson(json& jsonF) {
        _name = jsonF.value("name", "");
        _type = MaterialProperty::PropertyType(jsonF.value("type", 0));
        switch (_type)
        {
            case JEngine::MaterialProperty::UInt:
                asUInt() = jsonF.value("value", 0);
                break;
            case JEngine::MaterialProperty::Float:
                asFloat() = jsonF.value("value", 0.0f);
                break;
            case JEngine::MaterialProperty::Texture: {
                UUID uuid;
                UUIDFactory::parseUUID(jsonF.value("value", ""), uuid);
                //TODO: Add texture finding by UUID
                asTexture2D() = ObjectRef<Texture2D>(nullptr);
                break;
            }
            case JEngine::MaterialProperty::Vector2f:
                asVector2f().deserializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector2i:
                asVector2i().deserializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector3f:
                asVector3f().deserializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector3i:
                asVector3i().deserializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector4f:
                asVector4f().deserializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector4i:
                asVector4i().deserializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Matrix4f:
                asMatrix4f().deserializeJson(jsonF["value"]);
                break;
            default: break;
        }
        applyChanges();
        return true;
    }

    const bool MaterialProperty::serializeJson(json& jsonF) const {
        jsonF["name"] = _name;
        jsonF["type"] = uint16_t(_type);
        
        switch (_type)
        {
            case JEngine::MaterialProperty::UInt:
                jsonF["value"] = asUInt();
                break;
            case JEngine::MaterialProperty::Float:
                jsonF["value"] = asFloat();
                break;
            case JEngine::MaterialProperty::Texture: {
                auto ptr = asTexture2D().getPtr();
                const UUID uuid = ptr ? ptr->getUUID() : UUIDFactory::UUID_EMPTY;
                jsonF["value"] = UUIDFactory::getUUIDStr(uuid);
                break;
            }
            case JEngine::MaterialProperty::Vector2f:
                asVector2f().serializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector2i:
                asVector2i().serializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector3f:
                asVector3f().serializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector3i:
                asVector3i().serializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector4f:
                asVector4f().serializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector4i:
                asVector4i().serializeJson(jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Matrix4f:
                asMatrix4f().serializeJson(jsonF["value"]);
                break;
            default: break;
        }
        return true;
    }

    const bool MaterialProperty::deserializeBinary(std::istream& stream) {
        _name = Serialization::deserializeStringBinary(stream);
        Serialization::deserializeBinary(stream, _type);

        switch (_type)
        {
            case JEngine::MaterialProperty::Texture: {
                UUID uuid;
                Serialization::deserializeBinary(stream, uuid);

                //TODO: Search texture by UUID
                break;
            }
            default:
                stream.read(reinterpret_cast<char*>(_propData), getPropertySize(_type));
                break;
        }
        applyChanges();
        return true;
    }

    const bool MaterialProperty::serializeBinary(std::ostream& stream) const {
        Serialization::serializeStringBinary(stream, _name);
        Serialization::serializeBinary(stream, _type);

        switch (_type)
        {
            case JEngine::MaterialProperty::Texture: {
                auto ptr = asTexture2D().getPtr();
                const UUID uuid = ptr ? ptr->getUUID() : UUIDFactory::UUID_EMPTY;
                Serialization::serializeBinary(stream, uuid);
                break;
            }
            default:
                stream.write(reinterpret_cast<const char*>(_propData), getPropertySize(_type));
                break;
        }
        return true;
    }
}