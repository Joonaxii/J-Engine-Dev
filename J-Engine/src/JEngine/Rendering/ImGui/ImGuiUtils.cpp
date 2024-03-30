#include <JEngine/Rendering/ImGui/ImGuiUtils.h>

namespace JEngine {
    namespace Gui {
        bool drawBitMask(std::string_view label, void* value, size_t size, uint64_t start, uint64_t length, Enum::GetEnumName nameFunc, bool allowMultiple, bool displayAll) {
            size = Math::min(size, 8ULL); 
            uint64_t bitCount = (size << 3);
            length = Math::min(length, bitCount - start);
            if (length <= 0) { return false; }

            uint64_t bits = 0;
            JE_COPY(&bits, value, size);

            bool changed = false;
            char temp[257]{ 0 };
            uint64_t tempL = 0;
            uint64_t bitsSet = 0;
            bool wasSet = false;
            for (uint64_t i = 0, j = 1ULL << start; i < length; i++, j <<= 1) {
                if (tempL >= 256) { break; }
                if (bits & j) {
                    if (!wasSet) {
                        bitsSet++;
                        wasSet = !allowMultiple;

                        if (tempL > 0) {
                            JE_COPY(temp + tempL, ", ", 2);
                            tempL += 2;
                        }
                        std::string_view name = nameFunc(&j);
                        JE_COPY(temp + tempL, name.data(), name.length());
                        tempL += name.length();
                    } else { changed |= true; }
                }
            }

            if (tempL == 0) {
                sprintf_s(temp, "None");
            }
            else if (bitsSet == length && displayAll) {
                sprintf_s(temp, "Everything");
            }

            uint64_t mask = ((1ULL << length) - 1) << start;
            if (ImGui::BeginCombo(label, temp, ImGuiComboFlags_HeightLarge)) {
                ImGui::Indent();
                ImGui::PushID("Elements");

                for (uint64_t i = 0, j = 1ULL << start; i < length; i++, j <<= 1) {
                    bool tempBool = (bits & j) != 0;

                    ImGui::PushID(&i);
                    if (ImGui::Checkbox(nameFunc(&j), &tempBool)) {
                        changed = true;
                        if (tempBool) {
                            if (!allowMultiple) {
                                uint64_t negate = (mask & ~j);
                                bits &= ~negate;
                            }
                            bits |= j;
                        } else { bits &= ~j; }
                    }
                    ImGui::PopID();
                }

                ImGui::PopID();
                ImGui::Unindent();
                ImGui::EndCombo();
            }

            if (changed) {
                JE_COPY(value, &bits, size);
            }
            return changed;
        }

        bool drawDropdown(std::string_view label, void* value, size_t size, uint64_t start, uint64_t length, Enum::GetEnumName nameFunc) {
            bool changed = false;
            uint64_t selectI = 0;
            size = Math::min(size, sizeof(uint64_t));
            JE_COPY(&selectI, value, size);

            if (ImGui::BeginCombo(label, nameFunc(&selectI)))
            {
                for (uint64_t i = 0, j = start; i < length; i++, j++) {
                    std::string_view name = nameFunc(&j);
                    if (name.length() < 1) { continue; }

                    ImGui::PushID(&i);
                    if (ImGui::Selectable(name, bool(j == selectI), 0)) {
                        selectI = j;
                        JE_COPY(value, &selectI, size);
                        changed = true;
                    }
                    ImGui::PopID();
                }
                ImGui::EndCombo();
            }
            return changed;
        }
    }
}