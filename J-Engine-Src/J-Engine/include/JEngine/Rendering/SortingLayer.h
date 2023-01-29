#pragma once
#include <JEngine/IO/Serialization/ISerializable.h>
#include <cstdint>
#include <string>
#include <vector>

namespace JEngine {
    struct SortingLayer : public ISerializable<SortingLayer, sizeof(uint32_t)> {
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
        static const uint16_t nameToLayer(const std::string& name, const bool ignoreCase);

        const uint32_t getUnion() const;
        const int16_t getOrder() const;
        const uint16_t getLayerIndex() const;

        void setOrder(const int16_t layer);
        void setLayerByIndex(const uint16_t layer);
        void setLayerByName(const std::string& layer);

        const bool deserializeJson(json& jsonFile);
        const bool serializeJson(json& jsonFile) const;

        const bool deserializeBinary(std::istream& stream);
        const bool serializeBinary(std::ostream& stream) const;

        static const uint16_t getLayerCount();
        static const std::vector<std::string>& getLayers();
        static void setLayers(const std::vector<std::string>& layers);

        static void addLayer(const std::string& layer);
        static void addLayers(const std::vector<std::string>& layers);

    private:
        static std::vector<std::string> LAYERS;

        uint16_t _order;
        uint16_t _layer;
    };
}