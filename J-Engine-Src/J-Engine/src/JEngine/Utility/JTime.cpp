#include <JEngine/Utility/JTime.h>

namespace JEngine {
	JTime::JTime() : _delta(), _frameNum(0), _clock(), _time(0), _timeP() {
		for (size_t i = 0; i < MAX_TIME_SPACES; i++) {
			_timeSpaces[i].reset();
		}
		_timeP = _clock.now();
	}

	void JTime::tick() {
		auto time = _clock.now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time - _timeP).count();
		
		_delta = ms / 1000.0;
		_time += _delta;

		for (size_t i = 0; i < MAX_TIME_SPACES; i++) {
			_timeSpaces[i].update(_delta);
		}
		_frameNum++;
		_timeP = time;
	}

	void JTime::reset() {
		_timeP = _clock.now();

		for (size_t i = 0; i < MAX_TIME_SPACES; i++) {
			_timeSpaces[i].reset();
		}
	}

	const uint64_t JTime::getFrameCount() const {
		return _frameNum;
	}

	JTime::TimeSpace& JTime::getTimeSpace(const uint8_t space) {
		return _timeSpaces[space >= MAX_TIME_SPACES ? MAX_TIME_SPACES - 1 : space];
	}

	const JTime::TimeSpace& JTime::getTimeSpace(const uint8_t space) const {
		return _timeSpaces[space >= MAX_TIME_SPACES ? MAX_TIME_SPACES - 1 : space];
	}
}