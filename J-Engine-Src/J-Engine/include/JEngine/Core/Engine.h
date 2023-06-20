#pragma once
#include <JEngine/Utility/JTime.h>

namespace JEngine {
    class Engine {
    public:
        Engine();
        ~Engine();

        bool init();
        bool step();

    private:
        JTime _time;

    };
}