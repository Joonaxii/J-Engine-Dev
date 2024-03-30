#include <JEngine/Assets/Serializers/ShaderSerializer.h>

namespace JEngine {
    void ShaderSerializer::deserializeImpl(FileEntry* entry, IAsset* asset, const Stream& stream, uint8_t flags) {
        if (!asset) { return; }
        auto shader = asAsset(asset);

        if (flags & SER_FullData) {
            size_t len = 0;
            stream.read(&len, 4, false);
            shader->_shader.resize(len);
            stream.read(&shader->_shader[0], len, false);

            auto srcs = shader->parseShader();
            shader->createShader(srcs.vertexSource.c_str(), srcs.fragmentSource.c_str());
        }
    }

    void ShaderSerializer::serializeImpl(FileEntry* entry, const IAsset* asset, const Stream& stream, uint8_t flags) {
        if (!asset) { return; }
        auto shader = asAsset(asset);

        if (flags & SER_FullData) {

            size_t len = shader->_shader.size();
            stream.write(&len, 4, false);
            stream.write(shader->_shader.data(), len, false);
        }
    }
}