#include <JEngine/Assets/Graphics/Shader/Shader.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <JEngine/IO/Helpers/IOUtils.h>
#include <JEngine/Utility/StringHelpers.h>
#include <JEngine/Utility/Span.h>
#include <fstream>
#include <sstream>

namespace JEngine {
    uint64_t Shader::CURRENT_BLENDING_MODE = 0;
    Shader::ShaderLUT Shader::SHADER_LUT{};

    struct BlendMode {
        const char* name;
        const uint16_t value;

        bool isEqual(ConstSpan<char> str) const {
            return Helpers::strIEquals(str, ConstSpan<char>(name));
        }
    };

    static const BlendMode BLEND_MODES[] {
         { "Zero",                      GL_ZERO},
         { "One",                       GL_ONE},
         { "SrcColor",                  GL_SRC_COLOR},
         { "OneMinusSrcColor",          GL_ONE_MINUS_SRC_COLOR},
         { "SrcAlpha",                  GL_SRC_ALPHA},
         { "OneMinusSrcAlpha",          GL_ONE_MINUS_SRC_ALPHA},
         { "DstAlpha",                  GL_DST_ALPHA},
         { "OneMinusDstAlpha",          GL_ONE_MINUS_DST_ALPHA},
         { "DstColor",                  GL_DST_COLOR},
         { "OneMinusDstColor",          GL_ONE_MINUS_DST_COLOR},
         { "ConstantColor",             GL_CONSTANT_COLOR},
         { "OneMinusConstantColor",     GL_ONE_MINUS_CONSTANT_COLOR},
         { "ConstantAlpha",             GL_CONSTANT_ALPHA},
         { "OneMinusConstantAlpha",     GL_ONE_MINUS_CONSTANT_ALPHA},
         { "SrcAlphaSaturate",          GL_SRC_ALPHA_SATURATE},

         { "Add",                       GL_FUNC_ADD},
         { "Subtract",                  GL_FUNC_SUBTRACT},
         { "ReverseSubtract",           GL_FUNC_REVERSE_SUBTRACT},
         { "Min",                       GL_MIN},
         { "Max",                       GL_MAX},
    };
    static constexpr size_t BLEND_COUNT = sizeof(BLEND_MODES) / sizeof(BLEND_MODES[0]);

    uint16_t strToGLBlendingMode(ConstSpan<char> str, const bool isSrc = true) {
        for (size_t i = 0; i < BLEND_COUNT; i++) {
            const auto& blend = BLEND_MODES[i];
            if (blend.isEqual(str)) { 
                return blend.value;
            }
        }
        return isSrc ? GL_SRC_ALPHA : GL_ONE_MINUS_SRC_ALPHA;
    }

    Shader::Shader() : IAsset(), _shaderID(0), _blendingModes {GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA, GL_ADD} { }
    Shader::Shader(std::string_view fileath) : IAsset(),  _shaderID(0), _blendingModes { GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA, GL_ADD } {
        //ShaderSources source = parseShader(fileath);

        //_blendingModes[0] = source.blendSrc;
        //_blendingModes[1] = source.blendDst;
        //_blendingModes[2] = source.blendFunc;
        //_blendingModes[3] = 0;
        //_shaderID = source.vertexSource.length() > 1 && source.fragmentSource.length() > 1 ?  createShader(source.vertexSource.c_str(), source.fragmentSource.c_str()) : 0;
    }
    Shader::~Shader() {
        if (_shaderID) {
            GLCall(glDeleteProgram(_shaderID));
        }
    }

    void Shader::bind() const {
        if (_shaderID) {
            const uint64_t blend = *reinterpret_cast<const uint64_t*>(_blendingModes);
            if (CURRENT_BLENDING_MODE != blend) {
                CURRENT_BLENDING_MODE = blend;
                GLCall(glBlendFunc(_blendingModes[0], _blendingModes[1]));
                GLCall(glBlendEquation(_blendingModes[2]));
            }
            GLCall(glUseProgram(_shaderID));
        }
    }
    void Shader::unbind() const { GLCall(glUseProgram(0)); }

