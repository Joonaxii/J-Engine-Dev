#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::ordered_json;

namespace JEngine {
    namespace Serialization {

        static inline void serializeWCStringBinary(std::ostream& stream, const wchar_t* str) {
            const uint64_t len = wcslen(str);
            stream.write(reinterpret_cast<const char*>(&len), sizeof(int32_t));
            stream.write(reinterpret_cast<const char*>(str), len * sizeof(wchar_t));
        }
        static inline wchar_t* deserializeWCStringBinary(std::istream& stream) {
            int32_t len = 0;
            stream.read(reinterpret_cast<char*>(&len), sizeof(int32_t));
            if (len < 1) { return L""; }

            char* buf = reinterpret_cast<char*>(malloc(len * sizeof(wchar_t) + 1));
            buf[len * sizeof(wchar_t)] = 0;

            stream.read(buf, len * sizeof(wchar_t));
            return reinterpret_cast<wchar_t*>(buf);
        }
                      
        static inline void serializeCStringBinary(std::ostream& stream, const char* str) {
            const uint64_t len = strlen(str);
            stream.write(reinterpret_cast<const char*>(&len), sizeof(int32_t));
            stream.write(reinterpret_cast<const char*>(str), len * sizeof(char));
        }
        static inline char* deserializeCStringBinary(std::istream& stream) {
            int32_t len = 0;
            stream.read(reinterpret_cast<char*>(&len), sizeof(int32_t));
            if (len < 1) { return ""; }

            char* buf = reinterpret_cast<char*>(malloc(len + 1));
            buf[len] = 0;

            stream.read(buf, len * sizeof(wchar_t));
            return buf;
        }
                      
        static inline void serializeWStringBinary(std::ostream& stream, const std::wstring& str) {
            const uint64_t len = str.length();
            stream.write(reinterpret_cast<const char*>(&len), sizeof(int32_t));
            stream.write(reinterpret_cast<const char*>(str.c_str()), len * sizeof(wchar_t));
        }
        static inline std::wstring deserializeWStringBinary(std::istream& stream) {
            int len = 0;
            stream.read(reinterpret_cast<char*>(&len), sizeof(int));
            if (len < 1) { return L""; }

            char* buf = reinterpret_cast<char*>(_malloca(len * sizeof(wchar_t)));
            stream.read(buf, len * sizeof(wchar_t));
            std::wstring str = std::wstring(reinterpret_cast<wchar_t*>(buf), len);
            _freea(buf);
            return str;
        }
                      
        static inline void serializeStringBinary(std::ostream& stream, const std::string& str) {
            uint64_t len = str.length();
            stream.write(reinterpret_cast<char*>(&len), sizeof(int32_t));
            if (len < 1) { return; }
            stream.write(str.c_str(), len);
        }
        static inline std::string deserializeStringBinary(std::istream& stream) {
            uint64_t len = 0;
            stream.read(reinterpret_cast<char*>(&len), sizeof(int32_t));
            if (len < 1) { return ""; }

            char* buf = reinterpret_cast<char*>(_malloca(len));

            std::string str("");
            if (buf) {
                stream.read(buf, len);
                str = std::string(buf, len);
            }
            _freea(buf);
            return str;
        }

        template<typename T>
        const void serializeBinary(const T& ref, std::ostream& stream) {
            static constexpr int offset = std::is_polymorphic<T>::value ? sizeof(size_t) : 0;
            stream.write(reinterpret_cast<const char*>(&ref) + offset, sizeof(T) - offset);
        }

        template<typename T>
        const void deserializeBinary(T& ref, std::istream& stream) {
            static constexpr int offset = std::is_polymorphic<T>::value ? sizeof(size_t) : 0;
            stream.read(reinterpret_cast<char*>(&ref) + offset, sizeof(T) - offset);
        }
    }

    template<typename T>
    class ISerializable {
        virtual bool deserializeJson(json& jsonF) { return false; }
        virtual bool serializeJson(json& jsonF) const { return false; }

        virtual bool deserializeBinary(std::istream& stream) { return false; }
        virtual bool serializeBinary(std::ostream& stream) const { return false; }

        static bool jsonToBinary(json& jsonF, std::ostream& stream) { return jsonToBinaryImpl(jsonF, stream); }

    protected:
        virtual bool jsonToBinaryImpl(json& jsonF, std::ostream& stream) const = 0;
    };

    template<typename T>
    struct Serializable {
        static bool deserializeJson(T& itemRef, json& jsonF, const T& defaultVal) {
            static_assert("JSON Deserialization not implemented for this type!");
            return false;
        }

