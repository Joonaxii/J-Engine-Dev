#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <cstdint>
#include <string_view>
#include <vector>

namespace JEngine {
    struct SortingLayer {
    public:
        static constexpr size_t MAX_NAME_LEN = 32;
#pragma pack(push, 1)
        struct LayerName {
            char buffer[MAX_NAME_LEN + 1]{ 0 };

            constexpr LayerName() : buffer{ 0 } {}

            LayerName(const char* input) : buffer{ 0 } {
                memcpy(buffer, input, std::min(MAX_NAME_LEN, strlen(input)));
            }   
            
            LayerName(std::string_view input) : buffer{ 0 } {
                memcpy(buffer, input.data(), std::min(MAX_NAME_LEN, input.length()));
            }
            const char* getName() const { return buffer; }
        };
#pragma pack(pop)

        constexpr SortingLayer::SortingLayer() : _order(0), _layer(0) { }
        constexpr SortingLayer::SortingLayer(int32_t order, int32_t layer) : _order(order + INT16_MIN), _layer(layer) { }
        
        SortingLayer(int32_t order, std::string_view layerName);

        bool operator==(const SortingLayer& other) const;
        bool operator!=(const SortingLayer& other) const;

        bool operator<(const SortingLayer& other) const;
        bool operator>(const SortingLayer& other) const;

        int32_t compareTo(const SortingLayer& other) const;

        static std::string_view layerToName(const uint16_t layer);
        static uint16_t nameToLayer(std::string_view name);

        uint32_t getUnion() const;
        int32_t getOrder() const;
        uint16_t getLayerIndex() const;

        void setOrder(int32_t layer);
        void setLayerByIndex(uint16_t layer);
        void setLayerByName(std::string_view layer);

        static size_t getLayerCount();
        static const std::vector<LayerName>& getLayers();

        static void addLayer(std::string_view layer);

    private:
        static std::vector<LayerName> Layers;

        friend struct Serializable<SortingLayer>;
        uint16_t _order;
        uint16_t _layer;
    };
}

#pragma region Serialization
//YAML
namespace YAML {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const JEngine::SortingLayer& itemRef) {
        yamlOut << YAML::Flow;
        yamlOut << YAML::BeginSeq << itemRef.getLayerIndex() << itemRef.getOrder() << YAML::EndSeq;
        return yamlOut;
    }

    template<>
    struct convert<JEngine::SortingLayer> {
        static Node encode(const JEngine::SortingLayer& rhs) {
            Node node;
            node.push_back(rhs.getLayerIndex());
            node.push_back(rhs.getOrder());
            return node;
        }

        static bool decode(const Node& node, JEngine::SortingLayer& rhs) {
            if (!node.IsSequence() || node.size() < 2) { return false; }
            rhs.setLayerByIndex(node[0].as<uint16_t>());
            rhs.setOrder(node[1].as<int32_t>());
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<SortingLayer>::deserializeJson(SortingLayer& itemRef, const json& jsonF, const SortingLayer& defaultValue) {
        Serialization::deserialize(itemRef._order, jsonF["order"], defaultValue._order);
        auto& layer = jsonF["layer"];
        itemRef._layer = layer.is_string() ? SortingLayer::nameToLayer(layer.get<std::string>()) : layer.is_number_integer() ? layer.get<uint16_t>() : defaultValue._layer;
        return true;
    }

    template<>
    inline bool Serializable<SortingLayer>::serializeJson(const SortingLayer& itemRef, json& jsonF) {
        jsonF["order"] = itemRef._order;
        jsonF["layer"] = itemRef._layer;
        return true;
    }
}
#pragma endregion