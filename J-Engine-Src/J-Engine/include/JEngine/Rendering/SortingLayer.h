#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <cstdint>
#include <string>
#include <vector>

namespace JEngine {
    struct SortingLayer {
    public:
        SortingLayer();
        SortingLayer(const int16_t order, const uint16_t layer);
        SortingLayer(const int16_t order, const std::string& layerName);

        const bool operator==(const SortingLayer& other) const;
        const bool operator!=(const SortingLayer& other) const;

        const bool operator<(const SortingLayer& other) const;
        const bool operator>(const SortingLayer& other) const;

        const int32_t compareTo(const SortingLayer& other) const;

        static const std::string& layerToName(const uint16_t layer);
        static const uint16_t nameToLayer(const std::string& name, const bool ignoreCase = false);

        const uint32_t getUnion() const;
        const int16_t getOrder() const;
        const uint16_t getLayerIndex() const;

        void setOrder(const int16_t layer);
        void setLayerByIndex(const uint16_t layer);
        void setLayerByName(const std::string& layer);

        static const uint16_t getLayerCount();
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


#pragma region Serialization
    template<>
    inline const bool Serializable<SortingLayer>::deserializeJson(SortingLayer& itemRef, json& jsonF, const SortingLayer& defaultValue) {
        Serialization::deserialize(itemRef._order, jsonF["order"], defaultValue._order);
        auto& layer = jsonF["layer"];
        itemRef._layer = layer.is_string() ? SortingLayer::nameToLayer(layer.get<std::string>()) : layer.is_number_integer() ? layer.get<uint16_t>() : defaultValue._layer;
        return true;
    }

    template<>
    inline const bool Serializable<SortingLayer>::serializeJson(const SortingLayer& itemRef, json& jsonF) {
        jsonF["order"] = itemRef._order;
        jsonF["layer"] = itemRef._layer;
        return true;
    }
#pragma endregion
}