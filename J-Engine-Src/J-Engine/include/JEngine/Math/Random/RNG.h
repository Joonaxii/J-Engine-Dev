#pragma once
#include <random>
#include <chrono>
#include <cstdint>
#include <JEngine/Math/Random/WeightedValue.h>
#include <JEngine/Math/Math.h>

namespace JEngine::RNG {
    struct RNGData {
        static bool initialized;
        static uint32_t seed;
        static uint32_t offset;
    };

    bool RNGData::initialized = false;
    uint32_t RNGData::seed = 0;
    uint32_t RNGData::offset = 0;

    uint32_t getSeed(const bool reset = false);
    uint32_t setSeed(const uint32_t seed, const bool reset = true);

    uint32_t randNum();

    template<typename T>
    T randN() { return static_cast<T>(randNum() / T(UINT32_MAX)); }

    template<typename T>
    T randRange(const T min, const T max) {
        return Math::lerp(min, max, randN());
    }

    template<>
    inline int64_t randRange<int64_t>(const int64_t min, const int64_t max) {
        return min + (randNum() % (max - min));
    }

    template<>
    inline uint64_t randRange<uint64_t>(const uint64_t min, const uint64_t max) {
        return min + (randNum() % (max - min));
    }

    template<>
    inline int8_t randRange<int8_t>(const int8_t min, const int8_t max) {
        return int8_t(randRange<int64_t>(min, max));
    }

    template<>
    inline int16_t randRange<int16_t>(const int16_t min, const int16_t max) {
        return int16_t(randRange<int64_t>(min, max));
    }

    template<>
    inline int32_t randRange<int32_t>(const int32_t min, const int32_t max) {
        return int32_t(randRange<int64_t>(min, max));
    }

    template<>
    inline uint8_t randRange<uint8_t>(const uint8_t min, const uint8_t max) {
        return uint8_t(randRange<uint64_t>(min, max));
    }

    template<>
    inline uint16_t randRange<uint16_t>(const uint16_t min, const uint16_t max) {
        return uint16_t(randRange<uint64_t>(min, max));
    }

    template<>
    inline uint32_t randRange<uint32_t>(const uint32_t min, const uint32_t max) {
        return uint32_t(randRange<uint64_t>(min, max));
    }

    template<typename T>
    inline T randRange(const T min, const T max, const bool exclusive) {
        return randRange<T>(min, exclusive ? max : max + 1);
    }

    template<>
    inline float randRange<float>(const float min, const float max, const bool exclusive) {
        return Math::lerp(min, max, randN<float>());
    }

    template<>
    inline double randRange<double>(const double min, const double max, const bool exclusive) {
        return Math::lerp(min, max, randN<double>());
    }

    template<typename TValue, typename TWeight>
    const TValue& randSelect(const TValue* arrIn, size_t start, size_t length) {
        return arrIn[randRange(start, start + length)];
    }

    template<typename TValue, typename TWeight>
    TWeight getTotalWeight(const WeightedValue<TValue, TWeight>* arrIn, size_t start, size_t length) {
        TWeight total = 0;
        for (size_t i = start; i < start + length; i++) {
            total += arrIn[i].getWeight();
        }
        return total;
    }

    template<typename TValue, typename TWeight>
    const TValue& selectWeighted(const WeightedValue<TValue, TWeight>* arrIn, size_t start, size_t length, const TValue default) {
        return selectWeighted(arrIn, start, length, getTotalWeight(arrIn, start, length), default);
    }

    template<typename TValue, typename TWeight>
    const TValue& selectWeighted(const WeightedValue<TValue, TWeight>*arrIn, size_t start, size_t length, const float total, const TValue & default) {
        auto ind = selectWeightedIndex(arrIn, start, length, total);
        return ind < 0 ? default: arrIn[ind].getValue();
    }

    template<typename TValue, typename TWeight>
    int32_t selectWeightedIndex(const WeightedValue<TValue, TWeight>*arrIn, size_t start, size_t length) {
        return selectWeightedIndex(arrIn, start, length, getTotalWeight(arrIn, start, length));
    }

    template<typename TValue, typename TWeight>
    int32_t selectWeightedIndex(const WeightedValue<TValue, TWeight>*arrIn, size_t start, size_t length, const TWeight total) {
        const TWeight randValue = randN<TWeight>() * total;

        TWeight prevAccu = 0;
        TWeight accu = 0;
        int32_t selectedID = 0;
        TWeight select = 0;
        for (size_t i = start; i < start + length; i++) {
            prevAccu = accu;
            accu += arrIn[i].getWeight();

            if (randValue <= accu && randValue >= prevAccu) {
                TWeight d = Math::abs(accu - randValue);
                if (d < select) {
                    select = d;
                    selectedID = i;
                }
            }
        }
        return selectedID;
    }

}