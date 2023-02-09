#pragma once 
#include <JEngine/IO/Serialization/Serializable.h>

namespace JEngine {
    struct GizmoLayer {
    public:

        GizmoLayer();
        GizmoLayer(const int32_t layerIndex);
        GizmoLayer(const uint64_t layer);
        GizmoLayer(const std::string& layerName);

        static int32_t layerToIndex(const uint64_t layer);

        static uint64_t nameToLayer(const std::string& name);
        static const std::string& layerToName(const uint64_t layer);
        static const std::string& layerIndexToName(const int32_t index);

        static void setLayerAtName(const int32_t index, const std::string& name);
        static void setLayerName(const uint64_t layer, const std::string& name);
        static void setLayerNames(const std::string* names, size_t count);

    private:
        static std::string GIZMO_LAYER_NAMES[64];

        friend struct Serializable<GizmoLayer>;
        uint64_t _value;

        static int32_t indexOfLayer(const std::string& name);
    };


#pragma region Serialization
    template<>
    inline bool Serializable<GizmoLayer>::deserializeJson(GizmoLayer& itemRef, json& jsonF, const GizmoLayer& defaultValue) {
        if (jsonF.is_string()) {
            itemRef._value = GizmoLayer::nameToLayer(jsonF.get<std::string>());
            return true;
        }

        itemRef._value = jsonF.is_number_integer() ? jsonF.get<uint64_t>() : defaultValue._value;
        return true;
    }

    template<>
    inline bool Serializable<GizmoLayer>::serializeJson(const GizmoLayer& itemRef, json& jsonF) {
        jsonF = itemRef._value;
        return true;
    }
#pragma endregion
}