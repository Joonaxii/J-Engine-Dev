#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <cstdint>

namespace JEngine {
    struct JColor;
    struct JColor24;
    struct JColor555;
    struct JColor565;
    struct JColor32 {
        static const JColor32 White;
        static const JColor32 Black;
        static const JColor32 Gray;
        static const JColor32 Clear;
        static const JColor32 Red;
        static const JColor32 Green;
        static const JColor32 Blue;
        static const JColor32 Magenta;
        static const JColor32 Yellow;
        static const JColor32 Cyan;

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        JColor32();
        JColor32(const uint8_t r, const uint8_t g, const uint8_t b);
        JColor32(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

        JColor32(const JColor& rgba);
        JColor32(const JColor24& rgba);
        JColor32(const JColor24& rgb, const uint8_t alpha);

        JColor32(const JColor555& rgb);
        JColor32(const JColor555& rgb, const uint8_t alpha);
        JColor32(const JColor565& rgb);
        JColor32(const JColor565& rgb, const uint8_t alpha);

        operator uint32_t() const { return *reinterpret_cast<const uint32_t*>(this); }

        operator JColor24() const;
        operator JColor() const;

        void set(const JColor& rgba);
        void set(const JColor24& rgb);
        void set(const uint8_t r, const uint8_t g, const uint8_t b);
        void set(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a);

        bool operator==(const JColor32& other) const;
        bool operator!=(const JColor32& other) const;

        JColor32 operator-(const JColor32& other) const;
        JColor32 operator+(const JColor32& other) const;

        JColor32& operator-=(const JColor32& other);
        JColor32& operator+=(const JColor32& other);

        JColor32 operator/(const float value) const;
        JColor32 operator*(const float value) const;

        JColor32& operator/=(const float value);
        JColor32& operator*=(const float value);

        JColor32 operator*(const JColor32& other) const;
        JColor32& operator*=(const JColor32& other);

        bool operator==(const JColor24& other) const;
        bool operator!=(const JColor24& other) const;

        bool operator<(const JColor32& other) const {
            return *reinterpret_cast<const uint32_t*>(this) < *reinterpret_cast<const uint32_t*>(&other);
        };
        bool operator>(const JColor32& other) const {
            return *reinterpret_cast<const uint32_t*>(this) > *reinterpret_cast<const uint32_t*>(&other);
        };

        bool operator<(const JColor24& other) const;
        bool operator>(const JColor24& other) const;

        void flipRB();
    };

    inline std::ostream& operator<<(std::ostream& os, const JColor32& rgba) {
        os << "RGBA: (" << int32_t(rgba.r) << ", " << int32_t(rgba.g) << ", " << int32_t(rgba.b) << ", " << int32_t(rgba.a) << ")";
        return os;
    }

    inline const JColor32 JColor32::Clear = JColor32(0x00, 0x00, 0x00, 0x00);
    inline const JColor32 JColor32::White = JColor32(0xFF, 0xFF, 0xFF, 0xFF);
    inline const JColor32 JColor32::Black = JColor32(0x00, 0x00, 0x00, 0xFF);
    inline const JColor32 JColor32::Gray = JColor32(0x7F, 0x7F, 0x7F, 0xFF);

    inline const JColor32 JColor32::Red = JColor32(0xFF, 0x00, 0x00, 0xFF);
    inline const JColor32 JColor32::Green = JColor32(0x00, 0xFF, 0x00, 0xFF);
    inline const JColor32 JColor32::Blue = JColor32(0x00, 0x00, 0xFF, 0xFF);
    inline const JColor32 JColor32::Magenta = JColor32(0xFF, 0x00, 0xFF, 0xFF);
    inline const JColor32 JColor32::Yellow = JColor32(0xFF, 0xFF, 0x00, 0xFF);
    inline const JColor32 JColor32::Cyan = JColor32(0x00, 0xFF, 0xFF, 0xFF);

}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JColor32& itemRef) {
        yamlOut << YAML::Flow << YAML::Hex;
        yamlOut << YAML::BeginSeq << uint16_t(itemRef.r) << uint16_t(itemRef.g) << uint16_t(itemRef.b) << uint16_t(itemRef.a) << YAML::EndSeq;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::JColor32> {
        static Node encode(const JEngine::JColor32& rhs) {
            Node node;
            node.push_back(uint16_t(rhs.r));
            node.push_back(uint16_t(rhs.g));
            node.push_back(uint16_t(rhs.b));
            node.push_back(uint16_t(rhs.a));
            return node;
        }

        static bool decode(const Node& node, JEngine::JColor32& rhs) {
            if (!node.IsSequence() || node.size() < 1) { return false; }

            uint8_t r = node[0].as<uint8_t>();
            uint8_t g = r;
            uint8_t b = r;
            uint8_t a = 0xFF;
            switch (node.size()) {
                case 1:     //Grayscale, same as init values
                    break; 
                case 2:     //RG
                    g = node[1].as<uint8_t>();
                    break;
                case 3:     //RGB
                    g = node[1].as<uint8_t>();
                    b = node[2].as<uint8_t>();
                    break;
                default:    //RGBA
                    g = node[1].as<uint8_t>();
                    b = node[2].as<uint8_t>();
                    a = node[2].as<uint8_t>();
                    break;
            }
            rhs.set(r, g, b, a);
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<JColor32>::deserializeJson(JColor32& itemRef, const json& jsonF, const JColor32& defaultValue) {
        Serialization::deserialize(itemRef.r, jsonF["r"], defaultValue.r);
        Serialization::deserialize(itemRef.g, jsonF["g"], defaultValue.g);
        Serialization::deserialize(itemRef.b, jsonF["b"], defaultValue.b);
        Serialization::deserialize(itemRef.a, jsonF["a"], defaultValue.a);
        return true;
    }

    template<>
    inline bool Serializable<JColor32>::serializeJson(const JColor32& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.r, jsonF["r"]);
        Serialization::serialize(itemRef.g, jsonF["g"]);
        Serialization::serialize(itemRef.b, jsonF["b"]);
        Serialization::serialize(itemRef.a, jsonF["a"]);
        return true;
    }
}
#pragma endregion