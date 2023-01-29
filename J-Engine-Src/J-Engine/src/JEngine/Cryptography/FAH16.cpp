#include <JEngine/Cryptography/FAH16.h>
#include <JEngine/Helpers/StringExtensions.h>
#include <sstream>

namespace JEngine {

    static constexpr size_t HASH_STR_LEN = 2 + (32);

    FAH16::FAH16() : _hash {0, 0}, _len(0) { }
    FAH16::FAH16(const FAH16& other) {
        _hash[0] = other._hash[0];
        _hash[1] = other._hash[1];
        _len = other._len;
    }
    FAH16::FAH16(const uint32_t a, const uint32_t b, const int64_t len) : _hash { a, b }, _len(len) { }
    FAH16::FAH16(const std::string& str) : _hash{ 0, 0 }, _len(0) {
        parse(str);
    }

    const bool FAH16::operator==(const FAH16& other) const {
        return (_hash[0] == other._hash[0]) && (_hash[1] == other._hash[1]);
    }

    const bool FAH16::operator!=(const FAH16& other) const {
        return (_hash[0] != other._hash[0]) || (_hash[1] != other._hash[1]);
    }

    std::ostream& operator<<(std::ostream& stream, const FAH16& hash) {
        stream << std::setw(8 ) << std::setfill('0') << std::hex << hash._hash[0] << '-';
        stream << std::setw(8 ) << std::setfill('0') << std::hex << hash._hash[1] << '-';
        stream << std::setw(16) << std::setfill('0') << std::hex << hash._len;
        return stream;
    }

    std::string FAH16::toString() const {
        std::stringstream sStrm;
        sStrm << *this;
        return sStrm.str();
    }

    FAH16& FAH16::parse(const std::string& str) {
        return parse(ConstSpan(str.c_str(), str.length()));
    }

    FAH16& FAH16::parse(const char* str, const size_t count) {
        return parse(ConstSpan(str, count));
    }

    FAH16& FAH16::parse(ConstSpan<char>& str) {
        if (str.length() < HASH_STR_LEN) { *this = FAH16(); return *this; }

        Helpers::tryParseHex(str.slice(0 , 8 ), _hash[0], true);
        Helpers::tryParseHex(str.slice(9 , 8 ), _hash[1], true);
        Helpers::tryParseHex(str.slice(18, 16), _len    , true);
        return *this;
    }

    FAH16& FAH16::computeHash(std::istream& stream, const size_t blockSize) {
        char* ptr = reinterpret_cast<char*>(_malloca(blockSize));

        computeHash(stream, Span<char>(ptr, blockSize));
        _freea(ptr);
        return *this;
    }

    FAH16& FAH16::computeHash(std::istream& stream, Span<char>& buffer) {
        _hash[0] = 0;
        _hash[1] = 0;

        const auto pA = stream.tellg();
        stream.seekg(0, std::istream::end);

        const auto pB = stream.tellg();
        stream.seekg(pA, std::istream::beg);
        _len = pB - pA;

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

        ConstSpan<uint8_t> cBuf(reinterpret_cast<uint8_t*>(buffer.begin()), bLen);
        int32_t cur = 0;
        while (true) {
            size_t rLen = stream.readsome(reinterpret_cast<char*>(buffer.begin()), bLen);
            if (rLen < 1) { break; }

            int32_t i = cur++ & 0x1;
            _hash[i] = compute(cBuf.slice(0, rLen), _hash[i]);
        }
        return *this;
    }

    FAH16& FAH16::computeHash(const char* data, const size_t size, const size_t blockSize) {
        return computeHash(ConstSpan<uint8_t>(reinterpret_cast<const uint8_t*>(data), size), blockSize);
    }

    FAH16& FAH16::computeHash(Span<uint8_t> span, const size_t blockSize) {
        return computeHash(ConstSpan<uint8_t>(span.begin(), span.end()), blockSize);
    }

    FAH16& FAH16::computeHash(ConstSpan<uint8_t> data, const size_t blockSize) {
        _hash[0] = 0;
        _hash[1] = 0;
        _len = data.length();

        int32_t cur = 0;
        int64_t pos = 0;
        while (true)
        {
            if (pos >= _len) { break; }

            uint64_t cLen = uint64_t(_len - pos);
            size_t len = cLen < blockSize ? cLen : blockSize;

            int32_t i = cur++ & 0x1;
            _hash[i] = compute(data.slice(pos, len), _hash[i]);
            pos += len;
        }
        return *this;
    }

    const bool FAH16::deserializeJson(json& jsonF) {
        jsonF.swap(json({ _hash[0], _hash[1] }));
        return true;
    }

    const bool FAH16::serializeJson(json& jsonF) {
        auto& val = jsonF.get<std::vector<uint64_t>>();

        if (val.size() < 2) { _hash[0] = 0; _hash[1] = 0; return false;; }

        _hash[0] = uint32_t((val[0] & 0xFFFFFFFFU));
        _hash[1] = uint32_t(val[0]);
        return true;
    }

    const uint32_t FAH16::compute(ConstSpan<uint8_t>& span, uint32_t crc) {
        crc = ~crc;
        size_t len = span.length();
        auto ptr = span.begin();

        while (len-- > 0)
        {
            crc ^= *ptr++;

            crc = (crc >> 1) ^ (POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (POLYNOMIAL & (0 - (crc & 1)));
            crc = (crc >> 1) ^ (POLYNOMIAL & (0 - (crc & 1)));
        }
        return ~crc;
    }
}
