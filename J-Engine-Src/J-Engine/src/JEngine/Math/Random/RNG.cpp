#include <stdlib.h>

#include <JEngine/Math/Math.h>
#include <JEngine/Math/Random/RNG.h>

namespace JEngine::RNG {
    const int64_t setSeed(const int64_t seed, const bool reset) {
        RNGData::initialized = true;
        RNGData::seed = seed;
        if (reset) {
            RNGData::offset = 0;
        }
        return seed + RNGData::offset;
    }
}
