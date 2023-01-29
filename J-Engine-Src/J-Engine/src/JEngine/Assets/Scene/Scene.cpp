#include <JEngine/assets/scene/Scene.h>
#include <JEngine/assets/AssetPaths.h>
#include <JEngine/io/FileExtensions.h>
#include <JEngine/core/Game.h>
#include <JEngine/core/managers/AssetManager.h>
#include <JEngine/components/Component.h>
#include <JEngine/components/GameObject.h>
#include <JEngine/components/builtin/CMap.h>
#include <JEngine/components/builtin/CCamera.h>
#include <JEngine/components/builtin/CTransform.h>
#include <JEngine/components/builtin/CCameraFollow.h>
#include <JEngine/components/builtin/CSpriteRenderer.h>
#include <iostream>
#include <string>
#include <fstream>
#include <JEngine/global/UUIDFactory.h>

namespace JEngine {

    Scene::Scene() : _beingRemoved(false), _isLoaded(false), _gameObjects{}, _startQueue(), _isStarting(false), _queueStartCalls(false) { }
    Scene::Scene(const Scene& scene) : Scene() { }

    Scene::~Scene() {
        _beingRemoved = true;

        for (size_t i = 0; i < _gameObjects.size(); i++) {
            delete _gameObjects[i];
        }

        _gameObjects.clear();
        std::vector<GameObject*>().swap(_gameObjects);
    }

    const int32_t Scene::getGameObjectCount() const {
        return _gameObjects.size();
    }

    void Scene::update(const GTime& time) {
        if (!_isLoaded) { return; }

        if (_queueStartCalls) {
            triggerStartQueue();
        }

        for (size_t i = 0; i < _gameObjects.size(); i++) {
            _gameObjects[i]->update(time);
        }
    }

    void Scene::lateUpdate(const GTime& time) {
        if (!_isLoaded) { return; }
        for (size_t i = 0; i < _gameObjects.size(); i++) {
            _gameObjects[i]->lateUpdate(time);
        }
    }

    void Scene::cleanUp() {
        for (int i = _gameObjects.size() - 1; i >= 0; i--) {
            auto go = _gameObjects[i];
            if (go->isMarkedForDeletion()) {
                go->actuallyDelete();
                continue;
            }
            go->cleanUp();
        }

        _queueStartCalls = false;
        _isStarting = false;
        _startQueue = {};
    }

    const bool Scene::load() {
        if (_isLoaded) { return true; }
        _isLoaded = true;

        std::string configPath = AssetPaths::SCENE_ROOT + _name + ".scene";
        if (IO::fileExists(configPath)) {
            std::cout << "Found config for scene '" << _name << "'!\n";

            std::ifstream strm(configPath, std::ios::binary);
            auto fType = Serialization::checkFileType(strm, SCENE_HEADER);

            try {

                switch (fType) {
                case Serialization::BINARY_FILE:
                    std::cout << "Scene config '" << _name << "' is BINARY!\n";

                    strm.seekg(0, std::ios::beg);
                    if (!deserializeBinary(strm)) {
                        std::cout << "Deserialization failed!\n";
                        goto abortLoad;
                    }
                    break;
                case Serialization::JSON_FILE: {
                    std::cout << "Scene config '" << _name << "' is JSON!\n";

                    strm.seekg(0, std::ios::beg);
                    json& jsonF = json::parse(strm);
                    if (!deserializeJson(jsonF)) {
                        std::cout << "Deserialization failed!\n";
                        goto abortLoad;
                    }
                    break;
                }
                default:
                    std::cout << "Scene config '" << _name << "' is UNKNOWN!\n";

                abortLoad:
                    unload();
                    _isLoaded = false;
                    break;
                }
            }
            catch (const std::exception& e) {
                std::cout << "Failed to load scene asset! \n";
                std::cout << e.what() << "\n";

                unload();

                _isLoaded = false;
            }
        }
        else {
            std::cout << "Didn't find config for scene '" << _name << "' in path " << configPath << "!\n";
        }

        return _isLoaded;
    }

    void Scene::unload() {
        if (!_isLoaded) { return; }
        std::vector<GameObject*> deleteThese;
        for (size_t i = 0; i < _gameObjects.size(); i++) {
            auto go = _gameObjects[i];
            if (!go->getTransform()->getParent()) {
                deleteThese.push_back(go);
            }
        }

        for (size_t i = 0; i < deleteThese.size(); i++) {
            deleteThese[i]->actuallyDelete();
        }
        deleteThese.clear();
        _gameObjects.clear();
        std::vector<GameObject*>().swap(_gameObjects);

        _queueStartCalls = false;
        _isStarting = false;
        _startQueue = {};

        _isLoaded = false;
    }

