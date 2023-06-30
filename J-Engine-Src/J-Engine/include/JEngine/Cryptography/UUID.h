#pragma once
#include <iostream>
#include <cstdint>
#include <JEngine/IO/Serialization/Serializable.h>
#include <JEngine/Utility/Span.h>

namespace JEngine {
    struct UUID8 {
    public:
        static constexpr uint32_t INIT_VAL = 0xFFFFFFFFU;
        static constexpr uint32_t POLYNOMIAL = 0x82F63B78U;
        static constexpr size_t BLOCK_SIZE = 8192 << 3;

        static const UUID8 Empty;

        UUID8();
        UUID8(const UUID8& other);
        UUID8(const uint32_t a, const uint32_t b);
        UUID8(const uint64_t hash);
        UUID8(const std::string& str);
        UUID8(const char* str);
        UUID8(const char* str, const size_t length);

        const bool operator ==(const UUID8& other) const;
        const bool operator !=(const UUID8& other) const;

        operator bool() const { return *reinterpret_cast<const uint64_t*>(_hash) != 0; }

        friend std::ostream& operator<<(std::ostream& stream, const UUID8& hash);

        std::string toString() const;

        inline uint64_t asUInt() const { return *reinterpret_cast<const uint64_t*>(_hash); }

        UUID8& parse(const std::string& str, const UUID8& defaultValue = {});
        UUID8& parse(const char* str, const UUID8& defaultValue = {});
        UUID8& parse(const char* str, const size_t count, const UUID8& defaultValue = {});
        UUID8& parse(ConstSpan<char> str, const UUID8& defaultValue = {});

        UUID8& computeHash(const Stream& stream, const size_t blockSize = BLOCK_SIZE);
        UUID8& computeHash(const Stream& stream, Span<char> buffer);

        UUID8& computeHash(const char* data, const size_t size, const size_t blockSize = BLOCK_SIZE);

        UUID8& computeHash(Span<char> data, const size_t blockSize = BLOCK_SIZE);
        UUID8& computeHash(ConstSpan<char> data, const size_t blockSize = BLOCK_SIZE);

        UUID8& computeHash(Span<uint8_t> data, const size_t blockSize = BLOCK_SIZE);
        UUID8& computeHash(ConstSpan<uint8_t> data, const size_t blockSize = BLOCK_SIZE);

        friend yamlEmit& operator<<(yamlEmit& yamlOut, const UUID8& itemRef);

    private:
        friend struct Serializable<UUID8>;
        friend struct std::hash<JEngine::UUID8>;
        friend struct YAML::convert<JEngine::UUID8>;
        uint32_t _hash[2];
    };

    struct UUID16 {
    public:

        static const UUID16 Empty;

        UUID16();
        UUID16(const UUID16& other);
        UUID16(const uint32_t a, const uint32_t b, const int64_t len);
        UUID16(const uint64_t hash, const int64_t len);
        UUID16(const UUID8& hash, const int64_t len);
        UUID16(const std::string& str);
        UUID16(const char* str);
        UUID16(const char* str, const size_t length);

        const bool operator ==(const UUID16& other) const;
        const bool operator !=(const UUID16& other) const;

        operator bool() const { return bool(_hash) && _len != 0; }

        friend std::ostream& operator<<(std::ostream& stream, const UUID16& hash);

        std::string toString() const;

        inline uint64_t asUInt() const { return _hash.asUInt(); }
        UUID8 asUUID8() const { return _hash; }
        int64_t getLength() const { return _len; }

        UUID16& parse(const std::string& str, const UUID16& defaultValue = {});
        UUID16& parse(const char* str, const UUID16& defaultValue = {});
        UUID16& parse(const char* str, const size_t count, const UUID16& defaultValue = {});
        UUID16& parse(ConstSpan<char> str, const UUID16& defaultValue = {});

