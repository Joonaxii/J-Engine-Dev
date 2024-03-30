#pragma once
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>
#include <JEngine/Core/GameObject.h>
#include <unordered_set>

namespace JEngine::Editor {
    class HierarchyPanel : public IDockPanel {
    public:
        HierarchyPanel(const char* title, Alignment align);
        void draw(Application* app) override;

        void init(Application* app) override {}
        void close(Application* app) override {}

    private:
        std::unordered_set<GameObject*> _selection;
    };
}