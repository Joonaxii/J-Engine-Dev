#pragma once
#include <string>
#include <vector>
#include <deque>
#include <JEngine/Core/Application.h>
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>

namespace JEngine::Editor {
    class MainWindow : public IGuiPanel {
    public:

        MainWindow(Application* application);
        ~MainWindow();
        bool initGui() override;
        void drawGui() override;

        std::vector<IDockPanel*>& getPanels() { return _panels; }
        const std::vector<IDockPanel*>& getPanels() const { return _panels; }

        void restoreDefault();

        void resetDockLayout();

        void addPanel(IDockPanel* panel);
        IDockPanel* removePanel(IDockPanel* panel);

    private:
        static constexpr uint8_t FLAG_DRAWING = 0x1;
        static constexpr uint8_t FLAG_RESET_DOCK = 0x2;

        struct PanelPostData {
            ImGuiID dockId{ 0 };
            ImGuiID focused{ 0 };

            PanelPostData() : dockId(), focused() {}
            PanelPostData(ImGuiID dockId, ImGuiID focus) : dockId(dockId), focused(focus) {}
        };

        UI8Flags _flags;
        ImGuiID _dockID;

        Application* _app;

        std::vector<IDockPanel*> _panels;
        std::deque<IDockPanel*> _panelInitQueue;
        std::deque<IDockPanel*> _panelAddQueue;
        std::deque<IDockPanel*> _panelRemoveQueue;
        std::deque<PanelPostData> _panelPostInfo;
    };
}