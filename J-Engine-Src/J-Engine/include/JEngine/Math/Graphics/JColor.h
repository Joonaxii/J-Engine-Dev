#pragma once
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    struct JColor24;
    struct JColor32;
    struct JColor {
        static const JColor White;
        static const JColor Black;
        static const JColor Gray;
        static const JColor Clear;
        static const JColor Red;
        static const JColor Green;
        static const JColor Blue;
        static const JColor Magenta;
        static const JColor Yellow;
        static const JColor Cyan;
        
        float r;
        float g;
        float b;
        float a;

        JColor();
        JColor(const float r, const float g, const float b);
        JColor(const float r, const float g, const float b, const float a);
        JColor(const JColor24& rgb);
        JColor(const JColor24& rgb, const float alpha);
        JColor(const JColor32& rgba);

        bool operator==(const JColor& other) const;
        bool operator!=(const JColor& other) const;

        operator JColor32() const;
        operator JColor24() const;

        void set(const JColor32& rgba);
        void set(const JColor24& rgb);
        void set(const float r, const float g, const float b);
        void set(const float r, const float g, const float b, const float a);

        JColor operator-(const JColor& other) const;
        JColor operator+(const JColor& other) const;

        JColor& operator-=(const JColor& other);
        JColor& operator+=(const JColor& other);

        JColor operator/(const float value) const;
        JColor operator*(const float value) const;

        JColor& operator/=(const float value);
        JColor& operator*=(const float value);

        JColor operator/(const JColor& other) const;
        JColor operator*(const JColor& other) const;

        JColor& operator/=(const JColor& other);
        JColor& operator*=(const JColor& other);
    };

    inline std::ostream& operator<<(std::ostream& os, const JColor& rgba) {
        os << "RGBA: (" << rgba.r << ", " << rgba.g << ", " << rgba.b << ", " << rgba.a << ")";
        return os;
    }

    inline const JColor JColor::Clear = JColor(0.0f, 0.0f, 0.0f, 0.0f);
    inline const JColor JColor::White = JColor(1.0f, 1.0f, 1.0f, 1.0f);
    inline const JColor JColor::Black = JColor(0.0f, 0.0f, 0.0f, 1.0f);
    inline const JColor JColor::Gray = JColor(0.5f, 0.5f, 0.5f, 1.0f);

    inline const JColor JColor::Red = JColor(1.0f, 0.0f, 0.0f, 1.0f);
    inline const JColor JColor::Green = JColor(0.0f, 1.0f, 0.0f, 1.0f);
    inline const JColor JColor::Blue = JColor(0.0f, 0.0f, 1.0f, 1.0f);
    inline const JColor JColor::Magenta = JColor(1.0f, 0.0f, 1.0f, 1.0f);
    inline const JColor JColor::Yellow = JColor(1.0f, 1.0f, 0.0f, 1.0f);
    inline const JColor JColor::Cyan = JColor(0.0f, 1.0f, 1.0f, 1.0f);
}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::JColor& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << itemRef.r << itemRef.g << itemRef.b << itemRef.a << YAML::EndSeq;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::JColor> {
        static Node encode(const JEngine::JColor& rhs) {
            Node node;
            node.push_back(rhs.r);
            node.push_back(rhs.g);
            node.push_back(rhs.b);
            node.push_back(rhs.a);
            return node;
        }

        static bool decode(const Node& node, JEngine::JColor& rhs) {
            if (!node.IsSequence() || node.size() < 1) { return false; }

            float r = node[0].as<float>();
            float g = r;
            float b = r;
            float a = 1.0f;
            switch (node.size()) {
                case 1:     //Grayscale, same as init values
                    break;
                case 2:     //RG
                    g = node[1].as<float>();
                    break;
                case 3:     //RGB
                    g = node[1].as<float>();
                    b = node[2].as<float>();
                    break;
                default:     //RGBA
                    g = node[1].as<float>();
                    b = node[2].as<float>();
                    a = node[3].as<float>();
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
    inline bool Serializable<JColor>::deserializeJson(JColor& itemRef, const json& jsonF, const JColor& defaultValue) {
        Serialization::deserialize(itemRef.r, jsonF["r"], defaultValue.r);
        Serialization::deserialize(itemRef.g, jsonF["g"], defaultValue.g);
        Serialization::deserialize(itemRef.b, jsonF["b"], defaultValue.b);
        Serialization::deserialize(itemRef.a, jsonF["a"], defaultValue.a);
        return true;
    }

    template<>
    inline bool Serializable<JColor>::serializeJson(const JColor& itemRef, json& jsonF) {
        Serialization::serialize(itemRef.r, jsonF["r"]);
        Serialization::serialize(itemRef.g, jsonF["g"]);
        Serialization::serialize(itemRef.b, jsonF["b"]);
        Serialization::serialize(itemRef.a, jsonF["a"]);
        return true;
    }
}
#pragma endregion