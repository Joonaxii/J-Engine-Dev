#include <JEngine/Core/Engine.h>

namespace JEngine {
    Engine::Engine() : _time() {}
    Engine::~Engine() {}

    bool Engine::init() {
        _time.reset();
        return true;
    }

    bool Engine::step() {

        _time.tick();
        return true;
    }
}