#include <JEngine/Audio/IAudioSource.h>

namespace JEngine {

    void IAudioSource::play(int32_t section, AudioPlayType playMode) {
        if (_clip == nullptr) {
            JE_CORE_WARN("[IAudioSource] Warning: Cannot play an audio clip that's null!");
            return;
        }


    }
}