#include <JEditor/GUI/MainWindow.h>
#include <JEditor/JEditor.h>
#include <JEditor/GUI/Panels/GameViewPanel.h>
#include <JEditor/GUI/Panels/SceneViewPanel.h>
#include <JEditor/GUI/Panels/HierarchyPanel.h>
#include <JEditor/GUI/Panels/InspectorPanel.h>
#include <JEditor/GUI/Panels/ProjectPanel.h>
#include <JEditor/GUI/Panels/ConsolePanel.h>
#include <algorithm>

namespace JEngine::Editor {
    static constexpr const char EDITOR_SETTINGS_TITLE[] = "Editor Settings";


    static constexpr ImGuiDockNodeFlags DOCKSPACE_FLAGS = ImGuiDockNodeFlags_PassthruCentralNode;
    /*  static constexpr const char* getMainPanelName(MainPanelType panel) {
          switch (panel) {
              default: return "Unknown";
              case P_Hierarchy: return "Hierarchy";
              case P_Inspector: return "Inspector";
              case P_Project: return "Project";
              case P_GameView: return "Game View";
          }
      }*/


    MainWindow::MainWindow(Application* application) : IGuiPanel(), _app(application), _panels{}, _dockID(), _panelPostInfo{}, _layoutsPath(std::string(JEDITOR_USER_DATA_PATH) + "/Layouts") {}

    MainWindow::~MainWindow() {
        for (auto panel : _panels) {
            delete panel;
        }
        _panels.clear();
    }

    bool MainWindow::initGui() {
        bool isInit = isInitialized();
        if (IGuiPanel::initGui()) {
            restoreDefault();
            return true;
        }
        return false;
    }

