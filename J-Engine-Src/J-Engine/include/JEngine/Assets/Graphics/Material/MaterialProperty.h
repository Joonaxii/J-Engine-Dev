#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Assets/Graphics/Texture/Texture2D.h>
#include <JEngine/Collections/ObjectRef.h>

namespace JEngine {
    class MaterialProperty {
    public:
        enum PropertyType : uint8_t {
            UInt,
            Float,
            Texture,

            Vector2f,
            Vector2i,

            Vector3f,
            Vector3i,

            Vector4f,
            Vector4i,

            Color24,
            Color32,
            Color,

            Matrix4f,
        };

        MaterialProperty();

        MaterialProperty(const std::string& name, const PropertyType type);
        MaterialProperty(const std::string& name, const PropertyType type, const void* data);
        MaterialProperty(const std::string& name, const PropertyType type, const void* data, const size_t size);

        MaterialProperty(const std::string& name, const ObjectRef<Texture2D>& value);
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
        const PropertyType getType() const;

        const bool operator==(const MaterialProperty& other) const;
        const bool operator!=(const MaterialProperty& other) const;

        const std::string& getName() const;

        ObjectRef<Texture2D>& asTexture2D();

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

        const ObjectRef<Texture2D>& asTexture2D()const;

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

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json& jsonF) const;

        const bool deserializeBinary(std::istream& stream);
        const bool serializeBinary(std::ostream& stream) const;

    private:
        static inline const uint8_t PROP_SIZES[static_cast<int32_t>(Matrix4f) + 1]
        {
            sizeof(uint32_t),
            sizeof(float),
            sizeof(ObjectRef<Texture2D>),

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
        std::string _name;
        PropertyType _type;
        uint8_t _propData[64];
    };

#pragma region Serialization

    template<>
    inline const bool Serializable<MaterialProperty::PropertyType>::deserializeJson(MaterialProperty::PropertyType& itemRef, json& jsonF, const MaterialProperty::PropertyType& defaultValue) {
        itemRef = jsonF.is_number() ? MaterialProperty::PropertyType(jsonF.get<uint8_t>()) : defaultValue;
        return true;
    }

    template<>
    inline const bool Serializable<MaterialProperty::PropertyType>::serializeJson(const MaterialProperty::PropertyType& itemRef, json& jsonF) {
        jsonF = uint8_t(itemRef);
        return true;
    }

    template<>
    inline const bool Serializable<MaterialProperty>::deserializeJson(MaterialProperty& itemRef, json& jsonF, const MaterialProperty& defaultValue) {
        Serialization::deserialize(itemRef._name, jsonF["name"], defaultValue._name);
        Serialization::deserialize(itemRef._type, jsonF["type"], defaultValue._type);

        switch (itemRef._type)
        {
            case JEngine::MaterialProperty::UInt:
                Serialization::deserialize(itemRef.asUInt(), jsonF["value"], defaultValue.asUInt());
                break;
            case JEngine::MaterialProperty::Float:
                Serialization::deserialize(itemRef.asFloat(), jsonF["value"], defaultValue.asFloat());
                break;
            case JEngine::MaterialProperty::Texture: {
                UUID uuid;
                Serialization::deserialize(uuid, jsonF["value"], UUIDFactory::Empty);

                //TODO: Add texture finding by UUID
                itemRef.asTexture2D() = ObjectRef<Texture2D>(nullptr);
                break;
            }
            case JEngine::MaterialProperty::Vector2f:
                Serialization::deserialize(itemRef.asVector2f(), jsonF["value"], defaultValue.asVector2f());
                break;
            case JEngine::MaterialProperty::Vector2i:
                Serialization::deserialize(itemRef.asVector2i(), jsonF["value"], defaultValue.asVector2i());
                break;
            case JEngine::MaterialProperty::Vector3f:
                Serialization::deserialize(itemRef.asVector3f(), jsonF["value"], defaultValue.asVector3f());
                break;
            case JEngine::MaterialProperty::Vector3i:
                Serialization::deserialize(itemRef.asVector3i(), jsonF["value"], defaultValue.asVector3i());
                break;
            case JEngine::MaterialProperty::Vector4f:
                Serialization::deserialize(itemRef.asVector4f(), jsonF["value"], defaultValue.asVector4f());
                break;
            case JEngine::MaterialProperty::Vector4i:
                Serialization::deserialize(itemRef.asVector4i(), jsonF["value"], defaultValue.asVector4i());
                break;
            case JEngine::MaterialProperty::Color32:
                Serialization::deserialize(itemRef.asColor32(), jsonF["value"], defaultValue.asColor32());
                break;
            case JEngine::MaterialProperty::Color24:
                Serialization::deserialize(itemRef.asColor24(), jsonF["value"], defaultValue.asColor24());
                break;
            case JEngine::MaterialProperty::Color:
                Serialization::deserialize(itemRef.asColor(), jsonF["value"], defaultValue.asColor());
                break;
            case JEngine::MaterialProperty::Matrix4f:
                Serialization::deserialize(itemRef.asMatrix4f(), jsonF["value"], defaultValue.asMatrix4f());
                break;
            default: break;
        }
        itemRef.applyChanges();
        return true;
    }