    uint32_t Shader::getNativeHandle() const { return _shaderID; }

    uint32_t Shader::setTextures(std::string_view name, TAssetRef<Texture> texture, const uint32_t position) {
        if (!texture || !_shaderID) { return position; }

        uint32_t pos = position;
        int32_t uId = getUniformLocation(name);

        if (uId < 0) { return pos; }
        GLCall(glUniform1i(uId, pos++));

        char palette[256]{ 0 };
        sprintf_s(palette, "%.*s%s", int(name.length()), name.data(), "_Pal");

        switch (texture->getFormat()) {
            case TextureFormat::Indexed8:
            case TextureFormat::Indexed16:
                uId = getUniformLocation(palette);
                if (uId > -1) {
                    GLCall(glUniform1i(uId, pos++));
                }
                break;
        }
        return pos;
    }

    bool Shader::serializeJson(json& jsonF) const { return false; }
    bool Shader::deserializeJson(json& jsonF) { return false; }

    bool Shader::serializeBinary(const Stream& stream) const { return false; }
    bool Shader::deserializeBinary(const Stream& stream, const size_t size) {
        if (stream.isOpen()) {
            char* data = reinterpret_cast<char*>(_malloca(size));
            if (data) {
                stream.read(data, size, false);
                if (_shaderID) {
                    GLCall(glDeleteProgram(_shaderID));
                }

                // membuf buf(data, size);
                // std::istream iStrm(&buf);
                // auto shdr = parseShader(iStrm);
                // _shaderID = createShader(shdr.vertexSource.c_str(), shdr.fragmentSource.c_str());
                _freea(data);
            }
        }
        return true;
    }

    uint64_t Shader::getBlendUnion() const {
        return *reinterpret_cast<const uint64_t*>(_blendingModes);
    }

    bool Shader::tryFindShader(std::string_view name, TAssetRef<Shader>& shader) {
        UUID16 hash(name);

        auto find = SHADER_LUT.find(hash);
        if (find == SHADER_LUT.end()) { return false; }

        shader = find->second;
        return true;
    }

    TAssetRef<Shader> Shader::findShader(std::string_view name) {
        UUID16 hash(name);
        auto find = SHADER_LUT.find(hash);

        if (find == SHADER_LUT.end()) { return nullptr; }
        return find->second;
    }

    void Shader::addShaderToLUT(TAssetRef<Shader> shader) {
        if (shader) {
            UUID16 hash(shader->getName());
            SHADER_LUT.insert(std::make_pair(hash, TAssetRef<Shader>(shader)));
        }
    }

    bool Shader::removeShaderFromLUT(TAssetRef<Shader> shader) {
        if (shader) {
            UUID16 hash(shader->getName());
            auto find = SHADER_LUT.find(hash);

            if (find == SHADER_LUT.end()) { return false; }

            SHADER_LUT.erase(hash);
        }

        return false;
    }

    void Shader::setUniform1i(std::string_view name, int32_t value) {
        if (!_shaderID) { return; }
        GLCall(glUniform1i(getUniformLocation(name), value));
    }

    void Shader::setUniform1ui(std::string_view name, uint32_t value) {
        if (!_shaderID) { return; }
        GLCall(glUniform1ui(getUniformLocation(name), value));
    }

    void Shader::setUniform1f(std::string_view name, float value) {
        if (!_shaderID) { return; }
        GLCall(glUniform1f(getUniformLocation(name), value));
    }

    void Shader::setUniformV2f(std::string_view name, const JVector2f& vector) {
        setUniformV2f(name, vector.x, vector.y);
    }
    void Shader::setUniformV2f(std::string_view name, float v0, float v1) {
        if (!_shaderID) { return; }
        GLCall(glUniform2f(getUniformLocation(name), v0, v1));
    }

