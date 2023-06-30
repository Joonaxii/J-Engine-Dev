#pragma once
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Utility/Flags.h>

namespace JEngine {
    class GameObject;
    class Scene {
    public:

        static Scene* createScene(const std::string& name, uint8_t flags);
        static Scene* createScene(const char* name, uint8_t flags);

        bool deserializeBinary(const Stream& stream);
        bool serializeBinary(const Stream& stream) const;

        bool deserializeJSON(json& jsonF);
        bool serializeJSON(json& jsonF) const;

        bool deserializeYAML(const yamlNode& node);
        bool serializeYAML(yamlEmit& emit) const;

    private:
        UI8Flags _flags;
        UI8Flags _infoFlags;
        std::string _name;
        std::vector<GameObject*> _objects;
        
        Scene(const char* name);
    };
}