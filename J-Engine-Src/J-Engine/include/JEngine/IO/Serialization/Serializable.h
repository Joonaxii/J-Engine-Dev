#pragma once
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <JEngine/IO/Stream.h>
using json = nlohmann::ordered_json;
using yamlEmit  = YAML::Emitter;
using yamlNode  = YAML::Node;

namespace JEngine {
    namespace Serialization {
        static inline void serializeWCStringBinary(const Stream& stream, const wchar_t* str) {
            size_t len(wcslen(str));
            stream.write(&len, sizeof(uint32_t));
            stream.write(reinterpret_cast<const char*>(str), len * sizeof(wchar_t));
        }
        static inline wchar_t* deserializeWCStringBinary(const Stream& stream) {
            size_t len = 0;
            stream.read(&len, sizeof(uint32_t), false);
            if (len < 1) { return nullptr; }

            wchar_t* buf = reinterpret_cast<wchar_t*>(malloc((len + 1) * sizeof(wchar_t)));
            if (buf) {
                buf[len] = 0;
                stream.read(buf, len * sizeof(wchar_t), false);
            }
            return buf;
        }
                      
        static inline void serializeCStringBinary(const Stream& stream, const char* str) {
            size_t len(strlen(str));
            stream.write(&len, sizeof(uint32_t));
            stream.write(str, len * sizeof(char));
        }
        static inline char* deserializeCStringBinary(const Stream& stream) {
            size_t len = 0;
            stream.read(&len, sizeof(uint32_t), false);
            if (len < 1) { return nullptr; }

            char* buf = reinterpret_cast<char*>(malloc(len + 1));
            if (buf) {
                buf[len] = 0;
                stream.read(buf, len * sizeof(wchar_t), false);
            }
            return buf;
        }

        static inline void serializeWStringBinary(const Stream& stream, const std::wstring& str) {
            size_t len(str.length());
            stream.write(&len, sizeof(uint32_t));
            stream.write(str.c_str(), len * sizeof(wchar_t));
        }
        static inline std::wstring deserializeWStringBinary(const Stream& stream) {
            uint32_t len = 0;
            stream.readValue(len, false);;
            if (len < 1) { return L""; }

            wchar_t* buf = reinterpret_cast<wchar_t*>(_malloca(len * sizeof(wchar_t)));
            std::wstring str{};
            if (buf) {
                stream.read(buf, len * sizeof(wchar_t), false);
                str.reserve(len);
                memcpy(&str[0], buf, sizeof(wchar_t) * len);
                _freea(buf);
            }
            return str;
        }

        static inline void serializeStringBinary(const Stream& stream, const std::string& str) {
            size_t len = str.length();
            stream.write(&len, sizeof(uint32_t));
            if (len < 1) { return; }
            stream.write(str.c_str(), len);
        }
        static inline std::string deserializeStringBinary(const Stream& stream) {
            uint32_t len = 0;
            stream.readValue(len, false);
            if (len < 1) { return ""; }

            char* buf = reinterpret_cast<char*>(_malloca(len));

            std::string str("");
            if (buf) {
                stream.read(buf, len, false);
                str = std::string(buf, len);

                str.reserve(len);
                memcpy(&str[0], buf, len);
                _freea(buf);
            }
            return str;
        }

        template<typename T>
        void serializeBinary(const T& ref, const Stream& stream) {
            if (std::is_polymorphic<T>::value) {
                stream.write(reinterpret_cast<const char*>(&ref) + 8, sizeof(T) - 8, false);
                return;
            }
            stream.writeValue(ref, 1, false);
        }

        template<typename T>
        void deserializeBinary(T& ref, const Stream& stream) {
            if (std::is_polymorphic<T>::value) {
                stream.read(reinterpret_cast<char*>(&ref) + 8, sizeof(T) - 8, false);
                return;
            }
            stream.readValue(ref, false);
        }
    }

    template<typename T>
    class ISerializable {
        virtual bool deserializeJson(json& jsonF) { return false; }
        virtual bool serializeJson(json& jsonF) const { return false; }

