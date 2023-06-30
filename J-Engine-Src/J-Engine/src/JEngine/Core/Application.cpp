#include <JEngine/Core/Application.h>
#include <JEngine/Rendering/Window.h>
#include <JEngine/Components/ComponentFactory.h>
#include <JEngine/Core/GameObject.h>

namespace JEngine {
    Application* Application::_instance{ nullptr };

    Application::Application(const AppSpecs specs) : _time() {}
    Application::~Application() {
        GameObject::getGameObjectAllocator().clear(true);
        ComponentFactory::clearAllComponentPools(true);
    }

    bool Application::init() {
        JENGINE_CORE_TRACE("Initializing Game: '{0}'", _specs.name.c_str());
        auto now = std::chrono::system_clock::now();
        JENGINE_CORE_TRACE(" - Start Time: {0}", now);
        JENGINE_CORE_TRACE(" - Working Directory: '{0}'", _specs.workingDir.c_str());

        if (!_renderer.init(_specs.name.c_str())) {
            return false;
        }

        _instance = this;
        _time.reset();
        Input::init();
        //TODO: Possibly add some other general initialization/verification stuff
        return true;
    }
}
