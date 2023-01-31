#include <JEngine/Debugging/FPS.h>

namespace JEngine {
    FPS::FPS() : _fps(60), _tempFrame(0), _tempTime(0), _histPos(0) {
        for (size_t i = 0; i < TOTAL_HIST_SIZE; i++) {
            _history[0] = 0;
        }
    }

    const double FPS::update(const JTime& time) {
        _tempFrame++;
        _tempTime += time.getEngineDeltaTime();
        if (_tempFrame >= 20) {
            auto fpsL = _tempFrame / _tempTime;

            for (int i = TOTAL_HIST_SIZE - 2; i >= 0; i--) {
                _history[i + 1] = _history[i];
            }

            _history[0] = fpsL;
            if (_histPos < TOTAL_HIST_SIZE) {
                _histPos++;
            }

            _fps = 0;
            for (size_t i = 0; i < _histPos; i++) {
                _fps += _history[i];
            }
            _fps /= double(_histPos);

            _tempFrame = 0;
            _tempTime = 0;
        }
        return _fps;
    }

    const double FPS::getFPS() const {
        return _fps;
    }
}