    void Scene::addToScene(GameObject* obj) {
        if (!_isLoaded) { return; }

        auto scenID = obj->getScene();
        if (scenID) {
            if (scenID == this) { return; }
            scenID->removeFromScene(obj);
        }
        obj->setScene(this);
        _gameObjects.push_back(obj);

        auto tr = obj->getTransform();
        auto childC = tr->getChildCount();
        for (size_t i = 0; i < childC; i++) {
            addToScene(tr->getChild(i)->getGameObject());
        }
    }

    void Scene::removeFromScene(GameObject* obj) {
        auto sceneID = obj->getSceneID();
        if (sceneID != _id) { return; }

        obj->setScene(nullptr);
        if (_beingRemoved) { return; }

        auto find = std::find(_gameObjects.begin(), _gameObjects.end(), obj);
        if (find != _gameObjects.end()) {
            _gameObjects.erase(find);
        }

        auto tr = obj->getTransform();
        auto childC = tr->getChildCount();
        for (size_t i = 0; i < childC; i++) {
            removeFromScene(tr->getChild(i)->getGameObject());
        }
    }

    GameObject* Scene::findGameObjectByName(const std::string& name, const bool active) {
        for (size_t i = 0; i < _gameObjects.size(); i++) {
            auto go = _gameObjects[i];
            if (active && !go->isActiveInScene()) { continue; }
            if (go->getName()._Equal(name)) { return go; }
        }
        return nullptr;
    }

    GameObject* Scene::getGameObjectAt(const int32_t index) {
        if (index < 0 || index >= _gameObjects.size()) { return nullptr; }
        return _gameObjects[index];
    }

    GameObject* Scene::getSerializableGameObjectAt(const int32_t index) {
        if (index < 0 || index >= _gameObjects.size()) { return nullptr; }

        for (size_t i = 0, j = 0; i < _gameObjects.size(); i++) {
            auto go = _gameObjects[i];
            if (go->getFlags().isBitSet(GameObject::NO_SERIALIZE_FLAG)) { continue; }

            if (j == index) { return go; }
            j++;
        }
        return nullptr;
    }

    GameObject* Scene::findGameObjectByUUID(const UUID& uuid) {
        for (size_t i = 0; i < _gameObjects.size(); i++) {
            if (_gameObjects[i]->getUUID() == uuid) {
                return _gameObjects[i];
            }
        }
        return nullptr;
    }

    const int32_t Scene::getObjectIndex(const GameObject* go) const {
        if (go == nullptr) { return -1; }
        for (size_t i = 0; i < _gameObjects.size(); i++) {
            if (_gameObjects[i] == go) { return i; }
        }
        return -1;
    }

    const int32_t Scene::getSerializableObjectIndex(const GameObject* go) const {
        if (go == nullptr || go->getFlags().isBitSet(GameObject::NO_SERIALIZE_FLAG)) { return -1; }

        int id = 0;
        for (size_t i = 0; i < _gameObjects.size(); i++)
        {
            auto goTmp = _gameObjects[i];
            if (goTmp == go) { return id; }
            if (!go->getFlags().isBitSet(GameObject::NO_SERIALIZE_FLAG)) { id++; }
        }
        return -1;
    }

    void Scene::setWillQueueStartCalls(const bool val) {
        _queueStartCalls = val;
    }

    const bool Scene::willQueueStartCalls() const {
        return _queueStartCalls;
    }

    void Scene::queueCompoentStart(Component* comp) {
        if (_isStarting || !_queueStartCalls) { comp->start(); return; }
        _startQueue.push(comp);
    }

    const bool Scene::deserializeJson(json& jsonFile) {
        auto& objA = jsonFile["objects"];
        std::vector<std::pair<json&, GameObject*>> gos{ };

        if (objA.is_null()) {
            std::cout << "No GameObjects Found in '" << _name << "'...\n";
            return _isLoaded;
        }

        const int objCount = objA.size();

        std::cout << "Found '" << objCount << "' gameobjects in '" << _name << "'...\n";
        if (objCount > 0) {
            //Loop over all objects in JSON and create game objects
            for (auto& obj : objA) {
                UUID uuid{};
                if (!UUIDFactory::parseUUID(obj.value("uuid", ""), uuid)) {
                    UUIDFactory::generateUUID<IAsset>(uuid);
                }

                GameObject* go = new GameObject(obj.value("name", "New GameObject"), GameObject::IS_ACTIVE_FLAG, uuid);
                gos.push_back(std::pair<json&, GameObject*>(obj, go));
            }

            std::vector<int32_t> offsets{};
            std::vector<json*> compJson{};
            int32_t offset = 0;

            _queueStartCalls = true;
            //Loop over gameobjects and prepare component deserialization
            for (size_t i = 0; i < gos.size(); i++) {
                offsets.push_back(offset);
                gos[i].second->deserializeJsonPrep(gos[i].first, compJson);
                offset += compJson.size() - offset;
            }

            //Finally deserialize components
            for (size_t i = 0; i < gos.size(); i++) {
                gos[i].second->deserializeJson(gos[i].first, compJson.data() + offsets[i]);
            }

            postLoadProcess();
            _queueStartCalls = false;
            triggerStartQueue();
        }
        return true;
    }

