#include <JEngine/Core/Application.h>
#include <JEngine/Rendering/Window.h>

namespace JEngine {

    Application::Application(const AppSpecs specs) {}
    Application::~Application() {}

    bool Application::init() {
        JENGINE_CORE_TRACE("Initializing Game: '{0}'", _specs.name.c_str());
        auto now = std::chrono::system_clock::now();
        JENGINE_CORE_TRACE(" - Start Time: {0}", now);
        JENGINE_CORE_TRACE(" - Working Directory: '{0}'", _specs.workingDir.c_str());

        if (!_renderer.init(_specs.name.c_str())) {
            return false;
        }

        //TODO: Possibly add some other general initialization/verification stuff
        return true;
    }
}
