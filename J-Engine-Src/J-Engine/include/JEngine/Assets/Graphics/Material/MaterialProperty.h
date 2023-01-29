#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Assets/Graphics/Texture/Texture2D.h>
#include <JEngine/Collections/ObjectRef.h>

namespace JEngine {
    class MaterialProperty : ISerializable<MaterialProperty> {
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

        MaterialProperty(const std::string& name, const JMatrix& value);

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

        JMatrix& asMatrix4f();

        const ObjectRef<Texture2D>& asTexture2D()const;

        const uint32_t& asUInt() const;
        const float& asFloat() const;

        const JVector2f& asVector2f() const;
        const JVector2i& asVector2i() const;

        const JVector3f& asVector3f() const;
        const JVector3i& asVector3i() const;

        const JVector4f& asVector4f() const;
        const JVector4i& asVector4i() const;

        const JMatrix& asMatrix4f() const;

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

            sizeof(JMatrix)
        };

        uint32_t _crc;

        std::string _name;
        PropertyType _type;
        uint8_t _propData[64];
    };
}