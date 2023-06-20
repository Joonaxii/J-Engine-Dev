#include <JEngine/Assets/Graphics/Material/Material.h>

namespace JEngine {
    Material::Material() : _mainTex(), _shader(){ }
    Material::Material(const ObjectRef<Shader>& shader) : _shader(shader), _mainTex() { }
    Material::Material(const ObjectRef<Texture>& texture, const ObjectRef<Shader>& shader) : _mainTex(texture), _shader(shader) { }

    void Material::addProperty(const MaterialProperty& prop) {
        const int32_t index = indexOfProperty(prop.getName());
        if (index > -1) {
            _properties[index] = prop;
            return;
        }
        _properties.push_back(prop);
    }
    void Material::removeProperty(const std::string& name) {
        const int32_t index = indexOfProperty(name);
        if (index > -1) {
            _properties.erase(_properties.begin() + index);
        }
    }

    bool Material::operator<(const Material& other) const {
        auto shdrA = _shader.getPtr();
        auto shdrB = other._shader.getPtr();

        if (shdrA && shdrB) {
            const auto uniA = shdrA->getBlendUnion();
            const auto uniB = shdrB->getBlendUnion();
            if (uniA == uniB) {
                goto end;
            }

            return uniA < uniB;
        }
        else if(shdrA && !shdrB) {
            return false;
        }
        else {
            if (!shdrA && shdrB) { return true; }
        }

        end:
        return getId() < other.getId();
    }

    bool Material::operator>(const Material& other) const {
        auto shdrA = _shader.getPtr();
        auto shdrB = other._shader.getPtr();

        if (shdrA && shdrB) {
            const auto uniA = shdrA->getBlendUnion();
            const auto uniB = shdrB->getBlendUnion();
            if (uniA == uniB) {
                goto end;
            }

            return uniA > uniB;
        }
        else if (shdrA && !shdrB) {
            return true;
        }
        else {
            if (!shdrA && shdrB) { return false; }
        }

    end:
        return getId() > other.getId();
    }

    bool Material::operator==(const Material& other) const {
        if (_mainTex.getID() != other._mainTex.getID()) { return false; }
        if (_shader.getID() != other._shader.getID()) { return false; }

        auto shdrA = _shader.getPtr();
        auto shdrB = other._shader.getPtr();

        if (shdrA && shdrB) {
            if (shdrA->getBlendUnion() != shdrB->getBlendUnion()) { return false; }
        }

        size_t size = _properties.size();
        if (size != other._properties.size()) { return false; }

        size_t count = size;
        for (size_t i = 0; count > 0 && i < size; i++) {
            auto& propA = _properties[i];
            auto& propB = other._properties[i];
            if (propA != propB) {
                for (size_t j = 0; j < size; j++)  {
                    auto& propC = other._properties[i];
                    if (propC == propA) { count--; break; }
                }
                continue;
            }
            count--;
        }
        return count == 0;
    }

    bool Material::operator!=(const Material& other) const  {
        return !(*this == other);
    }

    bool Material::trySetProperty(const MaterialProperty& prop) {
        const int32_t ind = indexOfProperty(prop.getName());
        if (ind > -1) {
            _properties[ind] = prop;
            return true;
        }
        return false;
    }
    bool Material::tryGetProperty(const std::string& name, MaterialProperty& prop) const {
        const int32_t ind = indexOfProperty(name);
        if (ind > -1) {
            prop = _properties[ind];
            return true;
        }
        return false;
    }
    const std::vector<MaterialProperty>& Material::getProperties() const {
        return _properties;
    }

    void Material::setFrameBuffer(const ObjectRef<FrameBuffer>& fBuffer) {
        _frameBuffer = fBuffer;
    }

