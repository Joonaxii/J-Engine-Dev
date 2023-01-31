#include <JEngine/Rendering/SortingLayer.h>
#include <JEngine/Helpers/Helpers.h>

namespace JEngine {
    std::vector<std::string> SortingLayer::LAYERS = { "Default" };

    SortingLayer::SortingLayer() : _order(0), _layer(0) { }

    SortingLayer::SortingLayer(const int16_t order, const std::string& layerName) : SortingLayer(order, nameToLayer(layerName, false)) { }
    SortingLayer::SortingLayer(const int16_t order, const uint16_t layer) : _order(layer + INT16_MIN), _layer(layer) { }

    const bool SortingLayer::operator==(const SortingLayer& other) const {
        return getUnion() == other.getUnion();
    }

    const bool SortingLayer::operator!=(const SortingLayer& other) const {
        return getUnion() != other.getUnion();
    }

    const bool SortingLayer::operator<(const SortingLayer& other) const {
        return getUnion() < other.getUnion();
    }

    const bool SortingLayer::operator>(const SortingLayer& other) const {
        return getUnion() > other.getUnion();
    }

    const int32_t SortingLayer::compareTo(const SortingLayer& other) const {
        const auto uniA = getUnion();
        const auto uniB = other.getUnion();
        return (uniA == uniB) ? 0 : (uniA < uniB ? -1 : 1);
    }

    const std::string& SortingLayer::layerToName(const uint16_t layer) {
        return layer >= getLayerCount() ? LAYERS[0] : LAYERS[layer];
    }

    const uint16_t SortingLayer::nameToLayer(const std::string& name, const bool ignoreCase) {
        const auto layers = getLayerCount();
        if (ignoreCase) {
            for (size_t i = 0; i < layers; i++) {
                if (Helpers::strEqualsIgnoreCase(LAYERS[i], name)) { return uint16_t(i); }
            }
            return 0;
        }
        for (size_t i = 0; i < layers; i++) {
            if (std::strcmp(LAYERS[i].c_str(), name.c_str())) { return uint16_t(i); }
        }
        return 0;
    }

    const uint32_t SortingLayer::getUnion() const {
        return _order | (_layer << 16);
    }

    const int16_t SortingLayer::getOrder() const {
        return (int32_t)_order - INT16_MIN;
    }

    const uint16_t SortingLayer::getLayerIndex() const {
        return _layer;
    }

    void SortingLayer::setOrder(const int16_t layer) {
        _order = layer + INT16_MIN;
    }

    void SortingLayer::setLayerByIndex(const uint16_t layer) {
        if (layer >= getLayerCount()) { return; }
        _layer = layer;
    }    
    
    void SortingLayer::setLayerByName(const std::string& layer) {
        const auto layerInd = nameToLayer(layer, false);
        if (layerInd >= getLayerCount()) { return; }
        _layer = layerInd;
    }

    const uint16_t SortingLayer::getLayerCount() {
        return uint16_t(LAYERS.size());
    }

    const std::vector<std::string>& SortingLayer::getLayers() {
        return LAYERS;
    }

    void SortingLayer::setLayers(const std::vector<std::string>& layers)  {
        LAYERS.clear();
        if (layers.size() < 1) {
            LAYERS.push_back("Default");
            return;
        }
        LAYERS.insert(LAYERS.begin(), layers.begin(), layers.end());
    }

    void SortingLayer::addLayer(const std::string& layer) {
        if (std::find(LAYERS.begin(), LAYERS.end(), layer) != LAYERS.end()) { return; }
        LAYERS.push_back(layer);
    }

    void SortingLayer::addLayers(const std::vector<std::string>& layers) {
        for (const auto& layer : layers) {
            addLayer(layer);
        }
    }
}