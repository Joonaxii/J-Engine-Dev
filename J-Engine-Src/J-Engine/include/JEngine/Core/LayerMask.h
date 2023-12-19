#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <string_view>

namespace JEngine {
    struct LayerMask {
    public:
        static constexpr size_t MAX_NAME_LEN = 32;
#pragma pack(push, 1)
        struct LayerName {
            char buffer[MAX_NAME_LEN + 1]{ 0 };

            constexpr LayerName() : buffer{ 0 } {}

            LayerName(const char* input) : buffer{ 0 } {
                memcpy(buffer, input, std::min(MAX_NAME_LEN, strlen(input)));
            }
        };
#pragma pack(pop)

        constexpr LayerMask() : _value(0) {}
        constexpr LayerMask(uint32_t value) : _value(value) {}

        constexpr operator uint32_t() const { return _value; }
        constexpr LayerMask LayerMask::operator~() const { return LayerMask(~_value); }

#pragma region Operators UInt32/Size/Templated

        template<typename T>
        bool operator<(const T& other) const {
            return _value < other;
        }

        template<typename T>
        bool operator>(const T& other) const {
            return _value > other;
        }

        template<typename T>
        bool operator<=(const T& other) const {
            return _value <= other;
        }

        template<typename T>
        bool operator>=(const T& other) const {
            return _value >= other;
        }

        template<typename T>
        bool operator==(const T& other) const {
            return _value == other;
        }

        template<typename T>
        bool operator!=(const T& other) const {
            return _value != other;
        }

        template<typename T>
        LayerMask operator&(const T& other) const {
            return LayerMask(_value & other);
        }

        template<typename T>
        LayerMask operator|(const T& other) const {
            return LayerMask(_value | other);
        }

        template<typename T>
        LayerMask operator^(const T& other) const {
            return LayerMask(_value ^ other);
        }

        template<typename T>
        LayerMask& operator&=(const T& other) {
            _value &= other;
            return *this;
        }

        template<typename T>
        LayerMask& operator|=(const T& other) {
            _value |= other;
            return *this;
        }

        template<typename T>
        LayerMask& operator^=(const T& other) {
            _value ^= other;
            return *this;
        }


        template<>
        inline bool operator<(const LayerMask& other) const {
            return _value < other._value;
        }

        template<>
        inline bool operator>(const LayerMask& other) const {
            return _value > other._value;
        }

        template<>
        inline bool operator<=(const LayerMask& other) const {
            return _value <= other._value;
        }

        template<>
        inline bool operator>=(const LayerMask& other) const {
            return _value >= other._value;
        }

        template<>
        inline bool operator==(const LayerMask& other) const {
            return _value == other._value;
        }

        template<>
        inline bool operator!=(const LayerMask& other) const {
            return _value != other._value;
        }

        template<>
        inline LayerMask operator&(const LayerMask& other) const {
            return LayerMask(_value & other._value);
        }

        template<>
        inline LayerMask operator|(const LayerMask& other) const {
            return LayerMask(_value | other._value);
        }

        template<>
        inline LayerMask operator^(const LayerMask& other) const {
            return LayerMask(_value ^ other._value);
        }

        template<>
        inline LayerMask& operator&=(const LayerMask& other) {
            _value &= other._value;
            return *this;
        }

        template<>
        inline LayerMask& operator|=(const LayerMask& other) {
            _value |= other._value;
            return *this;
        }

        template<>
        inline LayerMask& operator^=(const LayerMask& other) {
            _value ^= other._value;
            return *this;
        }

        static LayerMask nameToLayer(std::string_view name);
        static std::string_view layerToName(LayerMask layer);

        static void setLayerName(int32_t index, std::string_view newName);
        static std::string_view getLayerName(int32_t index);

        static const LayerName* getLayerNames() { return LayerNames; }

#pragma endregion
    private:
        static LayerName LayerNames[32];

        friend struct Serializable<LayerMask>;
        friend struct YAML::convert<LayerMask>;
        uint32_t _value;
    };
}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::LayerMask& itemRef) {
        yamlOut << YAML::Hex << static_cast<const uint32_t>(itemRef);
        return yamlOut;
    }

    template<>
    struct convert<JEngine::LayerMask> {
        static Node encode(const JEngine::LayerMask& rhs) {
            Node node;
            node.push_back(static_cast<const uint32_t>(rhs));
            return node;
        }

        static bool decode(const Node& node, JEngine::LayerMask& rhs) {
            rhs = JEngine::LayerMask(node.as<uint32_t>());
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<LayerMask>::deserializeJson(LayerMask& itemRef, const json& jsonF, const LayerMask& defaultValue) {
        itemRef._value = jsonF.is_number_integer() ? jsonF.get<uint32_t>() : defaultValue._value;
        return true;
    }

    template<>
    inline bool Serializable<LayerMask>::serializeJson(const LayerMask& itemRef, json& jsonF) {
        jsonF.update(itemRef._value);
        return true;
    }
}
#pragma endregion