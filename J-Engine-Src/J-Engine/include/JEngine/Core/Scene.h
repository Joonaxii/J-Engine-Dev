#pragma once
#include <JEngine/Assets/IAsset.h>
#include <JEngine/Utility/Flags.h>

namespace JEngine {
    class GameObject;
    class Scene : public IAsset {
    public:
        ~Scene();

        static Scene* createScene(const std::string& name, uint8_t flags);
        static Scene* createScene(const char* name, uint8_t flags);

        bool deserializeBinary(const Stream& stream);
        bool serializeBinary(const Stream& stream) const;

        bool deserializeJSON(json& jsonF);
        bool serializeJSON(json& jsonF) const;

        bool deserializeYAML(const yamlNode& node);
        bool serializeYAML(yamlEmit& emit) const;

        bool load();
        bool unload();

    private:
        enum : uint8_t {
            SC_IS_LOADED = 0x1,
        };
        friend class SceneManager;

        UI8Flags _infoFlags;
        std::vector<GameObject*> _objects;
        
        Scene(const char* name, uint8_t flags);

        Scene(const Scene& other) = delete;
        Scene(Scene&& other) = delete;
        Scene& operator=(const Scene&) = delete;

        void* operator new(size_t size) noexcept;
        void operator delete(void* ptr) noexcept;
    };
}