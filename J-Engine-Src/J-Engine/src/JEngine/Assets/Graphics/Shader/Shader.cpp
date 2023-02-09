#include <JEngine/Assets/Graphics/Shader/Shader.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>
#include <JEngine/Rendering/OpenGL/GLHelpers.h>
#include <JEngine/IO/Helpers/IOHelpers.h>
#include <JEngine/Helpers/StringExtensions.h>
#include <JEngine/Utility/Span.h>
#include <fstream>
#include <sstream>

namespace JEngine {
    uint64_t Shader::CURRENT_BLENDING_MODE = 0;
    Shader::ShaderLUT Shader::SHADER_LUT{};

    struct BlendMode {
        const char* name;
        const uint16_t value;

        bool isEqual(const Span<char>& str) const {
            return Helpers::equalsNoCase(ConstSpan<char>(str), ConstSpan<char>(name, strlen(name)));
        }

        bool isEqual(const ConstSpan<char>& str) const {
            return Helpers::equalsNoCase(str, ConstSpan<char>(name, strlen(name)));
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

    uint16_t strToGLBlendingMode(const Span<char>& str, const bool isSrc = true) {
        for (size_t i = 0; i < BLEND_COUNT; i++) {
            const auto& blend = BLEND_MODES[i];
            if (blend.isEqual(str)) { 
                return blend.value;
            }
        }
        return isSrc ? GL_SRC_ALPHA : GL_ONE_MINUS_SRC_ALPHA;
    }

    Shader::Shader() : _shaderID(0), _blendingModes {GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA} { }
    Shader::Shader(const std::string& fileath) : _shaderID(0), _blendingModes { GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA } {
        ShaderSources source = parseShader(fileath);

        _blendingModes[0] = source.blendSrc;
        _blendingModes[1] = source.blendDst;
        _blendingModes[2] = source.blendFunc;
        _blendingModes[3] = 0;
        _shaderID = source.vertexSource.length() > 1 && source.fragmentSource.length() > 1 ?  createShader(source.vertexSource.c_str(), source.fragmentSource.c_str()) : 0;
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

    uint32_t Shader::setTextures(const std::string& name, const Texture* texture, const uint32_t position) {
        if (!texture || !_shaderID) { return position; }

        uint32_t pos = position;
        int32_t uId = getUniformLocation(name);

        if (uId < 0) { return pos; }
        GLCall(glUniform1i(uId, pos++));
        if (texture->getTextureFormat() == TextureFormat::Indexed) {
            uId = getUniformLocation(name + "_Pal");
            if (uId > -1) {
                GLCall(glUniform1i(uId, pos++));
            }
        }
        return pos;
    }

    bool Shader::serializeJson(json& jsonF) const { return false; }
    bool Shader::deserializeJson(json& jsonF) { return false; }

    bool Shader::serializeBinary(std::ostream& stream) const { return false; }
    bool Shader::deserializeBinary(std::istream& stream, const size_t size) {
        if (stream.good()) {
            char* data = reinterpret_cast<char*>(malloc(size));
            stream.read(data, size);

            if (_shaderID) {
                GLCall(glDeleteProgram(_shaderID));
            }

            membuf buf(data, size);
            std::istream iStrm(&buf);
            auto shdr = parseShader(iStrm);
            _shaderID = createShader(shdr.vertexSource.c_str(), shdr.fragmentSource.c_str());

            free(data);
        }
        return true;
    }

    uint64_t Shader::getBlendUnion() const {
        return *reinterpret_cast<const uint64_t*>(_blendingModes);
    }

    bool Shader::tryFindShader(const char* name, ObjectRef<Shader>& shader) {
        FAH16 hash(name);

        auto& find = SHADER_LUT.find(hash);
        if (find == SHADER_LUT.end()) { return false; }

        shader = find->second;
        return true;
    }

    Shader* Shader::findShader(const char* name) {
        FAH16 hash(name);
        auto find = SHADER_LUT.find(hash);

        if (find == SHADER_LUT.end()) { return nullptr; }
        return find->second.getPtr();
    }

    void Shader::addShaderToLUT(const ObjectRef<Shader>& shader) {
        const Shader* shaderPtr = shader.getPtr();

        if (shaderPtr) {
            FAH16 hash(shaderPtr->getName().c_str());
            SHADER_LUT.insert(std::make_pair(hash, ObjectRef<Shader>(shader)));
        }
    }

    bool Shader::removeShaderFromLUT(const ObjectRef<Shader>& shader) {
        const Shader* shaderPtr = shader.getPtr();

        if (shaderPtr) {
            FAH16 hash(shaderPtr->getName().c_str());
            auto find = SHADER_LUT.find(hash);

            if (find == SHADER_LUT.end()) { return false; }

            SHADER_LUT.erase(hash);
        }

        return false;
    }

    void Shader::setUniform1i(const std::string& name, const int32_t value) {
        if (!_shaderID) { return; }
        GLCall(glUniform1i(getUniformLocation(name), value));
    }

    void Shader::setUniform1ui(const std::string& name, const uint32_t value) {
        if (!_shaderID) { return; }
        GLCall(glUniform1ui(getUniformLocation(name), value));
    }

    void Shader::setUniform1f(const std::string& name, const float value) {
        if (!_shaderID) { return; }
        GLCall(glUniform1f(getUniformLocation(name), value));
    }

    void Shader::setUniformV2f(const std::string& name, const JVector2f& vector) {
        setUniformV2f(name, vector.x, vector.y);
    }
    void Shader::setUniformV2f(const std::string& name, const float v0, const float v1) {
        if (!_shaderID) { return; }
        GLCall(glUniform2f(getUniformLocation(name), v0, v1));
    }

    void Shader::setUniformV2i(const std::string& name, const JVector2i& vector) {
        setUniformV2i(name, vector.x, vector.y);
    }
    void Shader::setUniformV2i(const std::string& name, const int32_t v0, const int32_t v1) {
        if (!_shaderID) { return; }
        GLCall(glUniform2i(getUniformLocation(name), v0, v1));
    }

    void Shader::setUniformV2ui(const std::string& name, const JVector2u& vector) {
        setUniformV2ui(name, vector.x, vector.y);
    }
    void Shader::setUniformV2ui(const std::string& name, const uint32_t v0, const uint32_t v1) {
        if (!_shaderID) { return; }
        GLCall(glUniform2ui(getUniformLocation(name), v0, v1));
    }

    void Shader::setUniformV3f(const std::string& name, const JVector3f& vector) {
        setUniformV3f(name, vector.x, vector.y, vector.z);
    }
    void Shader::setUniformV3f(const std::string& name, const float v0, const float v1, const float v2) {
        if (!_shaderID) { return; }
        GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
    }

    void Shader::setUniformV3i(const std::string& name, const JVector3i& vector) {
        setUniformV3i(name, vector.x, vector.y, vector.z);
    }
    void Shader::setUniformV3i(const std::string& name, const int32_t v0, const int32_t v1, const int32_t v2) {
        if (!_shaderID) { return; }
        GLCall(glUniform3i(getUniformLocation(name), v0, v1, v2));
    }

    void Shader::setUniformColor24(const std::string& name, const JColor24& color) {
        setUniformV3ui(name, color.r, color.g, color.b);
    }
    void Shader::setUniformV3ui(const std::string& name, const JVector3u& vector) {
        setUniformV3ui(name, vector.x, vector.y, vector.z);
    }
    void Shader::setUniformV3ui(const std::string& name, const uint32_t v0, const uint32_t v1, const uint32_t v2) {
        if (!_shaderID) { return; }
        GLCall(glUniform3ui(getUniformLocation(name), v0, v1, v2));
    }

    void Shader::setUniformColor(const std::string& name, const JColor& color) {
        setUniformV4f(name, color.r, color.g, color.b, color.a);
    }
    void Shader::setUniformV4f(const std::string& name, const JVector4f& vector) {
        setUniformV4f(name, vector.x, vector.y, vector.z, vector.w);
    }
    void Shader::setUniformV4f(const std::string& name, const float v0, const float v1, const float v2, const float v3) {
        if (!_shaderID) { return; }
        GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::setUniformV4i(const std::string& name, const JVector4i& vector) {
        setUniformV4i(name, vector.x, vector.y, vector.z, vector.w);
    }
    void Shader::setUniformV4i(const std::string& name, const int32_t v0, const int32_t v1, const int32_t v2, const int32_t v3) {
        if (!_shaderID) { return; }
        GLCall(glUniform4i(getUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::setUniformColor32(const std::string& name, const JColor32& color) {
        setUniformV4ui(name, color.r, color.g, color.b, color.a);
    }
    void Shader::setUniformV4ui(const std::string& name, const JVector4u& vector) {
        setUniformV4ui(name, vector.x, vector.y, vector.z, vector.w);
    }
    void Shader::setUniformV4ui(const std::string& name, const uint32_t v0, const uint32_t v1, const uint32_t v2, const uint32_t v3) {
        if (!_shaderID) { return; }
        GLCall(glUniform4ui(getUniformLocation(name), v0, v1, v2, v3));
    }


    void Shader::setUniformMat4f(const std::string& name, const JMatrix4f& mat) {
        if (!_shaderID) { return; }
        GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0]));
    }

    int32_t Shader::getUniformLocation(const std::string& name) {
        if (!_shaderID) { return -1; }
        if (_uniformCache.find(name) != _uniformCache.end()) { return _uniformCache[name]; }

        GLCall(int32_t location = glGetUniformLocation(_shaderID, name.c_str()));
        if (location == -1) {
            std::cout << "[JEngine - Shader] Warning: uniform '" << name << "' doesn't exist!" << std::endl;
        }
        _uniformCache[name] = location;
        return location;
    }

    ShaderSources Shader::parseShader(const std::string& filePath) {
        std::ifstream stream(filePath);
        return parseShader(stream);
    }

    ShaderSources Shader::parseShader(std::istream& stream)  {
        enum class ShaderType {
            NONE = -1,
            VERTEX = 0,
            FRAGMENT = 1,
        };

        std::string line;
        std::stringstream ss[2];

        ShaderType type = ShaderType::NONE;
        uint16_t blendSrc = GL_SRC_ALPHA;
        uint16_t blendDst = GL_ONE_MINUS_SRC_ALPHA;
        uint16_t blendFunc = GL_FUNC_ADD;
        while (getline(stream, line)) {
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

                if (ind > -1) {
                    blendSrc = strToGLBlendingMode(temp.slice(0, ind), true);

                    temp = temp.slice(ind + 1);
                    ind = temp.indexOf(' ');

                    if (ind > -1) {
                        blendDst = strToGLBlendingMode(temp.slice(0, ind), false);
                        blendFunc = strToGLBlendingMode(temp.slice(ind + 1), false);
                    }
                    else {
                        blendDst = strToGLBlendingMode(temp, false);
                    }
                    continue;
                }

                blendSrc = strToGLBlendingMode(temp, true);
                continue;
            }
            if (type == ShaderType::NONE) { continue; }

            ss[(int)type] << line << '\n';
        }
        return { ss[0].str(), ss[1].str(), blendSrc, blendDst, blendFunc };
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