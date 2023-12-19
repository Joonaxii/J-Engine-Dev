#include <Editor/Core/EditorEssentials.h>
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>
#include <JEngine/Rendering/SortingLayer.h>
#include <JEngine/Core/LayerMask.h>

namespace JEngine {
    namespace Gui {
        bool drawScalar(const char* label, bool isSlider, bool isAngle, int32_t dataType, void* value, float speed, const void* min, const void* max, const char* format, int32_t flags) { 
            if (isAngle && dataType == ImGuiDataType_Float) {
                return ImGui::SliderAngle(label, reinterpret_cast<float*>(value), min ? *reinterpret_cast<const float*>(min), max ? *reinterpret_cast<const float*>(max), format, flags);
            }

            return isSlider ? ImGui::SliderScalar(label, dataType, value, min, max, format, flags) :
                ImGui::DragScalar(label, dataType, value, speed, min, max, format, flags);
        }
        bool drawScalar(const char* label, int32_t dataType, void* value, const GUIStyle& style) {
            return drawScalar(label, style.has(GUIStyle::STYLE_SLIDER), style.has(GUIStyle::FLAG_ANGLE), dataType, value, 1.0f, style.getMin(), style.getMax(), style.format, style.has(GUIStyle::FLAG_MIN | GUIStyle::FLAG_MAX) ? ImGuiSliderFlags_AlwaysClamp : ImGuiSliderFlags_None);
        }

        bool drawScalars(const char* label, bool isSlider, int32_t dataType, void* value, int32_t count, float speed, const void* min, const void* max, const char* format, int32_t flags) {
            return
                isSlider ?
                ImGui::SliderScalarN(label, dataType, value, count, min, max, format, flags) :
                ImGui::DragScalarN(label, dataType, value, count, speed, min, max, format, flags);
        }
        bool drawScalars(const char* label, int32_t dataType, void* value, int32_t count, const GUIStyle& style) {
            return drawScalars(label, style.has(GUIStyle::STYLE_SLIDER), dataType, value, count, 1.0f, style.getMin(), style.getMax(), style.format, style.has(GUIStyle::FLAG_MIN | GUIStyle::FLAG_MAX) ? ImGuiSliderFlags_AlwaysClamp : ImGuiSliderFlags_None);
        }


        template<int32_t size, int32_t elementSize>
        static bool drawVector(const char* label, int32_t dataType, void* value, const GUIStyle& style, int32_t offset = 0) {
            static_assert(size > 1 && size < 5, "Vector size must be between 2 and 4!");
            static_assert(elementSize > 0, "Vector Element size cannot be less than 1!");
            if (!value) { return false; }

            static constexpr char const* axes[4]{
                "X","Y","Z","W"
            };
            static constexpr char const* axisID[4]{
              "##X","##Y","##Z","##W"
            };
            static constexpr ImVec4 colors[8]{
                ImVec4(0.85f, 0.075f, 0.0f, 1.0f),
                ImVec4(0.075f, 0.85f, 0.0f, 1.0f),
                ImVec4(0.0f, 0.075f, 0.85f, 1.0f),
                ImVec4(0.65f, 0.65f, 0.125f, 1.0f),

                ImVec4(0.95f, 0.095f, 0.0f, 1.0f),
                ImVec4(0.095f, 0.95f, 0.0f, 1.0f),
                ImVec4(0.0f, 0.095f, 0.95f, 1.0f),
                ImVec4(0.75f, 0.75f, 0.135f, 1.0f),
            };
            static constexpr float BUFFER = 2.0f;
            static constexpr float LABEL = 0.45f;
            float avail = ImGui::GetContentRegionAvail().x;
            float labelW = (avail * LABEL) + BUFFER;
            float elemW = std::max(avail - labelW, 0.0f);;

            bool hasMin = style.is(GUIStyle::FLAG_MIN);
            bool hasMax = style.is(GUIStyle::FLAG_MAX);

            uint8_t* valU = reinterpret_cast<uint8_t*>(value) + offset;
            const uint8_t* minU = hasMin ? reinterpret_cast<const uint8_t*>(style.getMin()) + offset : nullptr;
            const uint8_t* maxU = hasMax ? reinterpret_cast<const uint8_t*>(style.getMax()) + offset : nullptr;
            const uint8_t* defU = reinterpret_cast<const uint8_t*>(style.getDefaultValue()) + offset;

            bool isRange = style.is(GUIStyle::STYLE_SLIDER);
            bool isAngle = style.is(GUIStyle::FLAG_ANGLE);
            ImGuiSliderFlags scFlags = style.has(GUIStyle::FLAG_MIN | GUIStyle::FLAG_MAX) ? ImGuiSliderFlags_AlwaysClamp : ImGuiSliderFlags_None;

            ImGui::PushID(label);
            bool changed = false;
            if (style.is(GUIStyle::FLAG_TRANSFORM)) {
                ImGui::Text(label);
                for (size_t i = 0; i < size; i++) {
                    ImGui::SameLine();
                    changed |= drawScalar(axisID[i], isRange, isAngle, dataType, valU, 1.0f, minU, maxU, style.format, scFlags);

                    if (hasMin) { minU += elementSize; }
                    if (hasMax) { maxU += elementSize; }
                    defU += elementSize;
                    valU += elementSize;
                }
                ImGui::PopID();
                return changed;
            }

            ImGui::Columns(2);
            ImGui::Text(label);

            ImGui::NextColumn();

            ImGui::PushMultiItemsWidths(size, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, 0.15f);

            auto dList = ImGui::GetDrawListSharedData();
            uint8_t tmp[8]{ 0 };

            float height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 btnSize = { height + 3.0f, height };
            for (int32_t i = 0; i < size; i++) {
                ImGui::PushID(i);

                if (i != 0) {
                    ImGui::SameLine();
                }

                ImGui::PushStyleColor(ImGuiCol_Button, colors[i]);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors[i + 4]);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors[i]);
                if (ImGui::Button(axes[i], btnSize)) {
                    if (memcmp(valU, defU, elementSize) != 0) {
                        memcpy(valU, defU, elementSize);
                        changed |= true;
                    }
                }
                ImGui::PopStyleColor(3);

