#include <JEngine/Core/SceneManager.h>

namespace JEngine {
    SceneManager::SceneManager() : _activeScene(nullptr), _scenes {} { }

    void SceneManager::loadScene(Scene* scene) {
        if (scene) {
            if(scene == _activeScene){
                JENGINE_CORE_INFO("[J-Engine - SceneManager] Reloading Scene '{0}'!", scene->getName().c_str());
            }
            if(_activeScene) {
                unloadActiveScene();
            }
            _activeScene = scene;
            if (_activeScene->load()) {
                return;
            }
            JENGINE_CORE_ERROR("[J-Engine - SceneManager] Error: Failed to load Scene '{0}'!", scene->getName().c_str());
            return;
        }
        JENGINE_CORE_WARN("[J-Engine - SceneManager] Warning: Cannot load null scene!");
    }

    void SceneManager::unloadActiveScene() {
        if (_activeScene) {
            if (_activeScene->unload()) {
                JENGINE_CORE_INFO("[J-Engine - SceneManager] Unloaded Scene '{0}'!", _activeScene->getName().c_str());
                _activeScene = nullptr;
                return;
            }
            JENGINE_CORE_ERROR("[J-Engine - SceneManager] Error: Failed to unload Scene '{0}'! (Maybe already unloaded!)", _activeScene->getName().c_str());
            _activeScene = nullptr;
            return;
        }
        JENGINE_CORE_WARN("[J-Engine - SceneManager] Warning: Cannot unload null scene!");
    }

    void SceneManager::addScene(Scene* scene) {
        if (!scene) { return; }
        auto find = std::find(_scenes.begin(), _scenes.end(), scene);
        if (find == _scenes.end()) {
            _scenes.push_back(scene);
        }
    }

    Scene* SceneManager::removeScene(Scene* scene) {
        if (!scene) { return nullptr; }

        auto find = std::find(_scenes.begin(), _scenes.end(), scene);
        if (find != _scenes.end()) {
            _scenes.erase(find);
            if (scene == _activeScene) {

            }
            return nullptr;
        }
        return scene;
    }
}

