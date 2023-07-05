#include <JEngine/Core/Scene.h>
#include <JEngine/Core/GameObject.h>

namespace JEngine {
    Scene::Scene(const char* name, uint8_t flags) : IAsset(), _infoFlags(), _objects{} { }
    Scene::~Scene() {
        unload();
    }

    bool Scene::load() {
        if (_infoFlags & SC_IS_LOADED) { return false; }
        for (auto& obj : _objects) {
            obj->start();
        }
        return true;
    }

    bool Scene::unload()
    {
        return false;
    }
}