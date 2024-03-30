#pragma once
#include <JEngine/Core.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>
#include <JEngine/Collections/ObjectRef.h>

namespace JEngine {
    class MaterialProperty {
    public:
        enum PropertyType : uint8_t {
            T_UInt,
            T_Float,
            T_Texture,

            T_Vector2f,
            T_Vector2i,

            T_Vector3f,
            T_Vector3i,

            T_Vector4f,
            T_Vector4i,

            T_Color24,
            T_Color32,
            T_Color,

            T_Matrix4f,


            _T_COUNT,
        };

        MaterialProperty();

        MaterialProperty(const std::string& name, const PropertyType type);
        MaterialProperty(const std::string& name, const PropertyType type, const void* data);
        MaterialProperty(const std::string& name, const PropertyType type, const void* data, const size_t size);

        MaterialProperty(const std::string& name, const ObjectRef<Texture>& value);
        MaterialProperty(const std::string& name, const uint32_t value);
        MaterialProperty(const std::string& name, const float value);

        MaterialProperty(const std::string& name, const JVector2f& value);
        MaterialProperty(const std::string& name, const JVector2i& value);

        MaterialProperty(const std::string& name, const JVector3f& value);
        MaterialProperty(const std::string& name, const JVector3i& value);

        MaterialProperty(const std::string& name, const JVector4f& value);
        MaterialProperty(const std::string& name, const JVector4i& value);

        MaterialProperty(const std::string& name, const JColor32& value);
        MaterialProperty(const std::string& name, const JColor24& value);
        MaterialProperty(const std::string& name, const JColor& value);

        MaterialProperty(const std::string& name, const JMatrix4f& value);

        static inline constexpr uint8_t getPropertySize(const PropertyType type) {
            return PROP_SIZES[type];
        }

        void setType(const PropertyType type);
        PropertyType getType() const;

        bool operator==(const MaterialProperty& other) const;
        bool operator!=(const MaterialProperty& other) const;

        const std::string& getName() const;

        uint32_t& asUInt();
        float& asFloat();

        JVector2f& asVector2f();
        JVector2i& asVector2i();

        JVector3f& asVector3f();
        JVector3i& asVector3i();

        JVector4f& asVector4f();
        JVector4i& asVector4i();

        JColor32& asColor32();
        JColor24& asColor24();
        JColor& asColor();

        JMatrix4f& asMatrix4f();

        ObjectRef<Texture>& asTexture();
        const ObjectRef<Texture>& asTexture()const;

        const uint32_t& asUInt() const;
        const float& asFloat() const;

        const JVector2f& asVector2f() const;
        const JVector2i& asVector2i() const;

        const JVector3f& asVector3f() const;
        const JVector3i& asVector3i() const;

        const JVector4f& asVector4f() const;
        const JVector4i& asVector4i() const;

        const JColor32& asColor32() const;
        const JColor24& asColor24() const;
        const JColor& asColor() const;

        const JMatrix4f& asMatrix4f() const;

        void applyChanges();

        bool deserializeJson(json& jsonF);
        bool serializeJson(json& jsonF) const;

        bool deserializeBinary(std::istream& stream);
        bool serializeBinary(std::ostream& stream) const;

    private:
        static inline const uint8_t PROP_SIZES[_T_COUNT]
        {
            sizeof(uint32_t),
            sizeof(float),
            sizeof(ObjectRef<Texture>),

            sizeof(JVector2f),
            sizeof(JVector2i),

            sizeof(JVector3f),
            sizeof(JVector3i),

            sizeof(JVector4f),
            sizeof(JVector4i),

            sizeof(JColor32),
            sizeof(JColor24),
            sizeof(JColor),

            sizeof(JMatrix4f)
        };

        uint32_t _crc;

