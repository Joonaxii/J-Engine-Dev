#pragma once
#include <iostream>
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Utility/Span.h>
#include <JEngine/Utility/ConstSpan.h>
#include <functional>

namespace JEngine {
    struct FAH16 {
    public:

        FAH16();
        FAH16(const FAH16& other);
        FAH16(const uint32_t a, const uint32_t b, const int64_t len);
        FAH16(const std::string& str);
        FAH16(const char* str);
        FAH16(const char* str, const size_t length);

        const bool operator ==(const FAH16& other) const;
        const bool operator !=(const FAH16& other) const;

        friend std::ostream& operator<<(std::ostream& stream, const FAH16& hash);

        std::string toString() const;

        FAH16& parse(const std::string& str, const FAH16& defaultValue = {});
        FAH16& parse(const char* str, const FAH16& defaultValue = {});
        FAH16& parse(const char* str, const size_t count, const FAH16& defaultValue = {});
        FAH16& parse(ConstSpan<char>& str, const FAH16& defaultValue = {});

        FAH16& computeHash(std::istream& stream, const size_t blockSize = BLOCK_SIZE);
        FAH16& computeHash(std::istream& stream, Span<char>& buffer);

        FAH16& computeHash(const char* data, const size_t size, const size_t blockSize = BLOCK_SIZE);

        FAH16& computeHash(Span<char> data, const size_t blockSize = BLOCK_SIZE);
        FAH16& computeHash(ConstSpan<char> data, const size_t blockSize = BLOCK_SIZE);

        FAH16& computeHash(Span<uint8_t> data, const size_t blockSize = BLOCK_SIZE);
        FAH16& computeHash(ConstSpan<uint8_t> data, const size_t blockSize = BLOCK_SIZE);

    private:
        static constexpr uint32_t INIT_VAL = 0xFFFFFFFFU;
        static constexpr uint32_t POLYNOMIAL = 0x82F63B78U;
        static constexpr size_t BLOCK_SIZE = 8192 << 3;

        friend struct Serializable<FAH16>;
        friend struct std::hash<JEngine::FAH16>;
        uint32_t _hash[2];
        int64_t _len;
       
        static const uint32_t compute(ConstSpan<uint8_t>& span, uint32_t crc);
    };

#pragma region Serialization
    template<>
    inline bool Serializable<FAH16>::deserializeJson(FAH16& itemRef, json& jsonF, const FAH16& defaultValue) {
        itemRef = jsonF.is_string() ? itemRef.parse(jsonF.get<std::string>(), defaultValue) : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<FAH16>::serializeJson(const FAH16& itemRef, json& jsonF) {
        jsonF.update(itemRef.toString());
        return true;
    }
#pragma endregion

}

template<>
struct std::hash<JEngine::FAH16> {
    size_t operator()(const JEngine::FAH16& fah) const {
        return (fah._hash[0] ^ fah._hash[1]) ^ fah._len;
    }
};