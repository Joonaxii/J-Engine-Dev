#include <JEditor/GUI/Panels/ConsolePanel.h>
#include <JEngine/Core/Application.h>

namespace JEngine::Editor {
    ConsolePanel::ConsolePanel(const char* title, Alignment align) : IDockPanel(title, align) { }
    void ConsolePanel::init(Application* app)
    {
    }
    void ConsolePanel::draw(Application* app) {
        if (app) {

        }
    }
    void ConsolePanel::close(Application* app)
    {
    }
}