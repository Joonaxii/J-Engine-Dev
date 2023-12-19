#pragma once
#include <cstdint>
#include <string>
#include <JEngine.h>
#include <JEngine/Rendering/Renderer.h>
#include <JEngine/Utility/JTime.h>
#include <JEngine/Utility/Flags.h>
#include <JEngine/Core/AssetDB.h>
#include <JEngine/Core/SceneManager.h>
#include <Windows.h>

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

        Application(const AppSpecs& specs);
        virtual ~Application();
        
        static Application* get() { return _instance; }

        virtual bool init();
        virtual void run() = 0;

        bool isQuitting() const { return bool(_flags & APP_FLAG_QUITTING); }
        void cancelQuit() { _flags &= ~APP_FLAG_QUITTING; }
        void quit() { _flags |= APP_FLAG_QUITTING; }

        JTime& getTime() { return _time; }
        const JTime& getTime() const { return _time; }

        const AppSpecs& getSpecs() const { return _specs; }
        Renderer& getRenderer() { return _renderer; }

        Scene& getScene() { return _scene; }
        const Scene& getScene() const { return _scene; }

        AssetDB& getAssetDB() { return _assetDB; }
        const AssetDB& getAssetDB() const { return _assetDB; }

        void setForceFocus(bool focus) { _flags.setBit(APP_FORCE_FOCUS, focus); }
        bool hasFocus() const;

        static void toggleConsoleCloseButton(bool state);
        static void hideConsole();
        static void showConsole();
        static bool isConsoleVisible();

    private:
        static Application* _instance;
        enum : uint8_t {
            APP_FLAG_QUITTING = 0x1,
            APP_FORCE_FOCUS = 0x2,
        };
        UI8Flags _flags;

        AppSpecs _specs;
        AssetDB _assetDB;
        Scene _scene;
        Renderer _renderer;
        JTime _time;
    };

    Application* createApplication(AppArgs args);
}