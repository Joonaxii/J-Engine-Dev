#pragma once
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>

namespace JEngine::Editor {
    class SceneViewPanel : public IDockPanel {
    public:
        SceneViewPanel(const char* title, Alignment align);
        void draw(Application* app) override;

        void init(Application* app) override {}
        void close(Application* app) override {}
    };
}