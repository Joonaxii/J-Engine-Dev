#pragma once
#include <type_traits>
#include <nlohmann/json.hpp>
#define json nlohmann::ordered_json

namespace JEngine::Serialization {
    const static int UNKNOWN_FILE = -1;
    const static int BINARY_FILE = 0;
    const static int JSON_FILE = 1;

    const static int BINARY_VERSION = 0;

    template<typename T>
    const void serializeBinary(std::ostream& stream, const T& ref) {
        static constexpr int offset = std::is_polymorphic<T>::value ? sizeof(size_t) : 0;
        stream.write(reinterpret_cast<const char*>(&ref) + offset, sizeof(T) - offset);
    }

    template<typename T>
    const void deserializeBinary(std::istream& stream, T& ref) {
        static constexpr int offset = std::is_polymorphic<T>::value ? sizeof(size_t) : 0;
        stream.read(reinterpret_cast<char*>(&ref) + offset, sizeof(T) - offset);
    }

    const void serializeWStringBinary(std::ostream& stream, const std::wstring& str);
    const std::wstring deserializeWStringBinary(std::istream& stream);

    const void serializeStringBinary(std::ostream& stream, const std::string& str);
    const std::string deserializeStringBinary(std::istream& stream);

    const int checkFileType(std::istream& stream, const char* header);
}