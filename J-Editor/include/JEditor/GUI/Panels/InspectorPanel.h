#pragma once
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>

namespace JEngine::Editor {
    class InspectorPanel : public IDockPanel {
    public:
        InspectorPanel(const char* title, Alignment align);
        void draw(Application* app) override;

        void init(Application* app) override {}
        void close(Application* app) override {}
    };
}