#pragma once 
#include <JEngine/IO/Serialization/Serializable.h>
#include <cstdint>

static constexpr uint16_t RED_MASK_565 = 0x1F;
static constexpr uint16_t GREEN_MASK_565 = 0x7E0;
static constexpr uint16_t BLUE_MASK_565 = 0xF800;

namespace JEngine {
    struct JColor24;
    struct JColor555;
    struct JColor32;
    struct JColor565 {
        static const JColor565 White;
        static const JColor565 Black;
        static const JColor565 Gray;
        static const JColor565 Red;
        static const JColor565 Green;
        static const JColor565 Blue;
        static const JColor565 Magenta;
        static const JColor565 Yellow;
        static const JColor565 Cyan;

        uint16_t data{ 0 };

        JColor565() : data(0) {}
        JColor565(uint16_t value) : data(value) {}
        JColor565(uint8_t r, uint8_t g, uint8_t b);

        JColor565(const JColor555& rgb);
        JColor565(const JColor24& rgb);
        JColor565(const JColor32& rgba);

        constexpr bool operator <(const JColor565& other) const { return data < other.data; }
        constexpr bool operator >(const JColor565& other) const { return data > other.data; }

        void flipRB();
    };
}


#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JColor565& itemRef) {
        return yamlOut << YAML::Hex << itemRef.data;
    }

    template<>
    struct convert<JEngine::JColor565> {
        static Node encode(const JEngine::JColor565& rhs) {
            Node node(rhs.data);
            return node;
        }

        static bool decode(const Node& node, JEngine::JColor565& rhs) {
            if (!node.IsScalar() || node.size() < 1) { return false; }
            rhs.data = node.as<uint16_t>();
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<JColor565>::deserializeJson(JColor565& itemRef, json& jsonF, const JColor565& defaultValue) {
        itemRef.data = jsonF.is_number_integer() ? jsonF.get<uint16_t>() : 0;
        return true;
    }

    template<>
    inline bool Serializable<JColor565>::serializeJson(const JColor565& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.data, jsonF);
        return true;
    }
}
#pragma endregion