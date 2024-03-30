#pragma once
#include <cstdint>
#include <JEngine/Platform.h>
#include <immintrin.h>

#define SIMD_ALIGN ALIGNAS(16)

namespace JEngine {
    struct SIMDIndex {
        size_t byteIdx{};
        uint8_t bitIdx{};
    };

    struct SIMD {
        enum : size_t {
            npos
        };

        static size_t findFirstZero(const uint8_t* buffer, size_t count) {
            size_t simdBufs = count >> 4;
            size_t leftOver = count - (simdBufs << 4);

            size_t pos = 0;
            const __m128i* mmBuf = reinterpret_cast<const __m128i*>(buffer);
            while (simdBufs > 0) {

                simdBufs--;
            }
        }

        FORCE_INLINE static constexpr uint32_t getSIMDMaskAND_UI32(const __m128& simd) {
            return simd.m128_u32[0] & simd.m128_u32[1] & simd.m128_u32[2] & simd.m128_u32[3];
        }

        FORCE_INLINE static constexpr uint64_t getSIMDMaskAND_UI64(const __m128& simd) {
            return simd.m128_u64[0] & simd.m128_u64[1];
        }
    };   
}