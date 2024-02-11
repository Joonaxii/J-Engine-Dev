#pragma once
#include <JEngine/Utility/Flags.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/Collections/IndexStack.h>
#include <JEngine/Core/Ref.h>

namespace JEngine {
    class SceneAsset;
    class Scene {
    public:
        Scene() {}
        ~Scene();

        bool load(TAssetRef<SceneAsset> scene);
        bool unload();

        bool uuidIsInUse(uint32_t uuid) const;
        GameObject* getByUUID(uint32_t uuid);
        Component* Scene::findComponent(CompRef uuid);

        static GORef createObject(std::string_view name, const UUID16* components, size_t compCount, uint16_t flags = 0x00, uint32_t uuidRef = UINT32_MAX);
        static GORef createObject(std::string_view name, uint16_t flags = 0x00, uint32_t uuidRef = UINT32_MAX);

        static bool destroyObject(uint32_t uuid);
        static GameObject* destroyObject(GameObject* go);
        static GORef destroyObject(GORef go);

    private:
        TAssetRef<SceneAsset> _sceneAsset;

        ChunkedLUT<GameObject> _gameObjects;
        ChunkedLUT<GameObject> _singletons;

        Scene(const Scene& other) = delete;
        Scene(Scene&& other) = delete;
        Scene& operator=(const Scene&) = delete;

        void* operator new(size_t size) noexcept { return malloc(size); };
        void operator delete(void* ptr) noexcept { free(ptr); };

        void clear();
    };
}