    template<>
    inline const bool Serializable<MaterialProperty>::serializeJson(const MaterialProperty& itemRef, json& jsonF) {
        Serialization::serialize(itemRef._name, jsonF["name"]);
        Serialization::serialize(itemRef._type, jsonF["type"]);

        switch (itemRef._type)
        {
            case JEngine::MaterialProperty::UInt:
                Serialization::serialize(itemRef.asUInt(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Float:
                Serialization::serialize(itemRef.asFloat(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Texture: {
                const auto tex = itemRef.asTexture2D().getPtr();
                const UUID uuid = tex ? tex->getUUID() : UUIDFactory::Empty;
                Serialization::serialize(uuid, jsonF["value"]);
                break;
            }
            case JEngine::MaterialProperty::Vector2f:
                Serialization::serialize(itemRef.asVector2f(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector2i:
                Serialization::serialize(itemRef.asVector2i(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector3f:
                Serialization::serialize(itemRef.asVector3f(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector3i:
                Serialization::serialize(itemRef.asVector3i(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector4f:
                Serialization::serialize(itemRef.asVector4f(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Vector4i:
                Serialization::serialize(itemRef.asVector4i(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Color32:
                Serialization::serialize(itemRef.asColor32(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Color24:
                Serialization::serialize(itemRef.asColor24(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Color:
                Serialization::serialize(itemRef.asColor(), jsonF["value"]);
                break;
            case JEngine::MaterialProperty::Matrix4f:
                Serialization::serialize(itemRef.asMatrix4f(), jsonF["value"]);
                break;
            default: break;
        }
        return true;
    }  
    
    template<>
    inline const bool Serializable<MaterialProperty>::deserializeBinary(MaterialProperty& itemRef, std::istream& stream) {
        Serialization::deserialize(itemRef._name, stream);
        Serialization::deserialize(itemRef._type, stream);

        switch (itemRef._type)
        {
            case JEngine::MaterialProperty::UInt:
                Serialization::deserialize(itemRef.asUInt(), stream);
                break;
            case JEngine::MaterialProperty::Float:
                Serialization::deserialize(itemRef.asFloat(), stream);
                break;
            case JEngine::MaterialProperty::Texture: {
                UUID uuid;
                Serialization::deserialize(uuid, stream);

                //TODO: Add texture finding by UUID
                itemRef.asTexture2D() = ObjectRef<Texture2D>(nullptr);
                break;
            }
            case JEngine::MaterialProperty::Vector2f:
                Serialization::deserialize(itemRef.asVector2f(), stream);
                break;
            case JEngine::MaterialProperty::Vector2i:
                Serialization::deserialize(itemRef.asVector2i(), stream);
                break;
            case JEngine::MaterialProperty::Vector3f:
                Serialization::deserialize(itemRef.asVector3f(), stream);
                break;
            case JEngine::MaterialProperty::Vector3i:
                Serialization::deserialize(itemRef.asVector3i(), stream);
                break;
            case JEngine::MaterialProperty::Vector4f:
                Serialization::deserialize(itemRef.asVector4f(), stream);
                break;
            case JEngine::MaterialProperty::Vector4i:
                Serialization::deserialize(itemRef.asVector4i(), stream);
                break;
            case JEngine::MaterialProperty::Color32:
                Serialization::deserialize(itemRef.asColor32(), stream);
                break;
            case JEngine::MaterialProperty::Color24:
                Serialization::deserialize(itemRef.asColor24(), stream);
                break;
            case JEngine::MaterialProperty::Color:
                Serialization::deserialize(itemRef.asColor(), stream);
                break;
            case JEngine::MaterialProperty::Matrix4f:
                Serialization::deserialize(itemRef.asMatrix4f(), stream);
                break;
            default: break;
        }
        itemRef.applyChanges();
        return true;
    }

    template<>
    inline const bool Serializable<MaterialProperty>::serializeBinary(const MaterialProperty& itemRef, std::ostream& stream) {
        Serialization::serialize(itemRef._name, stream);
        Serialization::serialize(itemRef._type, stream);

        switch (itemRef._type)
        {
            case JEngine::MaterialProperty::UInt:
                Serialization::serialize(itemRef.asUInt(), stream);
                break;
            case JEngine::MaterialProperty::Float:
                Serialization::serialize(itemRef.asFloat(), stream);
                break;
            case JEngine::MaterialProperty::Texture: {
                const auto tex = itemRef.asTexture2D().getPtr();
                const UUID uuid = tex ? tex->getUUID() : UUIDFactory::Empty;
                Serialization::serialize(uuid, stream);
                break;
            }
            case JEngine::MaterialProperty::Vector2f:
                Serialization::serialize(itemRef.asVector2f(), stream);
                break;
            case JEngine::MaterialProperty::Vector2i:
                Serialization::serialize(itemRef.asVector2i(), stream);
                break;
            case JEngine::MaterialProperty::Vector3f:
                Serialization::serialize(itemRef.asVector3f(), stream);
                break;
            case JEngine::MaterialProperty::Vector3i:
                Serialization::serialize(itemRef.asVector3i(), stream);
                break;
            case JEngine::MaterialProperty::Vector4f:
                Serialization::serialize(itemRef.asVector4f(), stream);
                break;
            case JEngine::MaterialProperty::Vector4i:
                Serialization::serialize(itemRef.asVector4i(), stream);
                break;
            case JEngine::MaterialProperty::Color32:
                Serialization::serialize(itemRef.asColor32(), stream);
                break;
            case JEngine::MaterialProperty::Color24:
                Serialization::serialize(itemRef.asColor24(), stream);
                break;
            case JEngine::MaterialProperty::Color:
                Serialization::serialize(itemRef.asColor(), stream);
                break;
            case JEngine::MaterialProperty::Matrix4f:
                Serialization::serialize(itemRef.asMatrix4f(), stream);
                break;
            default: break;
        }
        return true;
    }


#pragma endregion
}