        static bool serializeJson(const T& itemRef, json& jsonF) {
            static_assert("JSON Serialization not implemented for this type!");
            return false;
        }

        static bool deserializeBinary(T& itemRef, std::istream& stream) {
            Serialization::deserializeBinary(itemRef, stream);
            return true;
        }

        static bool serializeBinary(const T& itemRef, std::ostream& stream) {
            Serialization::serializeBinary(itemRef, stream);
            return true;
        }
    };

#pragma region Primitive Values

    template<>
    inline bool Serializable<float>::deserializeJson(float& itemRef, json& jsonF, const float& defaultValue) {
        itemRef = jsonF.is_number_float() ? jsonF.get<float>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<float>::serializeJson(const float& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<double>::deserializeJson(double& itemRef, json& jsonF, const double& defaultValue) {
        itemRef = jsonF.is_number_float() ? jsonF.get<double>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<double>::serializeJson(const double& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<uint8_t>::deserializeJson(uint8_t& itemRef, json& jsonF, const uint8_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<uint8_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<uint8_t>::serializeJson(const uint8_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<uint16_t>::deserializeJson(uint16_t& itemRef, json& jsonF, const uint16_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<uint16_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<uint16_t>::serializeJson(const uint16_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<uint32_t>::deserializeJson(uint32_t& itemRef, json& jsonF, const uint32_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<uint32_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<uint32_t>::serializeJson(const uint32_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<uint64_t>::deserializeJson(uint64_t& itemRef, json& jsonF, const uint64_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<uint64_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<uint64_t>::serializeJson(const uint64_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<int8_t>::deserializeJson(int8_t& itemRef, json& jsonF, const int8_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<int8_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<int8_t>::serializeJson(const int8_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<int16_t>::deserializeJson(int16_t& itemRef, json& jsonF, const int16_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<int16_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<int16_t>::serializeJson(const int16_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<int32_t>::deserializeJson(int32_t& itemRef, json& jsonF, const int32_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<int32_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<int32_t>::serializeJson(const int32_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<int64_t>::deserializeJson(int64_t& itemRef, json& jsonF, const int64_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<int64_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<int64_t>::serializeJson(const int64_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<bool>::deserializeJson(bool& itemRef, json& jsonF, const bool& defaultValue) {
        itemRef = jsonF.is_boolean() ? jsonF.get<bool>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<bool>::serializeJson(const bool& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<std::string>::deserializeJson(std::string& itemRef, json& jsonF, const std::string& defaultValue) {
        itemRef = jsonF.is_string() ? jsonF.get<std::string>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<std::string>::serializeJson(const std::string& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<std::string>::deserializeBinary(std::string& itemRef, std::istream& stream)  {
        itemRef = Serialization::deserializeStringBinary(stream);
        return true;
    }

    template<>
    inline bool Serializable<std::string>::serializeBinary(const std::string& itemRef, std::ostream& stream) {
        Serialization::serializeStringBinary(stream, itemRef);
        return true;
    }

    template<>
    inline bool Serializable<std::wstring>::deserializeJson(std::wstring& itemRef, json& jsonF, const std::wstring& defaultValue) {
        itemRef = jsonF.is_string() ? jsonF.get<std::wstring>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<std::wstring>::serializeJson(const std::wstring& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<std::wstring>::deserializeBinary(std::wstring& itemRef, std::istream& stream) {
        itemRef = Serialization::deserializeWStringBinary(stream);
        return true;
    }

    template<>
    inline bool Serializable<std::wstring>::serializeBinary(const std::wstring& itemRef, std::ostream& stream) {
        Serialization::serializeWStringBinary(stream, itemRef);
        return true;
    }
#pragma endregion

    namespace Serialization {

        template<typename T>
        static bool serialize(const T& itemRef, json& jsonF) {
            return Serializable<T>::serializeJson(itemRef, jsonF);
        }

        template<typename T>
        static bool serialize(const T& itemRef, std::ostream& stream) {
            return Serializable<T>::serializeBinary(itemRef, stream);
        }

        template<typename T>
        static bool deserialize(T& itemRef, json& jsonF, const T& defaultVal = T()) {
            return Serializable<T>::deserializeJson(itemRef, jsonF, defaultVal);
        }

        template<typename T>
        static bool deserialize(T& itemRef, std::istream& stream) {
            return Serializable<T>::deserializeBinary(itemRef, stream);
        }

        template<typename T>
        static bool jsonToBinary(json& jsonF, std::ostream& stream) {
            T temp{};
            deserialize(temp, jsonF);
            return serialize(temp, stream);
        }
    }
}