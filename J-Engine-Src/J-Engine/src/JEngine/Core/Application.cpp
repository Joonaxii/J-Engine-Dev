#include <JEngine/Core/Application.h>
#include <JEngine/Rendering/Window.h>
#include <JEngine/Components/ComponentFactory.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/IO/Helpers/IOUtils.h>

namespace JEngine {
    Application* Application::_instance{ nullptr };

    Application::Application(const AppSpecs& specs) : _specs(specs), _time(), _assetDB() {}
    Application::~Application() {
        ComponentFactory::clearAllComponentPools(true);
    }

    bool Application::init() {
        JE_CORE_TRACE("Initializing: '{0}'", _specs.name.c_str());
        auto now = std::chrono::system_clock::now();
        JE_CORE_TRACE(" - Start Time: {0}", now);
        JE_CORE_TRACE(" - Working Directory: '{0}'", _specs.workingDir.c_str());

        if (!_renderer.init(_specs.name.c_str())) {
            return false;
        }

        _instance = this;
        _time.reset();
        //TODO: Possibly add some other general initialization/verification stuff
        return true;
    }

    bool Application::hasFocus() const {
        return _flags.isBitSet(APP_FORCE_FOCUS) || _renderer.getWindow().isFocused();
    }

    void Application::toggleConsoleCloseButton(bool state) {
        if (HWND hwnd = ::GetConsoleWindow()) {
            if (HMENU hMenu = GetSystemMenu(hwnd, FALSE))
            {
                EnableMenuItem(hMenu, SC_CLOSE, state ? (MF_BYCOMMAND | MF_ENABLED) : (MF_BYCOMMAND | MF_DISABLED | MF_GRAYED));
            }
        }
    }

    void Application::hideConsole() {
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    }

    void Application::showConsole() {
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    }

    bool Application::isConsoleVisible() {
        return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
    }
}
