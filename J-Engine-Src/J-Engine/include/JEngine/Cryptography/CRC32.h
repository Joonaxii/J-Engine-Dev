#pragma once 
#include <cstdint>

namespace JEngine {
    struct CRC32 {
        static constexpr uint32_t POLYNOMIAL = 0xEDB88320;
    public:
        static void generateTable(uint32_t table[256]) {
            for (size_t i = 0; i < 256; i++)
            {
                uint32_t c = uint32_t(i);
                for (size_t j = 0; j < 8; j++)
                {
                    c = (c & 0x1) ? POLYNOMIAL ^ (c >> 1) : c >> 1;
                }
                table[i] = c;
            }
        }

        static const uint32_t compute(const void* buffer, const size_t size) {
            if (!buffer) { return 0x0000; }
            if (!_init) {
                _init = true;
                generateTable(_table);
            }

            uint32_t crc = 0x000000;
            const uint8_t* data = reinterpret_cast<const uint8_t*>(buffer);
            for (size_t i = 0; i < size; i++)
            {
                crc = _table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
            }
            return crc ^ 0xFFFFFFFF;
        }

        static const uint32_t update(const uint32_t initial, const void* buffer, const size_t size) {
            if (!buffer) { return initial; }
            if (!_init) {
                _init = true;
                generateTable(_table);
            }

            uint32_t crc = initial ^ 0xFFFFFFFF;
            const uint8_t* data = reinterpret_cast<const uint8_t*>(buffer);
            for (size_t i = 0; i < size; i++)
            {
                crc = _table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
            }
            return crc ^ 0xFFFFFFFF;
        }

        static const uint32_t update(const uint32_t table[256], const uint32_t initial, const void* buffer, const size_t size) {
            if (!buffer) { return initial; }
            
            uint32_t crc = initial ^ 0xFFFFFFFF;
            const uint8_t* data = reinterpret_cast<const uint8_t*>(buffer);
            for (size_t i = 0; i < size; i++)
            {
                crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
            }
            return crc ^ 0xFFFFFFFF;
        }

    private:
        static inline bool _init = false;
        static inline uint32_t _table[256];
    };
}