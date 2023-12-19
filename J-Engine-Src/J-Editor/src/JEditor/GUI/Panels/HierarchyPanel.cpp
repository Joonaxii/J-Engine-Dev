#include <JEditor/GUI/Panels/HierarchyPanel.h>
#include <JEditor/JEditor.h>

namespace JEngine::Editor {
    HierarchyPanel::HierarchyPanel(const char* title, Alignment align) : IDockPanel(title, align) { }

    void HierarchyPanel::draw(Application* app) {
        static bool selected[4]{ false };
        char temp[65]{ 0 };

        if (app) {
            ImGui::Text("<Scene Name Here>");
            auto avail = ImGui::GetContentRegionAvail();
            ImGui::BeginChild("##Hierarchy", avail,false, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration);

            bool isShift = Input::isHeld<JEditor::INPUT_SYS_EDITOR>(Input::INP_LCtrl);
            for (uint32_t i = 0; i < 4; i++) {
                ImGui::PushID(i);
                sprintf_s(temp, "GameObject #%i", i);
                if (ImGui::Selectable(temp, selected + i)) {
                    if (!isShift) {
                        bool cache = selected[i];
                        memset(selected, 0, sizeof(selected));
                        selected[i] = cache;
                    }
                }
                ImGui::PopID();
            }
            ImGui::EndChild();
        }
    }
}