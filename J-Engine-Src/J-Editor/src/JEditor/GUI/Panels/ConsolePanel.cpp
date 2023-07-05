#include <JEditor/GUI/Panels/ConsolePanel.h>

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