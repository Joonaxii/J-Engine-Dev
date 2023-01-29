#pragma once
#include <vector>
#include <string>
#include <queue>
#include <JEngine/io/ISerializable.h>
#include <JEngine/core/misc/GTime.h>
#include <JEngine/core/misc/Flags.h>
#include <JEngine/assets/IAsset.h>

namespace JEngine {

    class GameObject;
    class Component;
    class CSpriteRenderer;
    class CCamera;
    class Scene : public ISerializable<Scene>, public IAsset {
    public:
        static constexpr char* SCENE_HEADER = "JSCE";

        static constexpr int32_t START_FLAG = 0x1;
        static constexpr int32_t UPDATE_FLAG = 0x2;
        static constexpr int32_t LATE_UPDATE_FLAG = 0x4;

        Scene();
        Scene(const Scene& scene);
        ~Scene();

        const int32_t getGameObjectCount() const;

        void update(const GTime& time);
        void lateUpdate(const GTime& time);

        void cleanUp();

        void addToScene(GameObject* obj);
        void removeFromScene(GameObject* obj);

        GameObject* findGameObjectByName(const std::string& name, const bool active = true);
        GameObject* getGameObjectAt(const int32_t index);
        GameObject* getSerializableGameObjectAt(const int32_t index);

        GameObject* findGameObjectByUUID(const UUID& uuid);

        const int32_t getObjectIndex(const GameObject* go) const;
        const int32_t getSerializableObjectIndex(const GameObject* go) const;

        inline bool operator ==(const int32_t& other) { return this->_id == other; }
        inline bool operator !=(const int32_t& other) { return this->_id != other; }

        inline bool operator <(const int32_t& other) { return this->_id < other; }
        inline bool operator >(const int32_t& other) { return this->_id > other; }

        void setWillQueueStartCalls(const bool val);
        const bool willQueueStartCalls() const;
        void queueCompoentStart(Component* comp);

        const bool loadPartially();
        const bool load();
        void unload();

#pragma region Serialization
        const bool deserializeJson(json& jsonFile);
        const bool serializeJson(json& jsonFile);

        const bool deserializeBinary(std::istream& stream);
        const bool serializeBinary(std::ostream& stream);

        const bool deserializeJsonPartial(json& jsonFile);
        const bool serializeJsonPartial(json& jsonFile);

        const bool deserializeBinaryPartial(std::istream& stream);
        const bool serializeBinaryPartial(std::ostream& stream);
#pragma endregion

    private:
        bool _beingRemoved;
        bool _isLoaded;

        UI8Flags _updateFlags;

        std::vector<GameObject*> _gameObjects;
        std::queue<Component*> _startQueue;

        std::vector<std::pair<Component*, GameObject*>> _updateComponentBuffer;
        std::vector<GameObject*> _updateBuffer;

        bool _isStarting;
        bool _queueStartCalls;

        void triggerStartQueue();
        void postLoadProcess();
    };
}
REGISTER_ASSET_SER(JEngine::Scene, JEngine::AssetType::Scene)