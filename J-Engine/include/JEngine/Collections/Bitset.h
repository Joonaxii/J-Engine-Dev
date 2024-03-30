#pragma once
#include <JEngine/Core/Memory.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
    namespace detail {
        struct Index {
            uint64_t index{};
            int32_t bit{};

            constexpr Index() : index(0), bit(0) {}
            constexpr Index(uint64_t index, int32_t bit) : index(index), bit(bit) {}

            FORCE_INLINE constexpr bool operator==(const Index& other) const {
                return index == other.index && bit == other.index;
            }

            FORCE_INLINE constexpr bool operator!=(const Index& other) const {
                return index != other.index || bit != other.index;
            }
        };

        struct IDXMarkType {
            enum : uint8_t {
                MARK_SUCCESS = 0x0,
                MARK_ALREADY_MARKED = 0x1,
                MARK_OUT_OF_RANGE = 0x2,
                MARK_INDEX_INVALID = 0x3,
            };

            uint8_t result{};
            Index index{};

            constexpr IDXMarkType() : result(0), index() {}
            constexpr IDXMarkType(uint8_t result, const Index& index) : result(result), index(index) {}
        };

        static constexpr float INDEX_MAX_EXPANSION = 1.6f;
        static constexpr Index INVALID_INDEX = Index(0xFFFFFFFFU, -1);

        FORCE_INLINE constexpr uint32_t getExpandedSize(uint32_t current) {
            return current + Math::max(uint32_t(INDEX_MAX_EXPANSION * current), 1U);
        }

        FORCE_INLINE constexpr Index extractIndex(uint64_t index) {
            const uint64_t lInd = index >> 6;
            return Index(lInd, int32_t(index - (lInd << 6)));
        }

        FORCE_INLINE constexpr uint64_t to1DIndex(const Index& index) {
            return (index.index << 6) + index.bit;
        }
    }

    template<size_t size, bool useSimd = true>
    class FixedBitset {
    public:
        constexpr FixedBitset() : _buffer{ 0 }, _count(0) {}
        constexpr size_t size() const { return _count; }
    private:
        static constexpr uint64_t BUF_CAPACITY = useSimd ? Math::nextDivByPowOf2<uint64_t, 128>(size) >> 3 : Math::nextDivByPowOf2<uint64_t, 8>(size) >> 3;
        alignas(16) uint8_t _buffer[BUF_CAPACITY];
        size_t _count;
    };

    class DynamicBitset {
    public:

    private:

    };
}