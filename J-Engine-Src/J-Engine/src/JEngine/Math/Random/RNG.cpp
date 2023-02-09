#include <stdlib.h>

#include <JEngine/Math/Math.h>
#include <JEngine/Math/Random/RNG.h>

namespace JEngine::RNG {
   uint32_t getSeed(const bool reset) {
        if (!RNGData::initialized) {
            uint64_t time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            setSeed(uint32_t(time & 0xFFFFFFU) ^ uint32_t(time >> 32), reset);
        }else if (reset) {
            RNGData::offset = 0;
        }
        return RNGData::seed + RNGData::offset++;
    }

    uint32_t setSeed(const uint32_t seed, const bool reset) {
        RNGData::initialized = true;
        RNGData::seed = seed;
        if (reset) {
            RNGData::offset = 0;
        }
        return seed + RNGData::offset;
    }

    uint32_t randNum() {
        static std::mt19937 eng(0);
        eng.seed(getSeed(false));
        return eng();
    }
}
