#pragma once
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>

namespace JEngine::Editor {
    class ConsolePanel : public IDockPanel {
    public:
        ConsolePanel(const char* title, Alignment align);
        void init(Application* app) override;
        void draw(Application* app) override;
        void close(Application* app) override;
    };
}