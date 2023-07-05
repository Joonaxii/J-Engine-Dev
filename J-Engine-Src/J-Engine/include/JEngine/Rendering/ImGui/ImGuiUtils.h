#pragma once
#include <imgui_internal.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Core/Application.h>

namespace JEngine {
    class IDockPanel {
    public:
        enum : uint8_t {
            FLAG_IS_INITIALIZED = 0x1,
        };
        enum : uint8_t {
            AL_None,
            AL_Left,
            AL_Right,
            AL_Up,
            AL_Down,
        };

        struct Alignment {
            uint8_t direction{ AL_None };
            float coverage{ 0.25f };
            uint16_t target{ 0 };
        };

        IDockPanel(const char* title, Alignment alignment) : _title(title), _alignment(alignment), _flags(0), _minSize{ 320, 180 }, _maxSize{-1, -1}{}
        IDockPanel(const char* title, Alignment alignment, const ImVec2& min, const ImVec2& max) : _title(title), _alignment(alignment), _flags(0), _minSize(min), _maxSize(max){}

        virtual Alignment getAlignment() const { return _alignment; }
        const char* getTitle() const { return _title.c_str(); }

        virtual bool initialize(ImGuiID dockspaceID) {
            if (isInitialized()) { return false; }
            _flags |= FLAG_IS_INITIALIZED;
            return true;
        }
        bool isInitialized() const { return _flags.isBitSet(FLAG_IS_INITIALIZED); }

        virtual void init(Application* app) = 0;
        virtual void draw(Application* app) = 0;
        virtual void close(Application* app) = 0;

        void setMinSize(const ImVec2& value) { _minSize = value; }
        void setMaxSize(const ImVec2& value) { _maxSize = value; }

        void setMinSize(float x, float y) { _minSize = {x, y}; }
        void setMaxSize(float x, float y) { _maxSize = {x, y}; }

        const ImVec2& getMinSize() const { return _minSize; }
        const ImVec2& getMaxSize() const { return _maxSize; }

    private:
        Alignment _alignment;
        std::string _title;
        UI8Flags _flags;
        ImVec2 _minSize;
        ImVec2 _maxSize;
    };

    class IGuiPanel {
    public:
        enum : uint8_t {
            FLAG_IS_INITIALIZED = 0x1,
        };

        IGuiPanel() : _flags(0x0) {}

        bool isInitialized() const { return _flags.isBitSet(FLAG_IS_INITIALIZED); }

        virtual bool initGui() {
            if (isInitialized()) { return false; }
            _flags |= FLAG_IS_INITIALIZED;
            return true;
        }
        virtual void drawGui() = 0;
    private:
        UI8Flags _flags;
    };

    static constexpr ImGuiDir_ alignmentToDir(IDockPanel::Alignment alignment) {
        switch (alignment.direction) {
            default: return ImGuiDir_None;
            case IDockPanel::AL_Down: return ImGuiDir_Down;
            case IDockPanel::AL_Up: return ImGuiDir_Up;
            case IDockPanel::AL_Right: return ImGuiDir_Right;
            case IDockPanel::AL_Left: return ImGuiDir_Left;
        }
    }

    namespace Gui {
        static void setDockOrder(const char* windowName, short order) {
            auto windowId = ImHashStr(windowName);

            if (ImGuiWindow* win = ImGui::FindWindowByID(windowId)) {
                win->DockOrder = order;
                return;
            }

            if (ImGuiWindowSettings* settings = ImGui::FindWindowSettingsByID(windowId)) {
                settings->DockOrder = order;
                return;
            }
        }

        static void setFocused(ImGuiID dockId, const char* window) {
            if (ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockId)) {
                auto windowId = ImHashStr(window);
                node->SelectedTabId = windowId;
            }
        }

        static ImGuiDockNode* getDockNode(const char* windowName) {
            ImGuiWindow* window = ImGui::FindWindowByName(windowName);

            if (window) {
                return window->DockNode;
            }

            auto nameId = ImHashStr(windowName);
            if (ImGuiWindowSettings* settings = ImGui::FindWindowSettingsByID(nameId)) {
                return ImGui::DockBuilderGetNode(settings->DockId);
            }
            return nullptr;
        }

        static void focusWindow(const char* windowName) {
            ImGuiWindow* window = ImGui::FindWindowByName(windowName);

            if (window == NULL) {
                auto name = ImHashStr(windowName);
                ImGuiWindowSettings* settings = ImGui::FindWindowSettingsByID(name);

                if (settings == NULL) { return; }
                settings->DockOrder = -1;
                return;
            }

            if (window->DockNode == NULL || window->DockNode->TabBar == NULL) {
                return;
            }
            window->DockNode->TabBar->NextSelectedTabId = window->TabId;
        }

        static void focusWindow(ImGuiID windowId) {
            ImGuiWindow* window = ImGui::FindWindowByID(windowId);
            if (window->DockNode == NULL || window->DockNode->TabBar == NULL) {
                return;
            }
            window->DockNode->TabBar->NextSelectedTabId = window->TabId;
        }
    }
}