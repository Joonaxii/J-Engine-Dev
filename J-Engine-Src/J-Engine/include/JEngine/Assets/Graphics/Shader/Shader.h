#pragma once

#include <JEngine/Assets/IAsset.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Cryptography/UUID.h>
#include <JEngine/Core/Ref.h>
#include <string>
#include <string_view>
#include <iostream>
#include <unordered_map>

namespace JEngine {
    struct ShaderSources {
        std::string vertexSource;
        std::string fragmentSource;

        uint16_t blendSrc;
        uint16_t blendDst;
        uint16_t blendFunc;
    };

    class ShaderSerializer;
    class Texture;
    class Shader : public IAsset {
    public:
        typedef bool (*FindInclude)(std::string_view line, const std::ostream& stream);

        Shader();
        Shader(std::string_view fileath);
        ~Shader();

        void bind() const;
        void unbind() const;

        uint32_t getNativeHandle() const;

        void setUniform1i(std::string_view name, int32_t value);
        void setUniform1ui(std::string_view name, uint32_t value);
        void setUniform1f(std::string_view name, float value);

        void setUniformV2f(std::string_view name, const JVector2f& vector);
        void setUniformV2f(std::string_view name, float v0, float v1);

        void setUniformV2i(std::string_view name, const JVector2i& vector);
        void setUniformV2i(std::string_view name, int32_t v0, int32_t v1);

        void setUniformV2ui(std::string_view name, const JVector2u& vector);
        void setUniformV2ui(std::string_view name, uint32_t v0, uint32_t v1);

        void setUniformV3f(std::string_view name, const JVector3f& vector);
        void setUniformV3f(std::string_view name, float v0, float v1, float v2);

        void setUniformColor24(std::string_view name, JColor24 color);
        void setUniformV3i(std::string_view name, const JVector3i& vector);
        void setUniformV3i(std::string_view name, int32_t v0, int32_t v1, int32_t v2);

        void setUniformV3ui(std::string_view name, const JVector3u& vector);
        void setUniformV3ui(std::string_view name, uint32_t v0, uint32_t v1, uint32_t v2);

        void setUniformColor(std::string_view name, const JColor& color);
        void setUniformV4f(std::string_view name, const JVector4f& vector);
        void setUniformV4f(std::string_view name, float v0, float v1, float v2, float v3);

        void setUniformV4i(std::string_view name, const JVector4i& vector);
        void setUniformV4i(std::string_view name, int32_t v0, int32_t v1, int32_t v2, int32_t v3);

        void setUniformColor32(std::string_view name, JColor32 color);
        void setUniformV4ui(std::string_view name, const JVector4u& vector);
        void setUniformV4ui(std::string_view name, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);

        void setUniformMat4f(std::string_view name, const JMatrix4f& mat);

        uint32_t setTextures(std::string_view name, TAssetRef<Texture> texture, const uint32_t position = 0);

        bool serializeJson(json& jsonF) const;
        bool deserializeJson(json& jsonF);

        bool serializeBinary(const Stream& stream) const;
        bool deserializeBinary(const Stream& stream, const size_t size);

        uint64_t getBlendUnion() const;

        static bool tryFindShader(std::string_view name, TAssetRef<Shader>& shader);
        static TAssetRef<Shader> findShader(std::string_view name);

        static void addShaderToLUT(TAssetRef<Shader> shader);
        static bool removeShaderFromLUT(TAssetRef<Shader> shader);
    private:
        friend class ShaderSerializer;

        typedef std::unordered_map<UUID16, TAssetRef<Shader>, std::hash<UUID16>> ShaderLUT;

        static uint64_t CURRENT_BLENDING_MODE;
        static ShaderLUT SHADER_LUT;

        uint16_t _blendingModes[4];
        uint32_t _shaderID;
        std::vector<char> _shader{};
        std::unordered_map<UUID16, int32_t, std::hash<UUID16>> _uniformCache;

        ShaderSources parseShader();
        uint32_t compileShader(const uint32_t type, const char* shader);
        uint32_t createShader(const char* vertShader, const char* fragShader);

        int32_t getUniformLocation(std::string_view name);
    };
}