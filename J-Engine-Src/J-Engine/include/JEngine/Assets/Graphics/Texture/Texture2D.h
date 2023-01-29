#pragma once
#include <JEngine/Assets/IAsset.h>
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/Assets/Graphics/Texture/Texture.h>

namespace JEngine {
    class Texture2D : public Texture, public ISerializable<Texture2D> {
    public:
        static constexpr char* JTEX_HEADER = "JTEX";

        Texture2D();
   
        const bool serializeBinary(std::ostream& stream) const override;
        const bool deserializeBinary(std::istream& stream, const size_t size) override;

        const bool serializeJson(json& jsonF) const override;
        const bool deserializeJson(json& jsonF) override;

    };
}

REGISTER_ASSET(JEngine::Texture2D, Texture2D);