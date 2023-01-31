#pragma once 
#include <JEngine/Assets/IAsset.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Collections/ObjectRef.h>
#include <JEngine/Assets/Graphics/Material/MaterialProperty.h>
#include <JEngine/Assets/Graphics/Texture/Texture2D.h>
#include <JEngine/Assets/Graphics/Shader/Shader.h>
#include <JEngine/Rendering/FrameBuffer.h>

namespace JEngine {
    class Material : public IAsset {
    public:

        Material();
        Material(const ObjectRef<Shader>& shader);
        Material(const ObjectRef<Texture2D>& texture, const ObjectRef<Shader>& shader);

        void addProperty(const MaterialProperty& prop);
        void removeProperty(const std::string& name);

        const bool operator <(const Material& other) const;
        const bool operator >(const Material& other) const;

        const bool operator ==(const Material& other) const;
        const bool operator !=(const Material& other) const;

        const bool trySetProperty(const MaterialProperty& prop);
        const bool tryGetProperty(const std::string& name, MaterialProperty& prop) const;

        const std::vector<MaterialProperty>& getProperties() const;

        void setTexture(const ObjectRef<Texture2D>& tex);
        void setFrameBuffer(const ObjectRef<FrameBuffer>& fBuffer);

        const bool serializeJson(json& jsonF) const override;
        const bool deserializeJson(json& jsonF) override;

        const bool serializeBinary(std::ostream& stream) const override;
        const bool deserializeBinary(std::istream& stream, const size_t size) override;

        const Shader* getShaderPtr() const;
        Shader* getShaderPtr();

        const ObjectRef<Shader>& getShader() const;
        ObjectRef<Shader>& getShader();

        const bool bind();
        void unbind() const;

    protected:
        virtual const bool jsonToBinaryImpl(json& jsonF, std::ostream& stream) const override;

    private:
        ObjectRef<Shader> _shader;
        ObjectRef<Texture2D> _mainTex;
        ObjectRef<FrameBuffer> _frameBuffer;

        std::vector<MaterialProperty> _properties;

        const int32_t indexOfProperty(const std::string& name) const;
        void applyProperties(Shader& shader) const;
    };
}
REGISTER_ASSET(JEngine::Material);