        friend struct Serializable<MaterialProperty>;
        friend struct YAML::convert<MaterialProperty>;
        std::string _name;
        PropertyType _type;
        uint8_t _propData[64];
    };
}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::MaterialProperty::PropertyType& itemRef) {
        yamlOut << YAML::Dec << uint16_t(itemRef);
        return yamlOut;
    }

    template<>
    struct convert<JEngine::MaterialProperty::PropertyType> {
        static Node encode(const JEngine::MaterialProperty::PropertyType& rhs) {
            Node node;
            node.push_back(uint16_t(rhs));
            return node;
        }

        static bool decode(const Node& node, JEngine::MaterialProperty::PropertyType& rhs) {
            rhs = JEngine::MaterialProperty::PropertyType(node.as<uint16_t>());
            return true;
        }
    };

    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::MaterialProperty& itemRef) {
        yamlOut << YAML::BeginMap << YAML::Block;
        yamlOut << YAML::Key << "name" << YAML::Value << itemRef.getName();
        yamlOut << YAML::Key << "type" << YAML::Value << itemRef.getType();
        yamlOut << YAML::Key << "value" << YAML::Value;

        switch (itemRef.getType())
        {
            case JEngine::MaterialProperty::T_UInt:
                yamlOut << itemRef.asUInt();
                break;
            case JEngine::MaterialProperty::T_Float:
                yamlOut << itemRef.asFloat();
                break;
            case JEngine::MaterialProperty::T_Texture: {
                const JEngine::Texture* tex = itemRef.asTexture().getPtr();
                yamlOut << (tex ? tex->getUUID() : JEngine::UUID8::Empty);
                break;
            }
            case JEngine::MaterialProperty::T_Vector2f:
                yamlOut << itemRef.asVector2f();
                break;
            case JEngine::MaterialProperty::T_Vector2i:
                yamlOut << itemRef.asVector2i();
                break;
            case JEngine::MaterialProperty::T_Vector3f:
                yamlOut << itemRef.asVector3f();
                break;
            case JEngine::MaterialProperty::T_Vector3i:
                yamlOut << itemRef.asVector3i();
                break;
            case JEngine::MaterialProperty::T_Vector4f:
                yamlOut << itemRef.asVector4f();
                break;
            case JEngine::MaterialProperty::T_Vector4i:
                yamlOut << itemRef.asVector4i();
                break;
            case JEngine::MaterialProperty::T_Color32:
                yamlOut << itemRef.asColor32();
                break;
            case JEngine::MaterialProperty::T_Color24:
                yamlOut << itemRef.asColor24();
                break;
            case JEngine::MaterialProperty::T_Color:
                //yamlOut << itemRef.asColor();
                break;
            case JEngine::MaterialProperty::T_Matrix4f:
                yamlOut << itemRef.asMatrix4f();
                break;
            default:
                yamlOut << YAML::Null;
                break;
        }
        yamlOut << YAML::EndMap;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::MaterialProperty> {
        static Node encode(const JEngine::MaterialProperty& rhs) {
            Node node;
            node["name"] = rhs._name;
            node["type"] = uint8_t(rhs._type);

            switch (rhs._type)
            {
                case JEngine::MaterialProperty::T_UInt:
                    node["value"] = rhs.asUInt();
                    break;
                case JEngine::MaterialProperty::T_Float:
                    node["value"] = rhs.asFloat();
                    break;
                case JEngine::MaterialProperty::T_Texture: {
                    const JEngine::Texture* tex = rhs.asTexture().getPtr();
                    node["value"] = (tex ? tex->getUUID() : JEngine::UUID8::Empty);
                    break;
                }
                case JEngine::MaterialProperty::T_Vector2f:
                    node["value"] = rhs.asVector2f();
                    break;
                case JEngine::MaterialProperty::T_Vector2i:
                    node["value"] = rhs.asVector2i();
                    break;
                case JEngine::MaterialProperty::T_Vector3f:
                    node["value"] = rhs.asVector3f();
                    break;
                case JEngine::MaterialProperty::T_Vector3i:
                    node["value"] = rhs.asVector3i();
                    break;
                case JEngine::MaterialProperty::T_Vector4f:
                    node["value"] = rhs.asVector4f();
                    break;
                case JEngine::MaterialProperty::T_Vector4i:
                    node["value"] = rhs.asVector4i();
                    break;
                case JEngine::MaterialProperty::T_Color32:
                    node["value"] = rhs.asColor32();
                    break;
                case JEngine::MaterialProperty::T_Color24:
                    node["value"] = rhs.asColor24();
                    break;
                case JEngine::MaterialProperty::T_Color:
                    //node["value"] = rhs.asColor();
                    break;
                case JEngine::MaterialProperty::T_Matrix4f:
                    node["value"] = rhs.asMatrix4f();
                    break;
                default:
                    node["value"] = YAML::Null;
                    break;


            }
            
            return node;
        }

        static bool decode(const Node& node, JEngine::MaterialProperty& rhs) {
            if (!node.IsSequence() || node.size() < 2) { return false; }
            rhs._name = node["name"].as<std::string>();
            rhs._type = JEngine::MaterialProperty::PropertyType(node["type"].as<uint8_t>());

            const Node& value = node["value"];

            if (value) {
                switch (rhs._type)
                {
                    case JEngine::MaterialProperty::T_UInt:
                        rhs.asUInt() = value.as<uint32_t>();
                        break;
                    case JEngine::MaterialProperty::T_Float:
                        rhs.asFloat() = value.as<float>();
                        break;
                    case JEngine::MaterialProperty::T_Texture: {
                        JEngine::UUID8 uuid = value.as<JEngine::UUID8>();
                        rhs.asTexture() = JEngine::ObjectRef<JEngine::Texture>();
                        break;
                    }
                    case JEngine::MaterialProperty::T_Vector2f:
                        rhs.asVector2f() = value.as<JVector2f>();
                        break;
                    case JEngine::MaterialProperty::T_Vector2i:
                        rhs.asVector2i() = value.as<JVector2i>();
                        break;
                    case JEngine::MaterialProperty::T_Vector3f:
                        rhs.asVector3f() = value.as<JVector3f>();
                        break;
                    case JEngine::MaterialProperty::T_Vector3i:
                        rhs.asVector3i() = value.as<JVector3i>();
                        break;
                    case JEngine::MaterialProperty::T_Vector4f:
                        rhs.asVector4f() = value.as<JVector4f>();
                        break;
                    case JEngine::MaterialProperty::T_Vector4i:
                        rhs.asVector4i() = value.as<JVector4i>();
                        break;
                    case JEngine::MaterialProperty::T_Color32:
                        rhs.asColor32() = value.as<JEngine::JColor32>();
                        break;
                    case JEngine::MaterialProperty::T_Color24:
                        rhs.asColor24() = value.as<JEngine::JColor24>();
                        break;
                    case JEngine::MaterialProperty::T_Color:
                        //rhs.asColor() = value.as<JEngine::JColor>();
                        break;
                    case JEngine::MaterialProperty::T_Matrix4f:
                        rhs.asMatrix4f() = value.as<JEngine::JMatrix4f>();
                        break;
                }
            }
            return true;
        }
    };
}


