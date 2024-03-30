#include <JEngine/Debug/GizmoLayer.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
    std::string GizmoLayer::GIZMO_LAYER_NAMES[]{ "Default" };

    GizmoLayer::GizmoLayer() : _value() {  }

    int32_t GizmoLayer::layerToIndex(const uint64_t layer) {
        const int32_t index = Math::potToIndex(layer);
        return index > -1 ? index : 0;
    }

    uint64_t GizmoLayer::nameToLayer(const std::string& name) {
        const int32_t ind = indexOfLayer(name);
        return 1ULL << (ind < 0 ? 0 : ind);
    }

    const std::string& GizmoLayer::layerToName(const uint64_t layer) {
        return layerIndexToName(layerToIndex(layer));
    }

    const std::string& GizmoLayer::layerIndexToName(const int32_t index) {
        assert(index > -1 && index < 64 && "Layer index out of range (0 - 63)");
        return GIZMO_LAYER_NAMES[index];
    }

    void GizmoLayer::setLayerAtName(const int32_t index, const std::string& name) {
        assert(index > -1 && index < 64 && "Layer index out of range (0 - 63)");
        GIZMO_LAYER_NAMES[index] = name;
    }

    void GizmoLayer::setLayerName(const uint64_t layer, const std::string& name) {
        const int32_t index = Math::potToIndex(layer);
        assert(index > -1 && index < 64 && "Given layer is possibly a mask and not a single layer!");
        GIZMO_LAYER_NAMES[index] = name;
    }

    void GizmoLayer::setLayerNames(const std::string* names, size_t count) {
        count = count > 64 ? 64 : count;
        for (size_t i = 0; i < count; i++) {
            GIZMO_LAYER_NAMES[i] = names[i];
        }
    }

    int32_t GizmoLayer::indexOfLayer(const std::string& name) {
        for (int32_t i = 0; i < 64; i++) {
            if (GIZMO_LAYER_NAMES[i] == name) {
                return i;
            }
        }
        return -1;
    }
}