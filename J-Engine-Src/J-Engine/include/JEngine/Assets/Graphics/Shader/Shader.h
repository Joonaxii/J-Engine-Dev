#pragma once
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <string>
#include <unordered_map>

namespace JEngine {
    struct ShaderSources {
        std::string vertexSource;
        std::string fragmentSource;
        uint16_t blendSrc;
        uint16_t blendDst;
        uint16_t blendFunc;
    };

    class Texture2D;
    struct JMatrix;
    class Shader : public IAsset, public ISerializable<Shader> {
    public:
        Shader();
        Shader(const std::string& fileath);
        ~Shader();

        void bind() const;
        void unbind() const;

        const uint32_t getNativeHandle() const;

        void setUniform1i(const std::string& name, const int32_t value);
        void setUniform1f(const std::string& name, const float value);

        void setUniformV2f(const std::string& name, const JVector2f& vector);
        void setUniformV2f(const std::string& name, const float v0, const float v1);

        void setUniformV2i(const std::string& name, const JVector2i& vector);
        void setUniformV2i(const std::string& name, const int32_t v0, const int32_t v1);

        void setUniformV3f(const std::string& name, const JVector3f& vector);
        void setUniformV3f(const std::string& name, const float v0, const float v1, const float v2);

        void setUniformV3i(const std::string& name, const JVector3i& vector);
        void setUniformV3i(const std::string& name, const int32_t v0, const int32_t v1, const int32_t v2);

        void setUniformV4f(const std::string& name, const JVector4f& vector);
        void setUniformV4f(const std::string& name, const float v0, const float v1, const float v2, const float v3);

        void setUniformV4i(const std::string& name, const JVector4i& vector);
        void setUniformV4i(const std::string& name, const int32_t v0, const int32_t v1, const int32_t v2, const int32_t v3);

        void setUniformMat4f(const std::string& name, const JMatrix& mat);

        const uint32_t setTextures(const std::string& name, const Texture2D* texture, const uint32_t position = 0);

        const bool serializeJson(json& jsonF) const override;
        const bool deserializeJson(json& jsonF) override;

        const bool serializeBinary(std::ostream& stream) const override;
        const bool deserializeBinary(std::istream& stream, const size_t size) override;

        const uint64_t getBlendUnion() const;

    private:
        static uint64_t CURRENT_BLENDING_MODE;

        uint16_t _blendingModes[4];
        uint32_t _shaderID;
        std::unordered_map<std::string, int32_t> _uniformCache;

        ShaderSources parseShader(const std::string& filePath);
        ShaderSources parseShader(std::istream& stream);
        const uint32_t compileShader(const uint32_t type, const char* shader);
        const uint32_t createShader(const char* vertShader, const char* fragShader);

        const int32_t getUniformLocation(const std::string& name);
    };
}

REGISTER_ASSET(JEngine::Shader, Shader);