#include <JEngine/Core/LayerMask.h>
#include <JEngine/Core/Assert.h>
#include <JEngine/Utility/StringHelpers.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
    LayerMask::LayerName LayerMask::LayerNames[32]{
        LayerMask::LayerName("Default")
    };

    LayerMask LayerMask::nameToLayer(std::string_view name) {
        for (uint32_t i = 0; i < 32; i++) {
            if (Helpers::strEquals(name, std::string_view(LayerNames[i].buffer))) {
                return LayerMask(1U << i);
            }
        }
        return LayerMask();
    }

    std::string_view LayerMask::layerToName(LayerMask layer) {
        if (Math::isPowerOf2<uint32_t>(layer)) {
            return std::string_view(LayerNames[Math::findFirstLSB(layer)].buffer);
        }
        return std::string_view();
    }

    void LayerMask::setLayerName(int32_t index, std::string_view newName) {
        JENGINE_CORE_ASSERT(index >= 0 && index < 32, "Layer index out of range!");
        size_t len = std::min(newName.length(), MAX_NAME_LEN);
        memcpy(LayerNames[index].buffer, newName.data(), len);
        LayerNames[index].buffer[len] = 0;
    }

    std::string_view LayerMask::getLayerName(int32_t index) {
        JENGINE_CORE_ASSERT(index >= 0 && index < 32, "Layer index out of range!");
        return std::string_view(LayerNames[index].buffer);
    }
}