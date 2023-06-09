#include <JEngine/Cryptography/UUID.h>
#include <JEngine/Utility/StringHelpers.h>
#include <sstream>

namespace JEngine {
    static constexpr size_t UUID8_HASH_STR_LEN  = 1 + 16;
    static constexpr size_t UUID16_HASH_STR_LEN = 2 + 32;

    uint32_t compute(const ConstSpan<uint8_t> span, uint32_t crc) {
        crc = ~crc;
        size_t len = span.length();
        auto ptr = span.get();

        while (len-- > 0)
        {
            crc ^= *ptr++;

            crc = (crc >> 1) ^ (UUID8::POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (UUID8::POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (UUID8::POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (UUID8::POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (UUID8::POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (UUID8::POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (UUID8::POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (UUID8::POLYNOMIAL & (0 - (crc & 1)));
        }
        return ~crc;
    }

    const UUID8 UUID8::Empty{};

    UUID8::UUID8() : _hash {0, 0} { }
    UUID8::UUID8(const UUID8& other) {
        _hash[0] = other._hash[0];
        _hash[1] = other._hash[1];
    }
    UUID8::UUID8(const uint32_t a, const uint32_t b) : _hash { a, b } { }
    UUID8::UUID8(const uint64_t hash) : _hash { uint32_t(hash & 0xFFFFFFFFU), uint32_t(hash >> 32)} { }
    UUID8::UUID8(const std::string& str) : _hash{ 0, 0 } { parse(str); }

    UUID8::UUID8(const char* str) : _hash{ 0, 0 } { parse(str); }
    UUID8::UUID8(const char* str, const size_t size) : _hash{ 0, 0 } {parse(str); }

    const bool UUID8::operator==(const UUID8& other) const {
        return (_hash[0] == other._hash[0]) && (_hash[1] == other._hash[1]);
    }

    const bool UUID8::operator!=(const UUID8& other) const {
        return (_hash[0] != other._hash[0]) || (_hash[1] != other._hash[1]);
    }

    std::ostream& operator<<(std::ostream& stream, const UUID8& hash) {
        stream << std::setw(8) << std::setfill('0') << std::hex << hash._hash[0] << '-';
        stream << std::setw(8) << std::setfill('0') << std::hex << hash._hash[1];
        return stream;
    }

    std::string UUID8::toString() const {
        std::stringstream sStrm;
        sStrm << *this;
        return sStrm.str();
    }

    UUID8& UUID8::parse(const std::string& str, const UUID8& defaultValue) { return parse(ConstSpan(str.c_str(), str.length()), defaultValue); }
    UUID8& UUID8::parse(const char* str, const UUID8& defaultValue) { return parse(ConstSpan(str, strlen(str)), defaultValue); }
    UUID8& UUID8::parse(const char* str, const size_t count, const UUID8& defaultValue) { return parse(ConstSpan(str, count), defaultValue); }

    UUID8& UUID8::parse(const ConstSpan<char> str, const UUID8& defaultValue) {
        if (str.length() < UUID8_HASH_STR_LEN) { *this = defaultValue; return *this; }

        Helpers::tryParseHex(str.slice(0 , 8 ), _hash[0], true);
        Helpers::tryParseHex(str.slice(9 , 8 ), _hash[1], true);
        return *this;
    }

    UUID8& UUID8::computeHash(const Stream& stream, const size_t blockSize) {
        char* ptr = reinterpret_cast<char*>(_malloca(blockSize));

        computeHash(stream, Span<char>(ptr, blockSize));
        _freea(ptr);
        return *this;
    }

    UUID8& UUID8::computeHash(const Stream& stream, Span<char> buffer) {
        _hash[0] = 0;
        _hash[1] = 0;

        const auto pA = stream.tell();
        int64_t _len = stream.size() - pA;

        const int64_t bLen = int64_t(buffer.length());

        if (_len <= bLen) {
            size_t len = bLen / _len;
            if (len < 2)
            {
                buffer = buffer.slice(0, std::max<size_t>(bLen >> 1, 4));
            }
            else
            {
                buffer = buffer.slice(0, std::max<size_t>(len, 4));
            }
        }

        ConstSpan<uint8_t> cBuf(buffer.get(), bLen);
        int32_t cur = 0;
        while (true) {
            size_t rLen = stream.read(buffer.get(), bLen, false);
            if (rLen < 1) { break; }

            int32_t i = cur++ & 0x1;
            _hash[i] = compute(cBuf.slice(0, rLen), _hash[i]);
        }
        return *this;
    }

    UUID8& UUID8::computeHash(const char* data, const size_t size, const size_t blockSize) {
        return computeHash(ConstSpan<uint8_t>(data, size), blockSize);
    }

    UUID8& UUID8::computeHash(Span<char> span, const size_t blockSize) {
        return computeHash(span.castToConst<uint8_t>(), blockSize);
    }

    UUID8& UUID8::computeHash(ConstSpan<char> span, const size_t blockSize) {
        return computeHash(span.castTo<uint8_t>(), blockSize);
    }

    UUID8& UUID8::computeHash(Span<uint8_t> span, const size_t blockSize) {
        return computeHash(ConstSpan<uint8_t>(span), blockSize);
    }

    UUID8& UUID8::computeHash(const ConstSpan<uint8_t> data, const size_t blockSize) {
        _hash[0] = 0;
        _hash[1] = 0;

        int32_t cur = 0;
        int64_t pos = 0;
        int64_t totLen = data.length();
        while (pos < totLen)
        {
            uint64_t cLen = uint64_t(totLen - pos);
            size_t len = cLen < blockSize ? cLen : blockSize;

            int32_t i = cur++ & 0x1;
            _hash[i] = compute(data.slice(pos, len), _hash[i]);
            pos += len;
        }
        return *this;
    }

    const UUID16 UUID16::Empty{};

    UUID16::UUID16() : _hash(), _len() { }
    UUID16::UUID16(const UUID16& other) {
        _hash = other._hash;
        _len = other._len;
    }
    UUID16::UUID16(const uint32_t a, const uint32_t b, const int64_t len) : _hash(a, b), _len(len) { }
    UUID16::UUID16(const uint64_t hash, const int64_t len) : _hash(hash), _len(len) { }
    UUID16::UUID16(const UUID8& hash, const int64_t len) : _hash(hash), _len(len) { }
    UUID16::UUID16(const std::string& str) : _hash(), _len(0) { parse(str); }

    UUID16::UUID16(const char* str) : _hash() { parse(str); }
    UUID16::UUID16(const char* str, const size_t size) : _hash() { parse(str); }

    const bool UUID16::operator==(const UUID16& other) const {
        return (_hash == other._hash) && (_len == other._len);
    }

    const bool UUID16::operator!=(const UUID16& other) const {
        return (_hash != other._hash) || (_len != other._len);
    }

    std::ostream& operator<<(std::ostream& stream, const UUID16& hash) {
        stream << hash._hash << '-';
        stream << std::setw(16) << std::setfill('0') << std::hex << hash._len;
        return stream;
    }

    std::string UUID16::toString() const {
        std::stringstream sStrm;
        sStrm << *this;
        return sStrm.str();
    }

    UUID16& UUID16::parse(const std::string& str, const UUID16& defaultValue) { return parse(ConstSpan(str.c_str(), str.length()), defaultValue); }
    UUID16& UUID16::parse(const char* str, const UUID16& defaultValue) { return parse(ConstSpan(str, strlen(str)), defaultValue); }
    UUID16& UUID16::parse(const char* str, const size_t count, const UUID16& defaultValue) { return parse(ConstSpan(str, count), defaultValue); }

    UUID16& UUID16::parse(const ConstSpan<char> str, const UUID16& defaultValue) {
        if (str.length() < UUID16_HASH_STR_LEN) { *this = defaultValue; return *this; }

        _hash.parse(str);
        Helpers::tryParseHex(str.slice(17, 16), _len, true);
        return *this;
    }

    UUID16& UUID16::computeHash(const Stream& stream, const size_t blockSize) {
        char* ptr = reinterpret_cast<char*>(_malloca(blockSize));

        computeHash(stream, Span<char>(ptr, blockSize));
        _freea(ptr);
        return *this;
    }

    UUID16& UUID16::computeHash(const Stream& stream, Span<char> buffer) {
        _len = stream.size() - stream.tell();
        _hash.computeHash(stream, buffer);
        return *this;
    }

    UUID16& UUID16::computeHash(const char* data, const size_t size, const size_t blockSize) {
        return computeHash(ConstSpan<uint8_t>(data, size), blockSize);
    }

    UUID16& UUID16::computeHash(Span<char> span, const size_t blockSize) {
        return computeHash(span.castToConst<uint8_t>(), blockSize);
    }

    UUID16& UUID16::computeHash(ConstSpan<char> span, const size_t blockSize) {
        return computeHash(span.castTo<uint8_t>(), blockSize);
    }

    UUID16& UUID16::computeHash(Span<uint8_t> span, const size_t blockSize) {
        return computeHash(ConstSpan<uint8_t>(span), blockSize);
    }

    UUID16& UUID16::computeHash(ConstSpan<uint8_t> data, const size_t blockSize) {
        _len = data.length();
        _hash.computeHash(data, blockSize);
        return *this;
    }
}
