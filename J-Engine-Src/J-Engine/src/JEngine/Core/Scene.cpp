#include <JEngine/Core/Scene.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/Components/ComponentFactory.h>
#include <JEngine/Collections/IndexStack.h>
#include <JEngine/Core/Application.h>
#include <JEngine/Assets/SceneAsset.h>

namespace JEngine {
    Scene::~Scene() {
        unload();
    }

    bool Scene::load(TAssetRef<SceneAsset> scene) {
        return true;
    }

    bool Scene::unload() {
        return false;
    }

    bool Scene::uuidIsInUse(uint32_t uuid) const {
        return _gameObjects.isIndexUsed(uuid);
    }

    GameObject* Scene::getByUUID(uint32_t uuid) {
        return _gameObjects.getAt(uuid);
    }

    Component* Scene::findComponent(CompRef uuid) {
        if (!uuid.isValid()) { return nullptr; }
        GameObject* go = _gameObjects.getAt(uuid.getGOUUID());
        if (!go) { return nullptr; }
        return go->getComponentByIndex(uuid.getIndex());
    }

    void Scene::clear() {
        _gameObjects.clear();
        _sceneAsset = nullptr;
    }

    GORef Scene::createObject(std::string_view name, const UUID16* components, size_t compCount, uint16_t flags, uint32_t uuidRef) {
        Scene& scene = Application::get()->getScene();

        GameObject* go = nullptr;
        uint32_t uuid = uuidRef != UINT32_MAX ? scene._gameObjects.tryReserve(uuidRef, &go) : scene._gameObjects.popNext(&go);

        if (!go) {
            JENGINE_CORE_ERROR("[Scene] Error: Couldn't allocate game object!");
            return GORef(nullptr);
        }

        go->init(name, flags);
        go->setUUID(uuid);

        if (components) {
            for (size_t i = 0; i < compCount; i++) {
                ComponentFactory::addComponent(go, components[i]);
            }
        }
        return GORef(uuid);
    }

    GORef Scene::createObject(std::string_view name, uint16_t flags, uint32_t uuidRef) {
        return createObject(name, nullptr, 0, flags, uuidRef);
    }

    bool Scene::destroyObject(uint32_t uuid) {
        Scene& scene = Application::get()->getScene();
        if (scene._gameObjects.markFree(uuid)) {
            return true;
        }
        JENGINE_CORE_ERROR("[Scene] Error: Couldn't deallocate game object! [{0:X}]", uuid);
        return false;
    }

    GameObject* Scene::destroyObject(GameObject* go) {
        if (!go) { return nullptr; }
        return destroyObject(go->getUUID()) ? nullptr : go;
    }

    GORef Scene::destroyObject(GORef go) {
        return destroyObject(go.uuid) ? GORef(nullptr) : go;
    }
}