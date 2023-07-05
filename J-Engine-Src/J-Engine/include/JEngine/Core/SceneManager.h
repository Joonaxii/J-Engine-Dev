#pragma once
#include <JEngine/Core/Scene.h>
#include <JEngine/Utility/Action.h>

namespace JEngine {
    class SceneManager {
    public:
        typedef Action<const Scene*> OnScene;
        SceneManager();

        Scene* getActiveScene() { return _activeScene; }
        const Scene* getActiveScene() const { return _activeScene; }
        const std::vector<Scene*>& getScenes() const { return _scenes; }

        OnScene& getOnSceneUnload() { return _onSceneUnload; }
        OnScene& getOnSceneLoad() { return _onSceneLoad; }

        void loadScene(Scene* scene);
        void unloadActiveScene();

        void addScene(Scene* scene);
        Scene* removeScene(Scene* scene);

    private:
        OnScene _onSceneUnload;
        OnScene _onSceneLoad;

        Scene* _activeScene;
        std::vector<Scene*> _scenes;
    };
}