#pragma once
#include <JEngine/Core.h>
#include <cstdint>
#include <string>

int main(int argc, char** argv);

namespace JEngine {
    struct AppArgs {
        int32_t count = 0;
        char** args = nullptr;

        const char* operator[](int32_t index) const
        {
            //JENGINE_CORE_ASSERT(index < Count);
            return args[index];
        }
    };

    struct AppSpecs {
        std::string name = "J-Engine Project";
        std::string workingDir;
        AppArgs args;
    };

    class Application {
    public:

        Application(const AppSpecs specs);
        virtual ~Application();

        void run();
        void close();

    private:
        AppSpecs _specs;

    };

    Application* createApplication(AppArgs args);
}