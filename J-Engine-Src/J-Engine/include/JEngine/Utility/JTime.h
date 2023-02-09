#pragma once
#include <cstdint>
#include <chrono>
#include <string>

namespace JEngine {
    class JTime {
    public:
        static constexpr uint8_t MAX_TIME_SPACES = 32;
        struct TimeSpace {
        public:

            TimeSpace() : _time(0), _timeScale(1.0), _deltaTime(0) {}

            template<typename P = double> 
            P getDeltaTime() const { return P(_deltaTime); }

            template<typename P = double>
            P getTime() const { return P(_time); }

            template<typename P = double>
            P getTimeScale() const { return P(_timeScale); }

            template<typename P = double>
            void setTimeScale(const P scale) { _timeScale = double(scale); }

            void reset() { _time = 0; _timeScale = 1.0; _deltaTime = 0; }

            template<typename P = double>
            void update(const P delta) {
                _deltaTime = delta * _timeScale;
                _time += _deltaTime;
            }


        private:
            double _time;
            double _timeScale;
            double _deltaTime;
        };

        JTime();

        void tick();
        void reset();

        template<typename P = double>
        P getEngineTime() const { return P(_time); }

        template<typename P = double>
        P getEngineDeltaTime() const { return P(_delta); }

        uint64_t getFrameCount() const;

        TimeSpace& getTimeSpace(const uint8_t space);
        const TimeSpace& getTimeSpace(const uint8_t space) const;

        template<typename P = double>
        P getDeltaTime(const uint8_t space) const { return getTimeSpace(space).getDeltaTime<P>(); }

        template<typename P = double>
        P getTime(const uint8_t space) const { return getTimeSpace(space).getTime<P>(); }

        template<typename P = double>
        P getTimeScale(const uint8_t space) const { return getTimeSpace(space).getTimeScale<P>(); }

        template<typename P = double>
        void setTimeScale(const uint8_t space, const P scale) { return getTimeSpace(space).setTimeScale<P>(scale); }

    private:
        TimeSpace _timeSpaces[MAX_TIME_SPACES];
        uint64_t _frameNum;
        std::chrono::high_resolution_clock _clock;
        double _delta;
        double _time;
        std::chrono::steady_clock::time_point _timeP;
    };
}