    void Shader::setUniformV2i(std::string_view name, const JVector2i& vector) {
        setUniformV2i(name, vector.x, vector.y);
    }
    void Shader::setUniformV2i(std::string_view name, int32_t v0, int32_t v1) {
        if (!_shaderID) { return; }
        GLCall(glUniform2i(getUniformLocation(name), v0, v1));
    }

    void Shader::setUniformV2ui(std::string_view name, const JVector2u& vector) {
        setUniformV2ui(name, vector.x, vector.y);
    }
    void Shader::setUniformV2ui(std::string_view name, uint32_t v0, uint32_t v1) {
        if (!_shaderID) { return; }
        GLCall(glUniform2ui(getUniformLocation(name), v0, v1));
    }

    void Shader::setUniformV3f(std::string_view name, const JVector3f& vector) {
        setUniformV3f(name, vector.x, vector.y, vector.z);
    }
    void Shader::setUniformV3f(std::string_view name, float v0, float v1, float v2) {
        if (!_shaderID) { return; }
        GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
    }

    void Shader::setUniformV3i(std::string_view name, const JVector3i& vector) {
        setUniformV3i(name, vector.x, vector.y, vector.z);
    }
    void Shader::setUniformV3i(std::string_view name, int32_t v0, int32_t v1, int32_t v2) {
        if (!_shaderID) { return; }
        GLCall(glUniform3i(getUniformLocation(name), v0, v1, v2));
    }

    void Shader::setUniformColor24(std::string_view name, JColor24 color) {
        setUniformV3ui(name, color.r, color.g, color.b);
    }
    void Shader::setUniformV3ui(std::string_view name, const JVector3u& vector) {
        setUniformV3ui(name, vector.x, vector.y, vector.z);
    }
    void Shader::setUniformV3ui(std::string_view name, uint32_t v0, uint32_t v1, uint32_t v2) {
        if (!_shaderID) { return; }
        GLCall(glUniform3ui(getUniformLocation(name), v0, v1, v2));
    }

