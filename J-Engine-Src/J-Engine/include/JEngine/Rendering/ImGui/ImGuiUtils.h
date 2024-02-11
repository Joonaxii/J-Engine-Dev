#pragma once
#include <GUI/ImGui.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Utility/EnumUtils.h>
#include <JEngine/Rendering/Window.h>

namespace JEngine {
    class Application;
    class IDockPanel {
    public:
        enum : uint8_t {
            FLAG_IS_INITIALIZED = 0x1,
            FLAG_IS_FOCUSED = 0x2,
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
        void setFocused(bool focused) { _flags.setBit(FLAG_IS_FOCUSED, focused); }
        bool isFocused() const { return _flags.isBitSet(FLAG_IS_FOCUSED); }

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
        static bool isCurrentWindowFocused(bool* forceFocus = nullptr, const void* mainWindow = nullptr) {
            auto win = ImGui::GetCurrentWindow();
            if (!win || !win->Active || win->Hidden) { return false; }

            //Check if this is a floating sub window
            if (win->Viewport != ImGui::GetMainViewport()) {
                if (forceFocus && win->Viewport && win->Viewport->PlatformHandle) {
                    *forceFocus |= Window::isOSWindowFocused(win->Viewport->PlatformHandle);
                }
                return ImGui::IsWindowFocused(ImGuiHoveredFlags_RootWindow);
            }
            return ImGui::IsWindowFocused(ImGuiHoveredFlags_RootWindow);
        }
    

        template<typename T>
        bool drawBitMask(const char* label, T& value, int32_t start, int32_t length, const char* const* names, bool allowMultiple = true, bool displayAll = true) {
            static constexpr size_t BITS = (sizeof(T) << 3);
            length = std::min<int32_t>(length, BITS - start);
            if (length <= 0) { return false; }
            uint64_t bits = uint64_t(value);

            bool changed = false;
            char temp[257]{ 0 };
            bool tempToggle[64]{ 0 };
            uint64_t tempL = 0;
            uint64_t bitsSet = 0;
            bool wasSet = false;
            for (uint64_t i = 0, j = 1ULL << start; i < length; i++, j <<= 1) {
                if (tempL >= 256) { break; }
                if (bits & j) {
                    if (!wasSet) {
                        bitsSet++;
                        tempToggle[start + i] = true;
                        wasSet = !allowMultiple;

                        if (tempL > 0) {
                            memcpy_s(temp + tempL, 256 - tempL, ", ", 2);
                            tempL += 2;
                        }
                        const char* name = names[i];
                        size_t len = strlen(name);

                        memcpy_s(temp + tempL, 256 - tempL, name, len);
                        tempL += len;
                    }
                    else {
                        changed |= true;
                    }
                }
            }

            if (tempL == 0) {
                sprintf_s(temp, "None");
            }
            else if (bitsSet == length && displayAll) {
                sprintf_s(temp, "Everything");
            }

            uint64_t mask = ((1ULL << length) - 1) << start;
            if (ImGui::BeginCombo(label, temp, ImGuiComboFlags_HeightLarge)) {
                ImGui::Indent();
                ImGui::PushID("Elements");

                for (size_t i = 0, j = 1ULL << start; i < length; i++, j <<= 1) {
                    bool& tempBool = *(tempToggle + start + i);
                    ImGui::PushID(int32_t(i));
                    if (ImGui::Checkbox(names[i], &tempBool)) {
                        changed = true;
                        if (tempBool) {
                            if (!allowMultiple) {
                                uint64_t negate = (mask & ~j);
                                bits &= ~negate;
                            }
                            bits |= j;
                        }
                        else {
                            bits &= ~j;
                        }
                    }
                    ImGui::PopID();
                }

                ImGui::PopID();
                ImGui::Unindent();
                ImGui::EndCombo();
            }

            if (changed) {
                value = T(bits);
            }
            return changed;
        }

        template<typename T>
        bool drawBitMask(const char* label, T& value, bool allowMultiple = true, bool displayAll = true) {
            return drawBitMask(label, value, 0, EnumNames<T>::Count, EnumNames<T>::getEnumNames(), allowMultiple, displayAll);
        }

        template<typename T>
        bool drawDropdown(const char* label, T& value, const char* const* names, int32_t itemCount) {
            int32_t valueInt = value;
            if (ImGui::Combo(label, &valueInt, names, itemCount)) {
                value = T(valueInt);
                return true;
            }
            return false;
        }

        template<typename T, typename U>
        bool drawDropdown(const char* label, T& value, const U* names, size_t itemCount) {
            bool changed = false;
            size_t selectI = size_t(value);

            if (ImGui::BeginCombo(label, selectI >= itemCount ? "" : names[selectI].getName())) {
                for (size_t i = 0; i < itemCount; i++) {
                    ImGui::PushID(int32_t(i));
                    if (ImGui::Selectable(names[i].getName(), i == selectI)) {
                        selectI = i;
                        value = T(selectI);
                        changed = true;
                    }
                    ImGui::PopID();
                }
                ImGui::EndCombo();
            }
            return changed;
        }


    }
}