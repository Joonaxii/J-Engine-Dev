#pragma once
#include <iostream>
#include <cstdint>
#include <JEngine/IO/Serialization/ISerializable.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Utility/ConstSpan.h>

namespace JEngine {
    struct FAH16 : public ISerializable<FAH16, 16> {
    public:

        FAH16();
        FAH16(const FAH16& other);
        FAH16(const uint32_t a, const uint32_t b, const int64_t len);
        FAH16(const std::string& str);

        const bool operator ==(const FAH16& other) const;
        const bool operator !=(const FAH16& other) const;

        friend std::ostream& operator<<(std::ostream& stream, const FAH16& hash);

        std::string toString() const;

        FAH16& parse(const std::string& str);
        FAH16& parse(const char* str, const size_t count);
        FAH16& parse(ConstSpan<char>& str);

        FAH16& computeHash(std::istream& stream, const size_t blockSize = BLOCK_SIZE);
        FAH16& computeHash(std::istream& stream, Span<char>& buffer);

        FAH16& computeHash(const char* data, const size_t size, const size_t blockSize = BLOCK_SIZE);
        FAH16& computeHash(Span<uint8_t> data, const size_t blockSize = BLOCK_SIZE);
        FAH16& computeHash(ConstSpan<uint8_t> data, const size_t blockSize = BLOCK_SIZE);

        const bool deserializeJson(json& jsonF);
        const bool serializeJson(json& jsonF);

    private:
        static constexpr uint32_t INIT_VAL = 0xFFFFFFFFU;
        static constexpr uint32_t POLYNOMIAL = 0x82F63B78U;
        static constexpr size_t BLOCK_SIZE = 8192 << 3;

        uint32_t _hash[2];
        int64_t _len;
       
        static const uint32_t compute(ConstSpan<uint8_t>& span, uint32_t crc);
    };
}