#pragma once
#include <JEngine/Core.h>
#include <JEngine/Rendering/Renderer.h>
#include <cstdint>
#include <string>

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

        bool init();
        virtual void run() = 0;

        const AppSpecs& getSpecs() const { return _specs; }
        Renderer& getRenderer() { return _renderer; }

    private:
        AppSpecs _specs;
        Renderer _renderer;
    };

    Application* createApplication(AppArgs args);
}