    bool Material::serializeJson(json& jsonF) const {
        const UUID8 shaderUUID = _shader.getPtr() ? _shader.getPtr()->getUUID() : UUID8::Empty;
        const UUID8 mainTexUUID = _mainTex.getPtr() ? _mainTex.getPtr()->getUUID() : UUID8::Empty;

        Serialization::serialize(shaderUUID, jsonF["shader"]);
        Serialization::serialize(mainTexUUID, jsonF["mainTexture"]);

        json::array_t props{};
        for (const MaterialProperty& prop : _properties) {
            json propJS{};
            Serialization::serialize(prop, propJS);
            props.push_back(propJS);
        }
        jsonF["properties"] = props;
        return true;
    }

    bool Material::deserializeJson(json& jsonF) {
        _properties.clear();

        UUID8 shaderUUID = UUID8::Empty;
        UUID8 mainTexUUID = UUID8::Empty;

        Serialization::deserialize(shaderUUID, jsonF["shader"]);
        Serialization::deserialize(mainTexUUID, jsonF["mainTexture"]);

        auto& props = jsonF["properties"];
        if (props.is_array()) {
            for (auto& prop : props) {
                MaterialProperty propMat;
                Serialization::deserialize(propMat, prop);
                _properties.push_back(propMat);
            }
        }
        return true;
    }

    bool Material::deserializeYaml(const yamlNode& yamlIn) {
        UUID8 shaderUUID = UUID8::Empty;
        UUID8 mainTexUUID = UUID8::Empty;

        Serialization::deserialize("shader", shaderUUID, yamlIn);
        Serialization::deserialize("mainTexture", mainTexUUID, yamlIn);
        Serialization::deserialize("properties", _properties, yamlIn);
        return true;
    }

    bool Material::serializeYaml(yamlEmit& yamlOut) const {
        const UUID8 shaderUUID = _shader.getPtr() ? _shader.getPtr()->getUUID() : UUID8::Empty;
        const UUID8 mainTexUUID = _mainTex.getPtr() ? _mainTex.getPtr()->getUUID() : UUID8::Empty;
        yamlOut << YAML::BeginMap;
        Serialization::serialize("shader", shaderUUID, yamlOut);
        Serialization::serialize("mainTexture", mainTexUUID, yamlOut);
        Serialization::serialize("properties", _properties.data(), _properties.size(), yamlOut);
        yamlOut << YAML::EndMap;
        return true;
    }

    bool Material::serializeBinary(const Stream& stream) const {
        const UUID8 shaderUUID = _shader.getPtr() ? _shader.getPtr()->getUUID() : UUID8::Empty;
        const UUID8 mainTexUUID = _mainTex.getPtr() ? _mainTex.getPtr()->getUUID() : UUID8::Empty;

        Serialization::serialize(shaderUUID, stream);
        Serialization::serialize(mainTexUUID, stream);

        const int32_t props = int32_t(_properties.size());
        Serialization::serialize(props, stream);

        for (const auto& prop : _properties) {
            Serialization::serialize(prop, stream);
        }
        return true;
    }

    bool Material::deserializeBinary(const Stream& stream, const size_t size) {
        UUID8 shaderUUID = UUID8::Empty;
        UUID8 mainTexUUID = UUID8::Empty;

        Serialization::deserialize(shaderUUID, stream);
        Serialization::deserialize(mainTexUUID, stream);

        _properties.clear();
        int32_t props;
        Serialization::deserialize(props, stream);
        _properties.resize(props);

        for (size_t i = 0; i < props; i++) {
            Serialization::deserialize(_properties[i], stream);
        }
        return true;
    }

    const Shader* Material::getShaderPtr() const {
        return _shader.getPtr();
    }

    Shader* Material::getShaderPtr() {
        return _shader.getPtr();
    }

    ObjectRef<Shader>& Material::getShader() {
        return _shader;
    }

    const ObjectRef<Shader>& Material::getShader() const {
        return _shader;
    }

    void Material::setMainTexture(const ObjectRef<Texture>& tex) {
        _mainTex = tex;
    }