    void Shader::setUniformColor(std::string_view name, const JColor& color) {
        setUniformV4f(name, color.r, color.g, color.b, color.a);
    }
    void Shader::setUniformV4f(std::string_view name, const JVector4f& vector) {
        setUniformV4f(name, vector.x, vector.y, vector.z, vector.w);
    }
    void Shader::setUniformV4f(std::string_view name, float v0, float v1, float v2, float v3) {
        if (!_shaderID) { return; }
        GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::setUniformV4i(std::string_view name, const JVector4i& vector) {
        setUniformV4i(name, vector.x, vector.y, vector.z, vector.w);
    }
    void Shader::setUniformV4i(std::string_view name, int32_t v0, int32_t v1, int32_t v2, int32_t v3) {
        if (!_shaderID) { return; }
        GLCall(glUniform4i(getUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::setUniformColor32(std::string_view name, JColor32 color) {
        setUniformV4ui(name, color.r, color.g, color.b, color.a);
    }
    void Shader::setUniformV4ui(std::string_view name, const JVector4u& vector) {
        setUniformV4ui(name, vector.x, vector.y, vector.z, vector.w);
    }
    void Shader::setUniformV4ui(std::string_view name, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
        if (!_shaderID) { return; }
        GLCall(glUniform4ui(getUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::setUniformMat4f(std::string_view name, const JMatrix4f& mat) {
        if (!_shaderID) { return; }
        GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0]));
    }

    int32_t Shader::getUniformLocation(std::string_view name) {
        if (!_shaderID) { return -1; }

        UUID16 hash(name);
        if (_uniformCache.find(hash) != _uniformCache.end()) { return _uniformCache[hash]; }

        char* temp = reinterpret_cast<char*>(_malloca(name.length() + 1));
        if (temp) {
            memcpy(temp, name.data(), name.length());
            temp[name.length()] = 0;

            GLCall(int32_t location = glGetUniformLocation(_shaderID, temp));
            if (location == -1) {
                JE_CORE_WARN("Shader", "Warning: Uniform", name, "doesn't exist");
            }
            _uniformCache[hash] = location;

            _freea(temp);
            return location;
        }
        return -1;
    }

    ShaderSources Shader::parseShader()  {
        enum class ShaderType {
            NONE = -1,
            VERTEX = 0,
            FRAGMENT = 1,
        };

        std::string line{};
        ShaderSources srcs{};

        ShaderType type = ShaderType::NONE;
        uint16_t blendSrc = GL_SRC_ALPHA;
        uint16_t blendDst = GL_ONE_MINUS_SRC_ALPHA;
        uint16_t blendFunc = GL_FUNC_ADD;

        const char* data = _shader.data();
        size_t strLen = _shader.size();
        while (Helpers::getLine(data, strLen, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos) {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos) {
                    type = ShaderType::FRAGMENT;
                }
                continue;
            }

            if (const auto pos = line.find("#blend") != std::string::npos) {
                Span<char> temp(line.data() + pos, line.length() - pos);
                temp = temp.trim();

                int64_t ind = temp.indexOf(' ');
                if (ind < 0) { continue; }

                temp = temp.slice(ind + 1);
                ind = temp.indexOf(' ');

                ConstSpan<char> cTemp(temp);
                if (ind > -1) {
                    blendSrc = strToGLBlendingMode(cTemp.slice(0, ind), true);

                    temp = temp.slice(ind + 1);
                    ind = temp.indexOf(' ');

                    cTemp = ConstSpan<char>(temp);

                    if (ind > -1) {
                        blendDst = strToGLBlendingMode(cTemp.slice(0, ind), false);
                        blendFunc = strToGLBlendingMode(cTemp.slice(ind + 1), false);
                    }
                    else {
                        blendDst = strToGLBlendingMode(cTemp, false);
                    }
                    continue;
                }

                blendSrc = strToGLBlendingMode(cTemp, true);
                continue;
            }
            if (type == ShaderType::NONE) { continue; }

            switch (type)
            {
                case ShaderType::VERTEX:
                    srcs.vertexSource.append(line.c_str());
                    srcs.vertexSource.append(1, '\n');
                    break;
                case ShaderType::FRAGMENT:
                    srcs.fragmentSource.append(line.c_str());
                    srcs.fragmentSource.append(1, '\n');
                    break;
            }
        }

        srcs.blendSrc = blendSrc;
        srcs.blendFunc = blendFunc;
        srcs.blendDst = blendDst;
        return srcs;
    }

    uint32_t Shader::compileShader(const uint32_t type, const char* shader) {
        uint32_t shaderId = glCreateShader(type);
        GLCall(glShaderSource(shaderId, 1, &shader, NULL));
        GLCall(glCompileShader(shaderId));

        int result;
        GLCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result));

        if (result == GL_FALSE) {
            int len;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);

            char* message = (char*)alloca(len * sizeof(char));
            GLCall(glGetShaderInfoLog(shaderId, len, &len, message));
            std::cout << "[JEngine - Shader] Failed to compile '" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "' shader!" << std::endl << message << std::endl << std::endl;
            GLCall(glDeleteShader(shaderId));
            return 0;
        }
        return shaderId;
    }

    uint32_t Shader::createShader(const char* vertShader, const char* fragShader) {
        uint32_t program = glCreateProgram();
        uint32_t vs = compileShader(GL_VERTEX_SHADER, vertShader);
        uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragShader);

        GLCall(glAttachShader(program, vs));
        GLCall(glAttachShader(program, fs));
        GLCall(glLinkProgram(program));
        GLCall(glValidateProgram(program));

        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));
        return program;
    }
}