#include <JEngine/Assets/Graphics/Material/Material.h>

namespace JEngine {
    Material::Material() : _mainTex(), _shader(){ }
    Material::Material(const ObjectRef<Shader>& shader) : _shader(shader), _mainTex() { }
    Material::Material(const ObjectRef<Texture2D>& texture, const ObjectRef<Shader>& shader) : _mainTex(texture), _shader(shader) { }

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

    const bool Material::operator<(const Material& other) const {
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
        return _id < other._id;
    }

    const bool Material::operator>(const Material& other) const {
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
        return _id > other._id;
    }

    const bool Material::operator==(const Material& other) const {
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

    const bool Material::operator!=(const Material& other) const  {
        return !(*this == other);
    }

    const bool Material::trySetProperty(const MaterialProperty& prop) {
        const int32_t ind = indexOfProperty(prop.getName());
        if (ind > -1) {
            _properties[ind] = prop;
            return true;
        }
        return false;
    }
    const bool Material::tryGetProperty(const std::string& name, MaterialProperty& prop) const {
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

    void Material::setTexture(const ObjectRef<Texture2D>& tex) {
        _mainTex = tex;
    }

    void Material::setFrameBuffer(const ObjectRef<FrameBuffer>& fBuffer) {
        _frameBuffer = fBuffer;
    }

    const bool Material::serializeJson(json& jsonF) const {

        return true;
    }

    const bool Material::deserializeJson(json& jsonF) {

        return true;
    }

    const bool Material::serializeBinary(std::ostream& stream) const {
        return true;
    }

    const bool Material::deserializeBinary(std::istream& stream, const size_t size) {
        //IAsset::deserializeBinary(stream, size);

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

    const bool Material::bind() {
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
            if (prop.getType() == MaterialProperty::Texture) {
                auto texP = prop.asTexture2D().getPtr();
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

        auto tex = _mainTex.getPtr();
        if (tex) {
            tex->unbind();
        }

        for (auto& prop : _properties) {
            if (prop.getType() == MaterialProperty::Texture) {
                auto texP = prop.asTexture2D().getPtr();
                if (texP) {
                    texP->unbind();
                }
            }
        }
    }

    const int32_t Material::indexOfProperty(const std::string& name) const {
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
                case MaterialProperty::UInt:
                    shader.setUniform1i(prop.getName(), prop.asUInt());
                    break;    
                
                case MaterialProperty::Float:
                    shader.setUniform1f(prop.getName(), prop.asFloat());
                    break;

                case MaterialProperty::Vector2f: 
                    shader.setUniformV2f(prop.getName(), prop.asVector2f()); break;
                    
                case MaterialProperty::Vector2i:
                    shader.setUniformV2i(prop.getName(), prop.asVector2i()); break;
                    
                case MaterialProperty::Vector3f:
                    shader.setUniformV3f(prop.getName(), prop.asVector3f()); break;

                case MaterialProperty::Vector3i:
                    shader.setUniformV3i(prop.getName(), prop.asVector3i()); break;

                case MaterialProperty::Vector4f:
                    shader.setUniformV4f(prop.getName(), prop.asVector4f()); break;

                case MaterialProperty::Vector4i:
                    shader.setUniformV4i(prop.getName(), prop.asVector4i()); break;
                    
                case MaterialProperty::Matrix4f:
                    shader.setUniformMat4f(prop.getName(), prop.asMatrix4f()); break;

                case MaterialProperty::Texture:
                    pos += shader.setTextures(prop.getName(), prop.asTexture2D().getPtr(), pos);
                    break;
                default: break;
            }
        }
    }
}