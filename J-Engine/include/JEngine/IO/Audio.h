#pragma once
#include <JEngine/IO/AudioUtils.h>
#include <JEngine/IO/Stream.h>

namespace JEngine {

    namespace Wav {

        bool decode(const char* path, AudioData& audio);
        bool decode(const Stream& stream, AudioData& audio);

        bool encode(const char* path, const AudioData& audio);
        bool encode(const Stream& stream, const AudioData& audio);

    }
}