//JSON and Binary
namespace JEngine {
    template<>
    inline bool Serializable<MaterialProperty::PropertyType>::deserializeJson(MaterialProperty::PropertyType& itemRef, const json& jsonF, const MaterialProperty::PropertyType& defaultValue) {
        itemRef = jsonF.is_number() ? MaterialProperty::PropertyType(jsonF.get<uint8_t>()) : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<MaterialProperty::PropertyType>::serializeJson(const MaterialProperty::PropertyType& itemRef, json& jsonF) {
        jsonF = uint8_t(itemRef);
        return true;
    }

    template<>
    inline bool Serializable<MaterialProperty>::deserializeJson(MaterialProperty& itemRef, const json& jsonF, const MaterialProperty& defaultValue) {
        Serialization::deserialize(itemRef._name, jsonF["name"], defaultValue._name);
        Serialization::deserialize(itemRef._type, jsonF["type"], defaultValue._type);

        switch (itemRef._type)
        {
            case JEngine::MaterialProperty::T_UInt:
                Serialization::deserialize(itemRef.asUInt(), jsonF["value"], defaultValue.asUInt());
                break;
            case JEngine::MaterialProperty::T_Float:
                Serialization::deserialize(itemRef.asFloat(), jsonF["value"], defaultValue.asFloat());
                break;
            case JEngine::MaterialProperty::T_Texture: {
                UUID8 uuid;
                Serialization::deserialize(uuid, jsonF["value"], UUID8::Empty);

                //TODO: Add texture finding by UUID
                itemRef.asTexture() = ObjectRef<Texture>(nullptr);
                break;
            }
            case JEngine::MaterialProperty::T_Vector2f:
                Serialization::deserialize(itemRef.asVector2f(), jsonF["value"], defaultValue.asVector2f());
                break;
            case JEngine::MaterialProperty::T_Vector2i:
                Serialization::deserialize(itemRef.asVector2i(), jsonF["value"], defaultValue.asVector2i());
                break;
            case JEngine::MaterialProperty::T_Vector3f:
                Serialization::deserialize(itemRef.asVector3f(), jsonF["value"], defaultValue.asVector3f());
                break;
            case JEngine::MaterialProperty::T_Vector3i:
                Serialization::deserialize(itemRef.asVector3i(), jsonF["value"], defaultValue.asVector3i());
                break;
            case JEngine::MaterialProperty::T_Vector4f:
                Serialization::deserialize(itemRef.asVector4f(), jsonF["value"], defaultValue.asVector4f());
                break;
            case JEngine::MaterialProperty::T_Vector4i:
                Serialization::deserialize(itemRef.asVector4i(), jsonF["value"], defaultValue.asVector4i());
                break;
            case JEngine::MaterialProperty::T_Color32:
                Serialization::deserialize(itemRef.asColor32(), jsonF["value"], defaultValue.asColor32());
                break;
            case JEngine::MaterialProperty::T_Color24:
                Serialization::deserialize(itemRef.asColor24(), jsonF["value"], defaultValue.asColor24());
                break;
            case JEngine::MaterialProperty::T_Color:
                //Serialization::deserialize(itemRef.asColor(), jsonF["value"], defaultValue.asColor());
                break;
            case JEngine::MaterialProperty::T_Matrix4f:
                Serialization::deserialize(itemRef.asMatrix4f(), jsonF["value"], defaultValue.asMatrix4f());
                break;
            default: break;
        }
        itemRef.applyChanges();
        return true;
    }

    template<>
    inline bool Serializable<MaterialProperty>::serializeJson(const MaterialProperty& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._name, jsonF["name"]);
        Serialization::serialize(itemRef._type, jsonF["type"]);

        switch (itemRef._type)
        {
            case JEngine::MaterialProperty::T_UInt:
                Serialization::serialize(itemRef.asUInt(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Float:
                Serialization::serialize(itemRef.asFloat(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Texture: {
                const Texture* tex = itemRef.asTexture().getPtr();
                const UUID8 uuid = tex ? tex->getUUID() : UUID8::Empty;
                Serialization::serialize(uuid, jsonF["value"]);
                break;
            }
            case JEngine::MaterialProperty::T_Vector2f:
                Serialization::serialize(itemRef.asVector2f(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Vector2i:
                Serialization::serialize(itemRef.asVector2i(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Vector3f:
                Serialization::serialize(itemRef.asVector3f(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Vector3i:
                Serialization::serialize(itemRef.asVector3i(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Vector4f:
                Serialization::serialize(itemRef.asVector4f(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Vector4i:
                Serialization::serialize(itemRef.asVector4i(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Color32:
                Serialization::serialize(itemRef.asColor32(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Color24:
                Serialization::serialize(itemRef.asColor24(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Color:
                //Serialization::serialize(itemRef.asColor(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::T_Matrix4f:
                Serialization::serialize(itemRef.asMatrix4f(), jsonF["value"]);
                break;
            default: break;
        }
        return true;
    }

    template<>
    inline bool Serializable<MaterialProperty>::deserializeBinary(MaterialProperty& itemRef, const Stream& stream) {
        Serialization::deserialize(itemRef._name, stream);
        Serialization::deserialize(itemRef._type, stream);

        switch (itemRef._type)
        {
            case JEngine::MaterialProperty::T_UInt:
                Serialization::deserialize(itemRef.asUInt(), stream);
                break;
            case JEngine::MaterialProperty::T_Float:
                Serialization::deserialize(itemRef.asFloat(), stream);
                break;
            case JEngine::MaterialProperty::T_Texture: {
                UUID8 uuid;
                Serialization::deserialize(uuid, stream);

                //TODO: Add texture finding by UUID
                itemRef.asTexture() = ObjectRef<Texture>(nullptr);
                break;
            }
            case JEngine::MaterialProperty::T_Vector2f:
                Serialization::deserialize(itemRef.asVector2f(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector2i:
                Serialization::deserialize(itemRef.asVector2i(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector3f:
                Serialization::deserialize(itemRef.asVector3f(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector3i:
                Serialization::deserialize(itemRef.asVector3i(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector4f:
                Serialization::deserialize(itemRef.asVector4f(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector4i:
                Serialization::deserialize(itemRef.asVector4i(), stream);
                break;
            case JEngine::MaterialProperty::T_Color32:
                Serialization::deserialize(itemRef.asColor32(), stream);
                break;
            case JEngine::MaterialProperty::T_Color24:
                Serialization::deserialize(itemRef.asColor24(), stream);
                break;
            case JEngine::MaterialProperty::T_Color:
                Serialization::deserialize(itemRef.asColor(), stream);
                break;
            case JEngine::MaterialProperty::T_Matrix4f:
                Serialization::deserialize(itemRef.asMatrix4f(), stream);
                break;
            default: break;
        }
        itemRef.applyChanges();
        return true;
    }

    template<>
    inline bool Serializable<MaterialProperty>::serializeBinary(const MaterialProperty& itemRef, const Stream& stream) {
        Serialization::serialize(itemRef._name, stream);
        Serialization::serialize(itemRef._type, stream);

        switch (itemRef._type)
        {
            case JEngine::MaterialProperty::T_UInt:
                Serialization::serialize(itemRef.asUInt(), stream);
                break;
            case JEngine::MaterialProperty::T_Float:
                Serialization::serialize(itemRef.asFloat(), stream);
                break;
            case JEngine::MaterialProperty::T_Texture: {
                const auto tex = itemRef.asTexture().getPtr();
                const UUID8 uuid = tex ? tex->getUUID() : UUID8::Empty;
                Serialization::serialize(uuid, stream);
                break;
            }
            case JEngine::MaterialProperty::T_Vector2f:
                Serialization::serialize(itemRef.asVector2f(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector2i:
                Serialization::serialize(itemRef.asVector2i(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector3f:
                Serialization::serialize(itemRef.asVector3f(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector3i:
                Serialization::serialize(itemRef.asVector3i(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector4f:
                Serialization::serialize(itemRef.asVector4f(), stream);
                break;
            case JEngine::MaterialProperty::T_Vector4i:
                Serialization::serialize(itemRef.asVector4i(), stream);
                break;
            case JEngine::MaterialProperty::T_Color32:
                Serialization::serialize(itemRef.asColor32(), stream);
                break;
            case JEngine::MaterialProperty::T_Color24:
                Serialization::serialize(itemRef.asColor24(), stream);
                break;
            case JEngine::MaterialProperty::T_Color:
                Serialization::serialize(itemRef.asColor(), stream);
                break;
            case JEngine::MaterialProperty::T_Matrix4f:
                Serialization::serialize(itemRef.asMatrix4f(), stream);
                break;
            default: break;
        }
        return true;
    }
}
#pragma endregion