    const Texture* Material::getMainTexturePtr() const {
        return _mainTex.getPtr();
    }

    Texture* Material::getMainTexturePtr() {
        return _mainTex.getPtr();
    }

    ObjectRef<Texture>& Material::getMainTexture() {
        return _mainTex;
    }

    const ObjectRef<Texture>& Material::getMainTexture() const {
        return _mainTex;
    }

    bool Material::bind() {
        Shader* shader = _shader.getPtr();
        if (!shader) { return false; }

        shader->bind();
        uint32_t texSlot = 0;
        auto fBuffer = _frameBuffer.getPtr();
        auto tex = _mainTex.getPtr();
        if (!fBuffer && tex) {
            texSlot = tex->bind(texSlot);
        }else if (fBuffer) {
            texSlot = fBuffer->bindColorAttachment(texSlot);
        }

        for (auto& prop : _properties) {
            if (prop.getType() == MaterialProperty::T_Texture) {
                auto texP = prop.asTexture().getPtr();
                if (texP) {
                    texSlot = texP->bind(texSlot);
                }
                else {
                    texSlot = Texture::bindNull(texSlot);
                }
            }
        }

        applyProperties(*shader);
        return true;
    }

    void Material::unbind() const {
        auto shader = _shader.getPtr();
        if (shader) {
            shader->unbind();
        }

        uint32_t slot = 0;
        auto tex = _mainTex.getPtr();
        if (tex) {
            slot = tex->unBind(slot);
        }

        for (auto& prop : _properties) {
            if (prop.getType() == MaterialProperty::T_Texture) {
                auto texP = prop.asTexture().getPtr();
                if (texP) {
                    slot = texP->unBind(slot);
                }
            }
        }
    }

    int32_t Material::indexOfProperty(const std::string& name) const {
        for (size_t i = 0; i < _properties.size(); i++) {
            const auto& prop = _properties[i];
            if (prop.getName() == name) { return int32_t(i); }
        }
        return -1;
    }

    void Material::applyProperties(Shader& shader) const {
        uint32_t pos = shader.setTextures("_MainTex", _mainTex.getPtr());
        for (const auto& prop : _properties) {
            switch (prop.getType())
            {
                case MaterialProperty::T_UInt:
                    shader.setUniform1i(prop.getName(), prop.asUInt());
                    break;    
                
                case MaterialProperty::T_Float:
                    shader.setUniform1f(prop.getName(), prop.asFloat());
                    break;

                case MaterialProperty::T_Vector2f: 
                    shader.setUniformV2f(prop.getName(), prop.asVector2f()); break;
                    
                case MaterialProperty::T_Vector2i:
                    shader.setUniformV2i(prop.getName(), prop.asVector2i()); break;
                    
                case MaterialProperty::T_Vector3f:
                    shader.setUniformV3f(prop.getName(), prop.asVector3f()); break;

                case MaterialProperty::T_Vector3i:
                    shader.setUniformV3i(prop.getName(), prop.asVector3i()); break;

                case MaterialProperty::T_Vector4f:
                    shader.setUniformV4f(prop.getName(), prop.asVector4f()); break;

                case MaterialProperty::T_Vector4i:
                    shader.setUniformV4i(prop.getName(), prop.asVector4i()); break;
                    
                case MaterialProperty::T_Matrix4f:
                    shader.setUniformMat4f(prop.getName(), prop.asMatrix4f()); break;

                case MaterialProperty::T_Color32:
                    shader.setUniformColor32(prop.getName(), prop.asColor32()); break;

                case MaterialProperty::T_Color24:
                    shader.setUniformColor24(prop.getName(), prop.asColor24()); break;

                case MaterialProperty::T_Color:
                    shader.setUniformColor(prop.getName(), prop.asColor()); break;

                case MaterialProperty::T_Texture:
                    pos += shader.setTextures(prop.getName(), prop.asTexture().getPtr(), pos);
                    break;
                default: break;
            }
        }
    }
}