        virtual bool deserializeYaml(const yamlNode& yamlIn) { return false; }
        virtual bool serializeYaml(yamlEmit& yamlOut) const { return false; }

        virtual bool deserializeBinary(const Stream& stream) { return false; }
        virtual bool serializeBinary(const Stream& stream) const { return false; }
    };

    template<typename T>
    struct Serializable {
        static bool deserializeJson(T& itemRef, const json& jsonF, const T& defaultVal) {
            static_assert("JSON Deserialization not implemented for this type!");
            return false;
        }

        static bool serializeJson(const T& itemRef, json& jsonF) {
            jsonF = itemRef;
            return true;
        }

        static bool deserializeBinary(T& itemRef, const Stream& stream) {
            Serialization::deserializeBinary(itemRef, stream);
            return true;
        }

        static bool serializeBinary(const T& itemRef, const Stream& stream) {
            Serialization::serializeBinary(itemRef, stream);
            return true;
        }
    };

#pragma region Primitive Values

    template<>
    inline bool Serializable<float>::deserializeJson(float& itemRef, const json& jsonF, const float& defaultValue) {
        itemRef = jsonF.is_number_float() ? jsonF.get<float>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<float>::serializeJson(const float& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<double>::deserializeJson(double& itemRef, const json& jsonF, const double& defaultValue) {
        itemRef = jsonF.is_number_float() ? jsonF.get<double>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<double>::serializeJson(const double& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<uint8_t>::deserializeJson(uint8_t& itemRef, const json& jsonF, const uint8_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<uint8_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<uint8_t>::serializeJson(const uint8_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<uint16_t>::deserializeJson(uint16_t& itemRef, const json& jsonF, const uint16_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<uint16_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<uint16_t>::serializeJson(const uint16_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<uint32_t>::deserializeJson(uint32_t& itemRef, const json& jsonF, const uint32_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<uint32_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<uint32_t>::serializeJson(const uint32_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<uint64_t>::deserializeJson(uint64_t& itemRef, const json& jsonF, const uint64_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<uint64_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<uint64_t>::serializeJson(const uint64_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<int8_t>::deserializeJson(int8_t& itemRef, const json& jsonF, const int8_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<int8_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<int8_t>::serializeJson(const int8_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<int16_t>::deserializeJson(int16_t& itemRef, const json& jsonF, const int16_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<int16_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<int16_t>::serializeJson(const int16_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<int32_t>::deserializeJson(int32_t& itemRef, const json& jsonF, const int32_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<int32_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<int32_t>::serializeJson(const int32_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<int64_t>::deserializeJson(int64_t& itemRef, const json& jsonF, const int64_t& defaultValue) {
        itemRef = jsonF.is_number_integer() ? jsonF.get<int64_t>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<int64_t>::serializeJson(const int64_t& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<bool>::deserializeJson(bool& itemRef, const json& jsonF, const bool& defaultValue) {
        itemRef = jsonF.is_boolean() ? jsonF.get<bool>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<bool>::serializeJson(const bool& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<std::string>::deserializeJson(std::string& itemRef, const json& jsonF, const std::string& defaultValue) {
        itemRef = jsonF.is_string() ? jsonF.get<std::string>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<std::string>::serializeJson(const std::string& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<std::string>::deserializeBinary(std::string& itemRef, const Stream& stream) {
        itemRef = Serialization::deserializeStringBinary(stream);
        return true;
    }

    template<>
    inline bool Serializable<std::string>::serializeBinary(const std::string& itemRef, const Stream& stream) {
        Serialization::serializeStringBinary(stream, itemRef);
        return true;
    }

    template<>
    inline bool Serializable<std::wstring>::deserializeJson(std::wstring& itemRef, const json& jsonF, const std::wstring& defaultValue) {
        itemRef = jsonF.is_string() ? jsonF.get<std::wstring>() : defaultValue;
        return true;
    }

    template<>
    inline bool Serializable<std::wstring>::serializeJson(const std::wstring& itemRef, json& jsonF) {
        jsonF = itemRef;
        return true;
    }

    template<>
    inline bool Serializable<std::wstring>::deserializeBinary(std::wstring& itemRef, const Stream& stream) {
        itemRef = Serialization::deserializeWStringBinary(stream);
        return true;
    }

    template<>
    inline bool Serializable<std::wstring>::serializeBinary(const std::wstring& itemRef, const Stream& stream) {
        Serialization::serializeWStringBinary(stream, itemRef);
        return true;
    }
#pragma endregion

    namespace Serialization {

        template<typename T>
        bool serialize(const T& itemRef, json& jsonF) {
            return Serializable<T>::serializeJson(itemRef, jsonF);
        }

        template<typename T>
        bool serialize(const T& itemRef, const Stream& stream) {
            return Serializable<T>::serializeBinary(itemRef, stream);
        }

        template<typename T>
        bool serialize(const T& itemRef, yamlEmit& yamlOut) {
            yamlOut << itemRef;
            return true;
        }

        template<typename T>
        bool serialize(const char* key, const T& itemRef, yamlEmit& yamlOut) {
            yamlOut << YAML::Key << key << YAML::Value << itemRef;
            return true;
        }

        template<typename T>
        bool serialize(const T* itemRef, const size_t count, yamlEmit& yamlOut) {
            yamlOut << YAML::BeginSeq;
            if (itemRef) {
                for (size_t i = 0; i < count; i++) {
                    yamlOut << itemRef[i];
                }
            }
            yamlOut << YAML::EndSeq;
            return true;
        }

        template<typename T>
        bool serialize(const char* key, const T* itemRef, const size_t count, yamlEmit& yamlOut) {
            yamlOut << YAML::Key << key << YAML::Value;
            return serialize<T>(itemRef, count, yamlOut);
        }

        template<typename T>
        bool deserialize(T& itemRef, const json& jsonF, const T& defaultVal = T()) {
            return Serializable<T>::deserializeJson(itemRef, jsonF, defaultVal);
        }

        template<typename T>
        bool deserialize(T& itemRef, const yamlNode& yamlIn, const T& defaultVal = T()) {
            if (!YAML::convert<T>::decode(yamlIn, itemRef)) {
                itemRef = defaultVal;
                return false;
            }
            return true;
        }

        template<typename T>
        bool deserialize(const yamlNode& yamlIn, const T& defaultVal = T()) {
            T val = defaultVal;
            if (!YAML::convert<T>::decode(yamlIn, val)) {
                return false;
            }
            return val;
        }

        template<typename T>
        bool deserialize(std::vector<T>& itemRefs, const yamlNode& yamlInp, const T& defaultVal = T()) {
            if (yamlInp.IsSequence()) {
                size_t len = itemRefs.size();
                for (const yamlNode& obj : yamlInp) {
                    itemRefs.push_back(T());
                    deserialize<T>(itemRefs[len++], yamlInp, defaultVal);
                }
            }
            return true;
        }

        template<typename T>
        bool deserialize(const char* key, T& itemRef, const yamlNode& yamlIn, const T& defaultVal = T()) {
            auto& node = yamlIn[key];
            if (!node) {
                itemRef = defaultVal;
                return false;
            }
            return deserialize<T>(itemRef, node, defaultVal);
        }

        template<typename T>
        bool deserialize(const char* key, std::vector<T>& itemRefs, const yamlNode& yamlIn, const T& defaultVal = T()) {
            auto& node = yamlIn[key];
            if (node) {
                deserialize<T>(itemRefs, node, defaultVal);
                return true;
            }
            return false;
        }

        template<typename T>
        bool deserialize(T& itemRef, const Stream& stream) {
            return Serializable<T>::deserializeBinary(itemRef, stream);
        }

        template<typename T>
        bool jsonToBinary(const json& jsonF, const Stream& stream) {
            T temp{};
            deserialize(temp, jsonF);
            return serialize(temp, stream);
        }

        template<typename T>
        bool yamlToBinary(const yamlNode& yamlIn, const Stream& stream) {
            T temp{};
            deserialize(temp, yamlIn);
            return serialize(temp, stream);
        }
    }
}