#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#define json nlohmann::ordered_json

//////////////////////////////////////////////////////
//                      NOTES                       //
//   The serialize- & deserialize methods are not   //
//   marked as virtual, instead the struct is       //
//   templated & when calling the (de)serialize     //
//   methods, a static_cast is done to find the     //
//   type specific implementation. This removes     //
//   the need to have the VTable pointer at the     //
//   beginning of each type that derives from       //
//   ISerializable, having extra bytes be there for //
//   essentially no reason for types that are meant //
//   to have a specific byte size is not good.      //
//                                                  //
//////////////////////////////////////////////////////

namespace JEngine {
    template<typename T, size_t size = 0>
    struct ISerializable {
        static constexpr size_t BIN_SIZE = size;

        const bool deserializeJson(json& jsonF, const bool check = false) {
            if (check) {
                return static_cast<T*>(this)->deserializeJson(jsonF);
            }

            static constexpr size_t UINTS = REQUIRED_UINT64 < 1 ? 1 : REQUIRED_UINT64;
            uint8_t* temp = reinterpret_cast<uint8_t*>(this);
            uint64_t buffer[UINTS]{ 0 };

            for (size_t i = 0; i < jsonF.size(); i++) {
                buffer[i] = jsonF[i].get<uint64_t>();
            }

            memcpy(this, buffer, size);
            return UINTS;
        }
        const bool serializeJson(json& jsonF, const bool check = false) const {
            if (check) {
                return static_cast<const T*>(this)->serializeJson(jsonF);
            }

            static constexpr size_t UINTS = REQUIRED_UINT64 < 1 ? 1 : REQUIRED_UINT64;
            uint8_t* temp = reinterpret_cast<uint8_t*>(this);
            uint64_t buffer[UINTS]{ 0 };

            int tempC = 0;
            for (size_t i = 0; i < UINTS; i++) {
                for (size_t j = 0, k = 0; j < length; j++, k += 8) {
                    buffer[i] |= static_cast<uint64_t>(temp[tempC++]) << k;
                    if (tempC >= size) { goto end; }
                }
            }
        end:

            if (UINTS > 1) {
                jsonF.update(json(json::array_t(std::vector<uint64_t>(buffer, buffer + UINTS))), false);
            }
            else {
                jsonF.update(json(buffer[0]), false);
            }
            return UINTS;
        }

        const bool deserializeBinary(std::istream& stream, const bool check = false) {
            if (check) {
                return static_cast<T*>(this)->deserializeBinary(stream);
            }
            stream.read(reinterpret_cast<char*>(this), size);
            return true;
        }

        const bool serializeBinary(std::ostream& stream, const bool check = false) const {
            if (check) {
                return static_cast<const T*>(this)->serializeBinary(stream);
            }
            stream.write(reinterpret_cast<const char*>(this), size);
            return true;
        }

        const bool convertToBinary(json& jsonF, std::ostream& stream) const {
            return jsonToBinary(jsonF, stream);
        }

        static const bool jsonToBinary(json& jsonF, std::ostream& stream) {
            T temp{};
            temp.deserializeJson(jsonF);
            return temp.serializeBinary(stream);
        }

    private:
        static constexpr size_t REQUIRED_UINT64 = ((size & 0x7) ? (size + (size & 0x7)) : size) >> 3;
    };
}