    void MainWindow::drawGui() {
        if (!_app) { return; }

        enum : uint8_t {
            FOCUS_EDITOR_SETTINGS = 0x1,
        };

        static bool editorSettings{false};
        uint8_t focusFlags{ 0 };

        auto& rend = _app->getRenderer();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImVec2 pos;
        ImVec2 size;
        //Main Dockspace
        {
            pos = viewport->Pos;
            size = viewport->Size;

            ImGui::SetNextWindowPos(pos);
            ImGui::SetNextWindowSize(size);
            ImGui::SetNextWindowViewport(viewport->ID);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("##Main", nullptr, window_flags);
            ImGui::PopStyleVar();
            ImGui::PopStyleVar(2);
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    static bool opened = false;
                    if (ImGui::MenuItem("Open", "", false, true)) {}
                    if (ImGui::MenuItem("Close", "", false, opened)) { opened = false; }
                    if (ImGui::MenuItem("Quit", "", false, true)) {
                        _app->quit();
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit")) {
                    static bool opened = false;
                    if (ImGui::MenuItem("Editor Settings##Editor", "", false, true)) {
                        if (editorSettings) {
                            focusFlags |= FOCUS_EDITOR_SETTINGS;
                        }
                        editorSettings = true;
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMenuBar();

            //Setup dockspaces
            {
                static bool first_time = true;
                if (first_time || _flags.isBitSet(FLAG_RESET_DOCK)) {
                    first_time = false;
                    resetDockLayout();
                }
                ImGui::DockSpace(_dockID, ImVec2(0.0f, 0.0f), DOCKSPACE_FLAGS);
            }
            ImGui::End();
        }

        _flags.setBit(FLAG_DRAWING, true);
        static constexpr ImGuiWindowFlags_ VIEW_FLAGS = ImGuiWindowFlags_(ImGuiWindowFlags_NoCollapse);
        bool forceFocus = false;
        for (auto panel : _panels) {
            ImGui::PushID(panel);
            if (ImGui::Begin(panel->getTitle(), 0, VIEW_FLAGS)) {
                panel->setFocused(Gui::isCurrentWindowFocused(&forceFocus));
                auto dockNode = ImGui::GetWindowDockNode();
                if (!dockNode || !dockNode->ParentNode) {
                    auto size = ImGui::GetWindowSize();
                    auto& minS = panel->getMinSize();
                    auto& maxS = panel->getMaxSize();

                    size.x = minS.x > 0 ? std::max(minS.x, size.x) : size.x;
                    size.y = minS.y > 0 ? std::max(minS.y, size.y) : size.y;

                    size.x = maxS.x > 0 ? std::min(maxS.x, size.x) : size.x;
                    size.y = maxS.y > 0 ? std::min(maxS.y, size.y) : size.y;

                    if (dockNode) {
                        ImGui::DockBuilderSetNodeSize(dockNode->ID, size);
                    }
                    else {
                        ImGui::SetWindowSize(size);
                    }
                }
                panel->draw(_app);
            }
            else if(panel->isFocused()) {
                panel->setFocused(false);
            }
            ImGui::End();
            ImGui::PopID();
        }
        _flags.setBit(FLAG_DRAWING, false);

        _app->setForceFocus(forceFocus);

        if (_panelPostInfo.size()) {
            while (_panelPostInfo.size()) {
                PanelPostData& post = _panelPostInfo.back();
                Gui::focusWindow(post.focused);
                _panelPostInfo.pop_back();
            }
            //GImGui->SettingsLoaded = false;
        }

        if ((focusFlags & FOCUS_EDITOR_SETTINGS) && editorSettings) {
            ImGui::SetNextWindowFocus();
        }
        drawEditorSettings(editorSettings);

        while (!_panelRemoveQueue.empty()) {
            removePanel(_panelRemoveQueue.back());
            _panelRemoveQueue.pop_back();
        }

        while (!_panelAddQueue.empty()) {
            addPanel(_panelAddQueue.back());
            _panelAddQueue.pop_back();
        }
    }

    void MainWindow::restoreDefault() {
        for (auto panel : _panels) {
            removePanel(panel);
        }

        addPanel(new SceneViewPanel("Scene View", {                              }));
        addPanel(new GameViewPanel("Game View",  {IDockPanel::AL_None , 0.0f , 1}));
        addPanel(new ProjectPanel("Project",   {IDockPanel::AL_Down , 0.25f, 0}));
        //addPanel(new ConsolePanel("Console",   {IDockPanel::AL_None , 0.25f, 1}));
        addPanel(new HierarchyPanel("Hierarchy", {IDockPanel::AL_Left , 0.2f , 0}));
        addPanel(new InspectorPanel("Inspector", {IDockPanel::AL_Right, 0.25f, 0}));
        resetDockLayout();
    }

    void MainWindow::resetDockLayout() {
        if ((_flags & FLAG_DRAWING)) {
            _flags |= FLAG_RESET_DOCK;
            return;
        }

        if (_panels.size() < 1) { return; }

        struct DockID {
            ImGuiID id{0};
            short curDock{0};
        };

        DockID* idBuffer = reinterpret_cast<DockID*>(_malloca(sizeof(DockID) * _panels.size()));
        if (!idBuffer) { return; }

        _panelPostInfo.clear();

        IDockPanel* mainPanel = _panels[0];
        _dockID = ImGui::GetID("MainDock");
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        _flags.setBit(FLAG_RESET_DOCK, false);
        ImGui::DockBuilderRemoveNode(_dockID);
        ImGui::DockBuilderAddNode(_dockID, DOCKSPACE_FLAGS | ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(_dockID, viewport->Size);

        idBuffer[0] = { _dockID , INT16_MIN };
        ImGui::DockBuilderDockWindow(mainPanel->getTitle(), _dockID);
        Gui::setDockOrder(mainPanel->getTitle(), INT16_MIN);

        ImGuiID idD = _dockID;
        for (size_t i = 1; i < _panels.size(); i++) {
            auto align = _panels[i]->getAlignment();
            if (align.direction == IDockPanel::AL_None) {
                if (align.target) {
                    auto& dockID = idBuffer[i - align.target];
                    ImGui::DockBuilderDockWindow(_panels[i]->getTitle(), dockID.id);
                    Gui::setDockOrder(_panels[i]->getTitle(), ++dockID.id);
                    _panelPostInfo.push_back(PanelPostData(dockID.id, ImHashStr(_panels[i - align.target]->getTitle())));
                }
                continue; 
            }
            idBuffer[i] = { ImGui::DockBuilderSplitNode(idD, alignmentToDir(align), align.coverage, nullptr, &idD), INT16_MIN };
            ImGui::DockBuilderDockWindow(_panels[i]->getTitle(), idBuffer[i].id);
            Gui::setDockOrder(_panels[i]->getTitle(), INT16_MIN);
        }
        ImGui::DockBuilderFinish(_dockID);

        _freea(idBuffer);
        //GImGui->SettingsLoaded = false;
    }

    void MainWindow::addPanel(IDockPanel* panel) {
        if (!panel) { return; }
        if (_flags & FLAG_DRAWING) {
            _panelAddQueue.push_back(panel);
            return;
        }
        if (std::find(_panels.begin(), _panels.end(), panel) == _panels.end()) {
            _panels.push_back(panel);
            _panelInitQueue.push_back(panel);
            panel->init(_app);
            _flags |= FLAG_RESET_DOCK;
        }
    }

    IDockPanel* MainWindow::removePanel(IDockPanel* panel) {
        if (!panel) { return false; }

        auto find = std::find(_panels.begin(), _panels.end(), panel);
        bool found = find != _panels.end();
        if (_flags & FLAG_DRAWING) {
            if (found) {
                _panelRemoveQueue.push_back(panel);
                return nullptr;
            }
            return panel;
        }

        if (found) {
            _panels.erase(find);
            panel->close(_app);
            delete panel;
            return nullptr;
        }
        return panel;
    }

    bool MainWindow::isEditorFocused() const {
        GameViewPanel* gameView = findPanelOfType<GameViewPanel>();
        if (gameView) {
            return !gameView->isFocused();
        }
        return true;
    }

    void MainWindow::drawEditorSettings(bool& open) {
        if (!open) { return; }

        enum : uint32_t {
            MENU_General,
            MENU_EditorVisuals,
            MENU_COUNT,
        };

        static constexpr const char* OPTIONS[MENU_COUNT]{
            "General",
            "Editor Visuals",
        };
        static uint32_t selOpt = 0;

        bool wasOpen = open;
        ImGui::PushID(EDITOR_SETTINGS_TITLE);


        ImGui::SetNextWindowSize({ 750.0f, 600.0f }, ImGuiCond_Appearing);
        ImGui::SetNextWindowPos({ 100.0f, 100.0f }, ImGuiCond_Appearing);
        if (ImGui::Begin(EDITOR_SETTINGS_TITLE, &open, ImGuiWindowFlags_MenuBar)) {
            if (ImGui::BeginMenuBar()) {
                bool selected = false;
                for (uint32_t i = 0; i < MENU_COUNT; i++) {
                    ImGui::PushID(i);
                    selected = i == selOpt;
                    if (ImGui::MenuItem(OPTIONS[i], "", selected)) {
                        selOpt = i;
                    }
                    ImGui::PopID();
                }
                ImGui::EndMenuBar();
            }

            if (ImGui::BeginChild("##SettingsArea", {0, 0}, ImGuiChildFlags_Border)) {
                //TODO: Handle settings...
            }
            ImGui::EndChild();
        }

        if (wasOpen && !open) {
            auto window = ImGui::GetCurrentWindow();
            ImGui::DockContextProcessUndockWindow(ImGui::GetCurrentContext(), window);
        }

        ImGui::End();
        ImGui::PopID();

    }
}