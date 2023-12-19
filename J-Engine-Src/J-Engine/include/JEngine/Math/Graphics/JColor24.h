#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <cstdint>

namespace JEngine {

    struct JColor;
    struct JColor32;
    struct JColor555;
    struct JColor565;

    struct JColor24 {
        static const JColor24 White;
        static const JColor24 Black;
        static const JColor24 Gray;
        static const JColor24 Red;
        static const JColor24 Green;
        static const JColor24 Blue;
        static const JColor24 Magenta;
        static const JColor24 Yellow;
        static const JColor24 Cyan;

        uint8_t r;
        uint8_t g;
        uint8_t b;

        JColor24();
        JColor24(const uint8_t r, const uint8_t g, const uint8_t b);
     
        JColor24(const JColor& rgba);
        JColor24(const JColor32& rgba);

        JColor24(const JColor555& rgb);
        JColor24(const JColor565& rgb);

        operator uint32_t() const;
        operator JColor32() const;
        operator JColor() const;

        void set(const JColor& rgba);
        void set(const JColor32& rgba);
        void set(const uint8_t r, const uint8_t g, const uint8_t b);

        bool operator==(const JColor24& other) const;
        bool operator!=(const JColor24& other) const;

        JColor24 operator-(const JColor24& other) const;
        JColor24 operator+(const JColor24& other) const;

        JColor24& operator-=(const JColor24& other);
        JColor24& operator+=(const JColor24& other);

        JColor24 operator/(const float value) const;
        JColor24 operator*(const float value) const;

        JColor24& operator/=(const float value);
        JColor24& operator*=(const float value);

        JColor24 operator*(const JColor24& other) const;
        JColor24& operator*=(const JColor24& other);
    };

    inline std::ostream& operator<<(std::ostream& os, const JColor24& rgb) {
        os << "RGB: (" << int32_t(rgb.r) << ", " << int32_t(rgb.g) << ", " << int32_t(rgb.b) << ")";
        return os;
    }

    inline const JColor24 JColor24::White = JColor24(0xFF, 0xFF, 0xFF);
    inline const JColor24 JColor24::Black = JColor24(0x00, 0x00, 0x00);
    inline const JColor24 JColor24::Gray = JColor24(0x7F, 0x7F, 0x7F);

    inline const JColor24 JColor24::Red = JColor24(0xFF, 0x00, 0x00);
    inline const JColor24 JColor24::Green = JColor24(0x00, 0xFF, 0x00);
    inline const JColor24 JColor24::Blue = JColor24(0x00, 0x00, 0xFF);
    inline const JColor24 JColor24::Magenta = JColor24(0xFF, 0x00, 0xFF);
    inline const JColor24 JColor24::Yellow = JColor24(0xFF, 0xFF, 0x00);
    inline const JColor24 JColor24::Cyan = JColor24(0x00, 0xFF, 0xFF);
}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JColor24& itemRef) {
        yamlOut << YAML::Flow << YAML::Hex;
        yamlOut << YAML::BeginSeq << uint16_t(itemRef.r) << uint16_t(itemRef.g) << uint16_t(itemRef.b) << YAML::EndSeq;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::JColor24> {
        static Node encode(const JEngine::JColor24& rhs) {
            Node node;
            node.push_back(uint16_t(rhs.r));
            node.push_back(uint16_t(rhs.g));
            node.push_back(uint16_t(rhs.b));
            return node;
        }

        static bool decode(const Node& node, JEngine::JColor24& rhs) {
            if (!node.IsSequence() || node.size() < 1) { return false; }

            uint8_t r = node[0].as<uint8_t>();
            uint8_t g = r;
            uint8_t b = r;
            switch (node.size()) {
                case 1:     //Grayscale, same as init values
                    break;
                case 2:     //RG
                    g = node[1].as<uint8_t>();
                    break;
                default:    //RGB
                    g = node[1].as<uint8_t>();
                    b = node[2].as<uint8_t>();
                    break;
            }
            rhs.set(r, g, b);
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<JColor24>::deserializeJson(JColor24& itemRef, const json& jsonF, const JColor24& defaultValue) {
        Serialization::deserialize(itemRef.r, jsonF["r"], defaultValue.r);
        Serialization::deserialize(itemRef.g, jsonF["g"], defaultValue.g);
        Serialization::deserialize(itemRef.b, jsonF["b"], defaultValue.b);
        return true;
    }

    template<>
    inline bool Serializable<JColor24>::serializeJson(const JColor24& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.r, jsonF["r"]);
        Serialization::serialize(itemRef.g, jsonF["g"]);
        Serialization::serialize(itemRef.b, jsonF["b"]);
        return true;
    }
}
#pragma endregion