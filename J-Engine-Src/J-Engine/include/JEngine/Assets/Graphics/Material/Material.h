#pragma once 
#include <JEngine/Assets/IAsset.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Collections/ObjectRef.h>
#include <JEngine/Assets/Graphics/Material/MaterialProperty.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>
#include <JEngine/Assets/Graphics/Shader/Shader.h>
#include <JEngine/Rendering/FrameBuffer.h>

namespace JEngine {
    class Material : public IAsset {
    public:

        Material();
        Material(const ObjectRef<Shader>& shader);
        Material(const ObjectRef<Texture>& texture, const ObjectRef<Shader>& shader);

        void addProperty(const MaterialProperty& prop);
        void removeProperty(const std::string& name);

        bool operator <(const Material& other) const;
        bool operator >(const Material& other) const;

        bool operator ==(const Material& other) const;
        bool operator !=(const Material& other) const;

        bool trySetProperty(const MaterialProperty& prop);
        bool tryGetProperty(const std::string& name, MaterialProperty& prop) const;

        const std::vector<MaterialProperty>& getProperties() const;

        void setMainTexture(const ObjectRef<Texture>& tex);
        void setFrameBuffer(const ObjectRef<FrameBuffer>& fBuffer);

        bool serializeJson(json& jsonF) const;
        bool deserializeJson(json& jsonF);

        bool deserializeYaml(const yamlNode& yamlIn);
        bool serializeYaml(yamlEmit& yamlOut) const;

        bool serializeBinary(const Stream& stream) const;
        bool deserializeBinary(const Stream& stream, const size_t size);

        const Shader* getShaderPtr() const;
        Shader* getShaderPtr();

        const ObjectRef<Shader>& getShader() const;
        ObjectRef<Shader>& getShader();

        const Texture* getMainTexturePtr() const;
        Texture* getMainTexturePtr();

        const ObjectRef<Texture>& getMainTexture() const;
        ObjectRef<Texture>& getMainTexture();

        bool bind();
        void unbind() const;
    private:
        ObjectRef<Shader> _shader;
        ObjectRef<Texture> _mainTex;
        ObjectRef<FrameBuffer> _frameBuffer;

        std::vector<MaterialProperty> _properties;

        int32_t indexOfProperty(const std::string& name) const;
        void applyProperties(Shader& shader) const;
    };
}
REGISTER_ASSET(JEngine::Material);