                ImGui::SameLine();
                changed |= drawScalar(axisID[i], isRange, isAngle, dataType, valU, 1.0f, minU, maxU, style.format, scFlags);
                ImGui::PopItemWidth();
                ImGui::PopID();

                if (hasMin) { minU += elementSize; }
                if (hasMax) { maxU += elementSize; }
                defU += elementSize;
                valU += elementSize;
            }
            ImGui::Columns(1);

            ImGui::PopID();
            return changed;
        }

        template<int32_t size, int32_t elementSize>
        static bool drawRect(const char* label, ImGuiDataType dataType, void* value, const GUIStyle& style) {
            static_assert(size > 1 && size < 5, "Vector size must be between 2 and 4!");
            static_assert(elementSize > 0, "Vector Element size cannot be less than 1!");

            bool changed = false;
            ImGui::PushID(label);
            static constexpr int32_t ONE_CH = size * elementSize;
            if (ImGui::CollapsingHeader(label)) {
                ImGui::Indent();
                changed |= drawVector<size, elementSize>("Min", dataType, value, style, 0);
                changed |= drawVector<size, elementSize>("Max", dataType, value, style, ONE_CH);
                ImGui::Unindent();
            }
            ImGui::PopID();
            return changed;
        }

        bool drawColor(const char* label, float color[4], bool hasAlpha, const GUIStyle& style) {
            const ImGuiColorEditFlags flags = style.is(GUIStyle::FLAG_HDR) ? ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float : ImGuiColorEditFlags_None;

            if (style.is(GUIStyle::FLAG_PICKER)) {
                return hasAlpha ?
                    ImGui::ColorPicker4(label, color, flags) :
                    ImGui::ColorPicker3(label, color, flags);
            }

            return hasAlpha ?
                ImGui::ColorEdit4(label, color, flags) :
                ImGui::ColorEdit3(label, color, flags);
        }

        bool drawColor(const char* label, ValueType colorType, void* value, const GUIStyle& style) {
            float tmp[4]{ 0.0f, 0.0f, 0.0f, 1.0f };
            uint8_t* data = reinterpret_cast<uint8_t*>(value);
            bool hasAlpha = false;
            switch (colorType) {
                case JEngine::ValueType::VTYPE_COLOR32: {
                    tmp[3] = float(data[3]) / 255.0f;
                    hasAlpha = true;
                    __fallthrough;
                case JEngine::ValueType::VTYPE_COLOR24:
                    tmp[0] = float(data[0]) / 255.0f;
                    tmp[1] = float(data[1]) / 255.0f;
                    tmp[2] = float(data[2]) / 255.0f;
                    bool changed = drawColor(label, tmp, hasAlpha, style);
                    if (changed) {
                        data[0] = uint8_t(tmp[0] * 255.0f);
                        data[1] = uint8_t(tmp[1] * 255.0f);
                        data[2] = uint8_t(tmp[2] * 255.0f);
                        if (hasAlpha) {
                            data[3] = uint8_t(tmp[3] * 255.0f);
                        }
                    }
                    return changed;
                }
                case JEngine::ValueType::VTYPE_COLOR:
                    return drawColor(label, reinterpret_cast<float*>(value), true, style);
            }
            return false;
        }

        bool drawLayerMask(const char* label, void* value, const GUIStyle& style) {
            static const char* const* LayerNames{ reinterpret_cast<const char* const*>(LayerMask::getLayerNames()) };
            return Gui::drawBitMask(label, *reinterpret_cast<uint32_t*>(value), 0, 32, LayerNames, !style.is(GUIStyle::FLAG_SELECT_SINGLE));
        }

        bool drawSortingLayer(const char* label, void* value, const GUIStyle& style) {
            bool changed = false;
            SortingLayer& sortLayer = *reinterpret_cast<SortingLayer*>(value);
            uint16_t layer = sortLayer.getLayerIndex();
            int16_t order = int16_t(sortLayer.getOrder());

            ImGui::PushID(label);
            if (ImGui::CollapsingHeader(label)) {
                ImGui::Indent();
                if (Gui::drawDropdown("Layer##SortingLayer", layer, SortingLayer::getLayers().data(), SortingLayer::getLayerCount())) {
                    sortLayer.setLayerByIndex(layer);
                    changed = true;
                }

                if (ImGui::DragScalar("Order##SortingLayer", ImGuiDataType_S16, &order)) {
                    sortLayer.setOrder(order);
                    changed = true;
                }
                ImGui::Unindent();
            }
            ImGui::PopID();
            return changed;
        }

        bool drawBuiltIn(const char* label, ValueType type, void* data, const GUIStyle& style) {
            switch (type) {
                case ValueType::VTYPE_INT8:
                case ValueType::VTYPE_UINT8:
                case ValueType::VTYPE_INT16:
                case ValueType::VTYPE_UINT16:
                case ValueType::VTYPE_INT32:
                case ValueType::VTYPE_UINT32:
                case ValueType::VTYPE_INT64:
                case ValueType::VTYPE_UINT64:
                case ValueType::VTYPE_FLOAT:
                case ValueType::VTYPE_DOUBLE:
                    //Note: ImGuiDataType is in the same order for primitive types, except JEngine's ValueType starts at 1
                    //because 0 is reserved for VTYPE_NONE
                    return drawScalar(label, ImGuiDataType(uint32_t(type) - 1), data, style);

                case ValueType::VTYPE_VEC_2F:
                    return drawVector<2, sizeof(float)>(label, ImGuiDataType_Float, data, style);

                case ValueType::VTYPE_VEC_2I:
                    return drawVector<2, sizeof(int32_t)>(label, ImGuiDataType_S32, data, style);

                case ValueType::VTYPE_VEC_3F:
                    return drawVector<3, sizeof(float)>(label, ImGuiDataType_Float, data, style);

                case ValueType::VTYPE_VEC_3I:
                    return drawVector<3, sizeof(int32_t)>(label, ImGuiDataType_S32, data, style);

                case ValueType::VTYPE_VEC_4F:
                    return drawVector<4, sizeof(float)>(label, ImGuiDataType_Float, data, style);

                case ValueType::VTYPE_VEC_4I:
                    return drawVector<4, sizeof(int32_t)>(label, ImGuiDataType_S32, data, style);

                case ValueType::VTYPE_COLOR:
                case ValueType::VTYPE_COLOR24:
                case ValueType::VTYPE_COLOR32:
                    return drawColor(label, type, data, style);

                case ValueType::VTYPE_GAME_OBJECT_REF:
                case ValueType::VTYPE_COMPONENT_REF:
                case ValueType::VTYPE_ASSET_REF:
                    //TODO: Somehow implement GUI for References
                    //Should allow drag & drop and selecting via a menu/popup
                    break;

                case ValueType::VTYPE_RECTF:
                    return drawRect<2, sizeof(float)>(label, ImGuiDataType_Float, data, style);
                case ValueType::VTYPE_RECTI:
                    return drawRect<2, sizeof(int32_t)>(label, ImGuiDataType_S32, data, style);

                case ValueType::VTYPE_LAYER_MASK:
                    return drawLayerMask(label, data, style);

                case ValueType::VTYPE_SORTING_LAYER:
                    return drawSortingLayer(label, data, style);
            }
            return false;
        }


    }
}