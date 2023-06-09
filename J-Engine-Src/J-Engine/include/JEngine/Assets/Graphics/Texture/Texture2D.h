#pragma once
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>

namespace JEngine {
    class Texture2D : public Texture {
    public:
        Texture2D();
   
        bool serializeBinary(std::ostream& stream) const override;
        bool deserializeBinary(std::istream& stream, const size_t size) override;

        bool serializeJson(json& jsonF) const override;
        bool deserializeJson(json& jsonF) override;

    protected:
        virtual bool jsonToBinaryImpl(json& jsonF, std::ostream& stream) const override { return false; }
    };
}

REGISTER_ASSET(JEngine::Texture2D);