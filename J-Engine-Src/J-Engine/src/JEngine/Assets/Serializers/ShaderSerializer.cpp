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
            if (shader->_data) {
                stream.write(shader->_data, shader->_length, false);
            }
        }
    }
}