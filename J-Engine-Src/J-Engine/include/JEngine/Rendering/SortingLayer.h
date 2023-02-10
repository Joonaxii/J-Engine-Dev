#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <cstdint>
#include <string>
#include <vector>

namespace JEngine {
    struct SortingLayer {
    public:
        SortingLayer();
        SortingLayer(const int32_t order, const uint16_t layer);
        SortingLayer(const int32_t order, const std::string& layerName);

        bool operator==(const SortingLayer& other) const;
        bool operator!=(const SortingLayer& other) const;

        bool operator<(const SortingLayer& other) const;
        bool operator>(const SortingLayer& other) const;

        int32_t compareTo(const SortingLayer& other) const;

        static const std::string& layerToName(const uint16_t layer);
        static uint16_t nameToLayer(const std::string& name, const bool ignoreCase = false);

        uint32_t getUnion() const;
        int32_t getOrder() const;
        uint16_t getLayerIndex() const;

        void setOrder(const int32_t layer);
        void setLayerByIndex(const uint16_t layer);
        void setLayerByName(const std::string& layer);

        static uint16_t getLayerCount();
        static const std::vector<std::string>& getLayers();
        static void setLayers(const std::vector<std::string>& layers);

        static void addLayer(const std::string& layer);
        static void addLayers(const std::vector<std::string>& layers);

    private:
        static std::vector<std::string> LAYERS;

        friend struct Serializable<SortingLayer>;
        uint16_t _order;
        uint16_t _layer;
    };
}

#pragma region Serialization
//YAML
namespace YAML {
    yamlOut& operator<<(yamlOut& yamlOut, const JEngine::SortingLayer& itemRef) {
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
    inline bool Serializable<SortingLayer>::deserializeJson(SortingLayer& itemRef, json& jsonF, const SortingLayer& defaultValue) {
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