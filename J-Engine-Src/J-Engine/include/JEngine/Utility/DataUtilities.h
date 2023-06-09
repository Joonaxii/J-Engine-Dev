#pragma once 
#include <functional>

namespace JEngine {
    namespace Data {
        static void adlerCRCBegin(uint32_t& sum1, uint32_t& sum2) {
            sum1 = 1;
            sum2 = 0;
        }

        static void adlerCRCUpdate(uint32_t& sum1, uint32_t& sum2, const void* data, size_t length) {
            static constexpr uint32_t MODULO = 65521;

            const uint8_t* dataB = reinterpret_cast<const uint8_t*>(data);
            for (size_t i = 0; i < length; i++) {
                sum1 = (sum1 + dataB[i]) % MODULO;
                sum2 = (sum1 + sum2) % MODULO;
            }
        }

        static uint32_t adlerCRCEnd(uint32_t& sum1, uint32_t& sum2) {
            return (sum2 << 16) | sum1;
        }

        static uint32_t calcAdlerCRC(uint32_t crc, const void* data, size_t length) {
            static constexpr uint32_t MODULO = 65521;

            const uint8_t* dataB = reinterpret_cast<const uint8_t*>(data);
            uint32_t sum1 = 1;
            uint32_t sum2 = 0;
            for (size_t i = 0; i < length; i++) {
                sum1 = (sum1 + dataB[i]) % MODULO;
                sum2 = (sum1 + sum2) % MODULO;
            }
            return (sum2 << 16) | sum1;
        }

        static uint32_t updateCRC(uint32_t crc, const void* data, size_t length) {
            if (length < 1 || !data) { return crc; }

            static bool isInit(false);
            static uint32_t table[256]{ 0 };
            static constexpr uint32_t POLYNOMIAL = 0xEDB88320U;

            if (!isInit) {
                for (size_t i = 0; i < 256; i++) {
                    uint32_t& c = table[i];
                    c = uint32_t(i);
                    for (size_t j = 0; j < 8; j++) {
                        if (c & 0x1) {
                            c = POLYNOMIAL ^ (c >> 1);
                        }
                        else {
                            c >>= 1;
                        }
                    }
                }
                isInit = true;
            }

            const uint8_t* dataB = reinterpret_cast<const uint8_t*>(data);
            for (size_t i = 0; i < length; i++) {
                crc = table[(crc ^ dataB[i]) & 0xFF] ^ (crc >> 8);
            }
            return crc;
        }


        template<typename T>
        static uint32_t updateCRC(uint32_t crc, const T& value) {
            return updateCRC(crc, &value, sizeof(T));
        }

        template<typename T>
        T read(const void* data) {
            return data ? *reinterpret_cast<const T*>(data) : T();
        }

        template<typename T>
        T& read(const void* data, T& value) {
            return value = read<T>(data);
        }

        static void reverseEndianess(void* buffer, size_t elementSize, size_t count) {
            if (elementSize < 2 || !buffer) { return; }
            uint8_t* data = reinterpret_cast<uint8_t*>(buffer);

            size_t size = count * elementSize;
            size_t half = elementSize >> 1;
            for (size_t i = 0, j = 0; i < count; i++, j += elementSize)
            {
                uint8_t* cur = data + j;
                for (size_t lo = 0, hi = elementSize - 1; lo < half; lo++, hi--)
                {
                    uint8_t c = cur[lo];
                    cur[lo] = cur[hi];
                    cur[hi] = c;
                }
            }
        }
        template<typename T>
        static void reverseEndianess(T* buffer, size_t count = 1) {
            if (sizeof(T) < 2 || !buffer) { return; }
            reverseEndianess(buffer, sizeof(T), count);
        }

    }

    struct DataHash {
        uint32_t size{0};
        uint32_t hash{0};

        DataHash() : size(0), hash(0) {}
        DataHash(uint32_t hash, uint32_t size) : size(size), hash(hash) {}
        DataHash(const void* data, uint32_t size) : size(size), hash(0) {
            hash = Data::updateCRC(0xFFFFFFFFU, data, size) ^ 0xFFFFFFFFU;
        }

        bool operator==(const DataHash& other) const { return *reinterpret_cast<const uint64_t*>(this) == *reinterpret_cast<const uint64_t*>(&other); }
        bool operator!=(const DataHash& other) const { return *reinterpret_cast<const uint64_t*>(this) != *reinterpret_cast<const uint64_t*>(&other); }

        operator uint64_t() const { return *reinterpret_cast<const uint64_t*>(this); }
    };

}

template<>
struct std::hash<JEngine::DataHash> {
    std::size_t operator()(const JEngine::DataHash& hash) const noexcept {
        return uint64_t(hash);
    }
};
