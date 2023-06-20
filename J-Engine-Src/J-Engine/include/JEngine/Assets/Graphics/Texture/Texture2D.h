#pragma once
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>

namespace JEngine {
    class Texture2D : public Texture {
    public:
        Texture2D();
   
        bool serializeBinary(const Stream& stream) const;
        bool deserializeBinary(const Stream& stream, const size_t size);

        bool serializeJson(json& jsonF) const;
        bool deserializeJson(json& jsonF);
    };
}

REGISTER_ASSET(JEngine::Texture2D);