#pragma once
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>

namespace JEngine::Editor {
    class ProjectPanel : public IDockPanel {
    public:
        ProjectPanel(const char* title, Alignment align);
        void init(Application* app) override;
        void draw(Application* app) override;
        void close(Application* app) override;
    };
}