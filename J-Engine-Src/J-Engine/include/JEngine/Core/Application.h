#pragma once
#include <JEngine.h>
#include <JEngine/Rendering/Renderer.h>
#include <cstdint>
#include <string>
#include <JEngine/Utility/JTime.h>

int main(int argc, char** argv);

namespace JEngine {
    struct AppArgs {
        int32_t count = 0;
        char** args = nullptr;

        const char* operator[](int32_t index) const
        {
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

        Application(const AppSpecs specs);
        virtual ~Application();
        
        static Application* getInstance() { return _instance; }

        bool init();
        virtual void run() = 0;

        JTime& getTime() { return _time; }
        const JTime& getTime() const { return _time; }

        const AppSpecs& getSpecs() const { return _specs; }
        Renderer& getRenderer() { return _renderer; }

    private:
        static Application* _instance;

        AppSpecs _specs;
        JTime _time;
        Renderer _renderer;
    };

    Application* createApplication(AppArgs args);
}