    const bool Scene::serializeJson(json& jsonFile) {
        auto& objs = jsonFile["objects"];
        int id = 0;
        for (auto obj : _gameObjects) {
            if (obj->shouldSerialize()) {
                objs.push_back(json({}));
                obj->serializeJson(objs[id++]);
            }
        }
        return true;
    }

    const bool Scene::deserializeBinary(std::istream& stream) {
        char hdr[4]{ 0 };
        stream.read(hdr, 4);

        if (strncmp(hdr, SCENE_HEADER, 4) != 0) {
            std::cout << "Invalid scene header! [" << hdr << "]\n";
            return false;
        }

        std::vector<int32_t> offsets{};
        std::vector<std::streampos> compPos{};
        GameVersion& gVers = Game::getGameVersion();
        GameVersion vers;
        vers.deserializeBinary(stream);
        auto res = Game::getEnginePreferences().compareVersions(gVers, vers);

        if (res != 0) {
            std::cout << "Game Version (" << gVers << ") doesn't match File Version (" << vers << ")!\n";
            return false;
        }
        std::cout << "File Version (" << vers << ")!\n";

        uint32_t objects;
        Serialization::deserializeBinary(stream, objects);
        std::cout << "Scene has '" << objects << "' GameObjects!\n";

        std::vector<std::pair<uint32_t, GameObject*>> gos{};
        uint32_t seek;
        UI16Flags flags;
        for (size_t i = 0; i < objects; i++) {
            std::string name = Serialization::deserializeStringBinary(stream);
            UUID uuid{};
            Serialization::deserializeBinary(stream, uuid);

            GameObject* go = new GameObject(name, GameObject::IS_ACTIVE_FLAG, uuid);
            uint32_t pos = stream.tellg();

            Serialization::deserializeBinary(stream, seek);
            flags.deserializeBinary(stream);
            go->setFlags(flags);

            stream.seekg(seek - flags.BIN_SIZE, std::ios::cur);
            gos.push_back(std::make_pair(pos, go));
        }

        _queueStartCalls = true;
        int32_t offset = 0;
        for (size_t i = 0; i < gos.size(); i++) {
            auto& goP = gos[i];
            stream.seekg(goP.first, std::ios::beg);
            offsets.push_back(offset);
            goP.second->deserializeBinaryPrep(stream, compPos);
            offset += compPos.size() - offset;
        }

        for (size_t i = 0; i < gos.size(); i++) {
            gos[i].second->deserializeBinary(stream, compPos.data() + offsets[i]);
        }

        postLoadProcess();
        _queueStartCalls = false;
        triggerStartQueue();
        return true;
    }

    const bool Scene::serializeBinary(std::ostream& stream) {
        stream.write(SCENE_HEADER, 4);
        Game::getGameVersion().serializeBinary(stream);

        uint32_t objectCount = 0;
        auto pos = stream.tellp();
        Serialization::serializeBinary(stream, objectCount);

        for (size_t i = 0; i < _gameObjects.size(); i++) {
            auto go = _gameObjects[i];
            if (go->shouldSerialize()) {
                go->serializeBinary(stream);
                objectCount++;
            }
        }

        auto end = stream.tellp();
        stream.seekp(pos, std::ostream::beg);
        stream.write(reinterpret_cast<char*>(&objectCount), sizeof(uint32_t));
        stream.seekp(end, std::ostream::beg);
        return true;
    }

    void Scene::triggerStartQueue() {
        _isStarting = true;
        while (!_startQueue.empty()) {
            _startQueue.front()->start();
            _startQueue.pop();
        }
        _isStarting = false;
    }

    void Scene::postLoadProcess() {
        for (size_t i = 0; i < _gameObjects.size(); i++) {
            auto tr = _gameObjects[i]->getTransform();
            if (!tr->getParent()) {
                tr->updateWorldTransform();
            }
        }
    }
}