#include <JEngine/Rendering/SortingLayer.h>
#include <JEngine/Helpers/Helpers.h>
#include <JEngine/Utility/StringHelpers.h>

namespace JEngine {
    std::vector<SortingLayer::LayerName> SortingLayer::Layers = { "Default" };

    SortingLayer::SortingLayer(int32_t order, std::string_view layerName) : SortingLayer(order, nameToLayer(layerName)) { }

    bool SortingLayer::operator==(const SortingLayer& other) const { return getUnion() == other.getUnion(); }
    bool SortingLayer::operator!=(const SortingLayer& other) const { return getUnion() != other.getUnion(); }

    bool SortingLayer::operator<(const SortingLayer& other) const { return getUnion() < other.getUnion(); }
    bool SortingLayer::operator>(const SortingLayer& other) const { return getUnion() > other.getUnion(); }

    int32_t SortingLayer::compareTo(const SortingLayer& other) const {
        uint32_t uniA = getUnion();
        uint32_t uniB = other.getUnion();
        return (uniA == uniB) ? 0 : (uniA < uniB ? -1 : 1);
    }

    std::string_view SortingLayer::layerToName(uint16_t layer) {
        return layer >= getLayerCount() ? std::string_view(Layers[0].buffer) : std::string_view(Layers[layer].buffer);
    }

    uint16_t SortingLayer::nameToLayer(std::string_view name) {
        size_t layers = getLayerCount();
        for (size_t i = 0; i < layers; i++) {
            if (Helpers::strEquals(name, std::string_view(Layers[i].buffer))) { return uint16_t(i); }
        }
        return 0;
    }

    uint32_t SortingLayer::getUnion() const { return _order | (_layer << 16); }

    void SortingLayer::setOrder(const int32_t layer) { _order = uint16_t(layer + INT16_MIN); }
    int32_t SortingLayer::getOrder() const { return int16_t(int32_t(_order) - INT16_MIN); }

    void SortingLayer::setLayerByIndex(const uint16_t layer) {
        if (layer >= getLayerCount()) { return; }
        _layer = layer;
    }

    uint16_t SortingLayer::getLayerIndex() const { return _layer; }

    void SortingLayer::setLayerByName(std::string_view layer) {
        _layer = nameToLayer(layer);;
    }

    const std::vector<SortingLayer::LayerName>& SortingLayer::getLayers() { return Layers; }
    size_t SortingLayer::getLayerCount() { return uint16_t(Layers.size()); }


    void SortingLayer::addLayer(std::string_view layer) {
        for (auto& lr : Layers) {
            if (Helpers::strEquals(layer, std::string_view(lr.buffer))) {
                return;
            }
        }
        Layers.emplace_back(layer);
    }
}