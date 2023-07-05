#pragma once
#include <cstdint>
#include <string>
#include <JEngine.h>
#include <JEngine/Rendering/Renderer.h>
#include <JEngine/Utility/JTime.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Core/AssetDB.h>
#include <JEngine/Core/SceneManager.h>

int main(int argc, char** argv);

namespace JEngine {
    struct AppArgs {
        int32_t count = 0;
        char** args = nullptr;

        const char* operator[](int32_t index) const {
            JENGINE_CORE_ASSERT(index >= 0 && index < count);
            return args[index];
        }
    };

    struct AppSpecs {
        std::string name = "J-Engine Project";
        std::string workingDir{""};
        AppArgs args{};
    };

    class Application {
    public:

        Application(const AppSpecs& specs, const ResourceRoot* roots, size_t rootCount);
        virtual ~Application();
        
        static Application* getInstance() { return _instance; }

        bool init();
        virtual void run() = 0;

        bool isQuitting() const { return bool(_flags & FLAG_QUITTING); }
        void cancelQuit() { _flags &= ~FLAG_QUITTING; }
        void quit() { _flags |= FLAG_QUITTING; }

        JTime& getTime() { return _time; }
        const JTime& getTime() const { return _time; }

        const AppSpecs& getSpecs() const { return _specs; }
        Renderer& getRenderer() { return _renderer; }

        SceneManager& getSceneManager() { return _sceneManager; }
        const SceneManager& getSceneManager() const { return _sceneManager; }

        AssetDB& getAssetDB() { return _assetDB; }
        const AssetDB& getAssetDB() const { return _assetDB; }

    private:
        static Application* _instance;
        static constexpr uint8_t FLAG_QUITTING = 0x1;

        UI8Flags _flags;

        AppSpecs _specs;
        AssetDB _assetDB;
        SceneManager _sceneManager;
        Renderer _renderer;
        JTime _time;
    };

    Application* createApplication(AppArgs args);
}