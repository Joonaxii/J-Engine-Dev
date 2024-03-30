#include <JEngine/Audio/AudioClip.h>

namespace JEngine {
    AudioClip::~AudioClip() {
        unload();
    }

    bool AudioClip::unload()  {
        if (IAsset::unload()) {
            _audioData.release();
            return true;
        }
        return false;
    }


}