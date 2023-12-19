#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    static constexpr uint16_t RED_MASK_555 = 0x1F;
    static constexpr uint16_t GREEN_MASK_555 = 0x3E0;
    static constexpr uint16_t BLUE_MASK_555 = 0x7C00;
    static constexpr uint16_t ALPHA_MASK_555 = 0x8000;

    struct JColor565;
    struct JColor24;
    struct JColor32;
    struct JColor555 {
        static const JColor555 Clear;
        static const JColor555 White;
        static const JColor555 Black;
        static const JColor555 Gray;
        static const JColor555 Red;
        static const JColor555 Green;
        static const JColor555 Blue;
        static const JColor555 Magenta;
        static const JColor555 Yellow;
        static const JColor555 Cyan;

        uint16_t data{ 0 };

        JColor555() : data(0) {}
        JColor555(uint16_t value) : data(value) {}
        JColor555(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha);
        JColor555(uint8_t r, uint8_t g, uint8_t b);

        JColor555(const JColor565& rgb);
        JColor555(const JColor24& rgb);
        JColor555(const JColor32& rgba);

        constexpr bool operator <(const JColor555& other) const { return data < other.data; }
        constexpr bool operator >(const JColor555& other) const { return data > other.data; }

        void flipRB();
    };
}



#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JColor555& itemRef) {
        return yamlOut << YAML::Hex << itemRef.data;
    }

    template<>
    struct convert<JEngine::JColor555> {
        static Node encode(const JEngine::JColor555& rhs) {
            Node node(rhs.data);
            return node;
        }

        static bool decode(const Node& node, JEngine::JColor555& rhs) {
            if (!node.IsScalar() || node.size() < 1) { return false; }
            rhs.data = node.as<uint16_t>();
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<JColor555>::deserializeJson(JColor555& itemRef, const json& jsonF, const JColor555& defaultValue) {
        itemRef.data = jsonF.is_number_integer() ? jsonF.get<uint16_t>() : 0;
        return true;
    }

    template<>
    inline bool Serializable<JColor555>::serializeJson(const JColor555& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.data, jsonF);
        return true;
    }
}
#pragma endregion