        UUID16& computeHash(const Stream& stream, const size_t blockSize = UUID8::BLOCK_SIZE);
        UUID16& computeHash(const Stream& stream, Span<char> buffer);

        UUID16& computeHash(const char* data, const size_t size, const size_t blockSize = UUID8::BLOCK_SIZE);

        UUID16& computeHash(Span<char> data, const size_t blockSize = UUID8::BLOCK_SIZE);
        UUID16& computeHash(ConstSpan<char> data, const size_t blockSize = UUID8::BLOCK_SIZE);

        UUID16& computeHash(Span<uint8_t> data, const size_t blockSize = UUID8::BLOCK_SIZE);
        UUID16& computeHash(ConstSpan<uint8_t> data, const size_t blockSize = UUID8::BLOCK_SIZE);

        friend yamlEmit& operator<<(yamlEmit& yamlOut, const UUID16& itemRef);

    private:
        friend struct Serializable<UUID16>;
        friend struct std::hash<JEngine::UUID16>;
        friend struct YAML::convert<JEngine::UUID16>;
        UUID8 _hash;
        int64_t _len;
    };
}


#pragma region Serialization

namespace JEngine {
    inline yamlEmit& operator<<(yamlEmit& yamlOut, const UUID8& itemRef) {
        yamlOut << YAML::Hex << *reinterpret_cast<const uint64_t*>(itemRef._hash);
        return yamlOut;
    }

    inline yamlEmit& operator<<(yamlEmit& yamlOut, const UUID16& itemRef) {
        yamlOut << YAML::BeginSeq << YAML::Flow << reinterpret_cast<const UUID8&>(itemRef) << YAML::Dec << itemRef._len<< YAML::EndSeq;
        return yamlOut;
    }
}
//YAML
namespace YAML {

    template<>
    struct convert<JEngine::UUID8> {
        static Node encode(const JEngine::UUID8& rhs) {
            Node node;
            node.push_back(*reinterpret_cast<const uint64_t*>(rhs._hash));
            return node;
        }

        static bool decode(const Node& node, JEngine::UUID8& rhs) {
            rhs = JEngine::UUID8(node.as<uint64_t>());
            return true;
        }
    };

    template<>
    struct convert<JEngine::UUID16> {
        static Node encode(const JEngine::UUID16& rhs) {
            Node node;
            node.push_back(rhs._hash);
            node.push_back(rhs._len);
            return node;
        }

        static bool decode(const Node& node, JEngine::UUID16& rhs) {
            if (!node.IsSequence() || node.size() < 2) { return false; }
            rhs = JEngine::UUID16(node[0].as<JEngine::UUID8>(), node[1].as<int64_t>());
            return true;
        }
    };
}

//JSON
namespace JEngine {
    template<>
    inline bool Serializable<UUID8>::deserializeJson(UUID8& itemRef, json& jsonF, const UUID8& defaultValue) {
        itemRef = jsonF.is_string() ? itemRef.parse(jsonF.get<std::string>(), defaultValue) : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<UUID8>::serializeJson(const UUID8& itemRef, json& jsonF) {
        jsonF = itemRef.toString();
        return true;
    }


    template<>
    inline bool Serializable<UUID16>::deserializeJson(UUID16& itemRef, json& jsonF, const UUID16& defaultValue) {
        itemRef = jsonF.is_string() ? itemRef.parse(jsonF.get<std::string>(), defaultValue) : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<UUID16>::serializeJson(const UUID16& itemRef, json& jsonF) {
        jsonF = itemRef.toString();
        return true;
    }
}
#pragma endregion

template<>
struct std::hash<JEngine::UUID8> {
    size_t operator()(const JEngine::UUID8& uuid) const {
        return *reinterpret_cast<const size_t*>(uuid._hash);
    }
};

template<>
struct std::hash<JEngine::UUID16> {
    size_t operator()(const JEngine::UUID16& uuid) const {
        return std::hash<JEngine::UUID8>()(uuid._hash) ^ uuid._len;
    }
};