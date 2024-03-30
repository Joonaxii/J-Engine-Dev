#pragma once
#include <cstdint>
#include <JEngine/Utility/JTime.h>

namespace JEngine {
    class FPS {
    public:
        FPS();

        double update(const JTime& time);
        double getFPS() const;

    private:
        static constexpr int TOTAL_HIST_SIZE = 8;

        int _histPos;
        double _history[TOTAL_HIST_SIZE];

        uint64_t _tempFrame;
        double _tempTime;

        double _fps;
    };
}