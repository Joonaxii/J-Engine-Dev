#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <deque>
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>
#include <JEngine/IO/Helpers/IOUtils.h>

namespace JEngine {
    namespace Editor {
        struct EditorColors {
            enum Colors : int32_t {
                Color_COUNT,

                Color_TOT_COUNT = ImGuiCol_COUNT + Color_COUNT,
            };
            JColor32 colors[Color_TOT_COUNT];
            std::string name{};

            void loadFromStream(const Stream& stream) {
                name = stream.readString();
                uint16_t count{};
                stream.readValue(count, false);
                count = std::min<uint16_t>(count, Color_TOT_COUNT);
                stream.read(colors, sizeof(JColor32) * count, false);
            }

            void writeToStream(const Stream& stream) const {
                stream.writeString(name);
                stream.writeValue<uint16_t>(Color_TOT_COUNT);
                stream.writeValue(colors);
            }

            void copyto(EditorColors& other, bool copyName = true) {
                memcpy(other.colors, colors, sizeof(colors));
                if (copyName) {
                    other.name = this->name;
                }
            }
        };

        struct EditorLayout {
            std::string name{};

            void load(std::string_view root) const {
                std::string path = IO::combine(root, name).append(".ini");
                if (IO::exists(path)) {
                    ImGui::LoadIniSettingsFromDisk(path.c_str());
                }
            }

            void saveCurrent(std::string_view root) const {
                std::string path = IO::combine(root, name).append(".ini");
                ImGui::SaveIniSettingsToDisk(path.c_str());
            }
        };

        class MainWindow : public IGuiPanel {
        public:
            MainWindow(Application* application);
            ~MainWindow();
            bool initGui() override;
            void drawGui() override;

            std::vector<IDockPanel*>& getPanels() { return _panels; }
            const std::vector<IDockPanel*>& getPanels() const { return _panels; }

            template<typename T>
            T* findPanelOfType() const {
                for (auto panel : _panels) {
                    T* temp = dynamic_cast<T*>(panel);
                    if (temp) { return temp; }
                }
                return nullptr;
            }

            std::vector<EditorColors>& getEditorColors() { return _colors; }
            std::vector<EditorLayout>& getEditorLayouts() { return _layouts; }

            const std::vector<EditorColors>& getEditorColors() const { return _colors; }
            const std::vector<EditorLayout>& getEditorLayouts() const { return _layouts; }

            void loadColors();

            void loadLayouts();
            void makeCurrentLayout();

            void restoreDefault();
            void resetDockLayout();

            void addPanel(IDockPanel* panel);
            IDockPanel* removePanel(IDockPanel* panel);

            bool isEditorFocused() const;

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

            std::vector<EditorColors> _colors;
            std::vector<EditorLayout> _layouts;

            std::string _layoutsPath;

            void drawEditorSettings(bool& open);
        };
    }
}