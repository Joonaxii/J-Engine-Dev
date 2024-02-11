#include <JEngine/IO/Serialization/SerializedItem.h>
#include <JEngine/Rendering/ImGui/ImGuiUtils.h>
#include <JEngine/Core/Log.h>

#include <JEngine/Components/ComponentFactory.h>

namespace JEngine {
    static void serializeBuiltIn(yamlEmit& node, const void* data, size_t size, VType type) {
        switch (type)
        {
        default:
            node << YAML::Null;
            break;
        case VType::VTYPE_INT8:
            node << *reinterpret_cast<const int8_t*>(data);
            break;
        case VType::VTYPE_UINT8:
            node << *reinterpret_cast<const uint8_t*>(data);
            break;
        case VType::VTYPE_INT16:
            node << *reinterpret_cast<const int16_t*>(data);
            break;
        case VType::VTYPE_UINT16:
            node << *reinterpret_cast<const uint16_t*>(data);
            break;
        case VType::VTYPE_INT32:
            node << *reinterpret_cast<const int32_t*>(data);
            break;
        case VType::VTYPE_UINT32:
            node << *reinterpret_cast<const uint32_t*>(data);
            break;
        case VType::VTYPE_INT64:
            node << *reinterpret_cast<const int64_t*>(data);
            break;
        case VType::VTYPE_UINT64:
            node << *reinterpret_cast<const uint64_t*>(data);
            break;
        case VType::VTYPE_FLOAT:
            node << *reinterpret_cast<const float*>(data);
            break;
        case VType::VTYPE_DOUBLE:
            node << *reinterpret_cast<const double*>(data);
            break;
        case VType::VTYPE_BOOL:
            node << *reinterpret_cast<const bool*>(data);
            break;
        case VType::VTYPE_VEC_2I:
            node << YAML::BeginSeq << YAML::Flow << *reinterpret_cast<const int32_t*>(data) << *(reinterpret_cast<const int32_t*>(data) + 1) << YAML::EndSeq;
            break;
        case VType::VTYPE_VEC_2F:
            node << YAML::BeginSeq << YAML::Flow << *reinterpret_cast<const float*>(data) << *(reinterpret_cast<const float*>(data) + 1) << YAML::EndSeq;
            break;
        case VType::VTYPE_VEC_3I:
            node <<
                YAML::BeginSeq << YAML::Flow <<
                (reinterpret_cast<const int32_t*>(data)[0]) <<
                (reinterpret_cast<const int32_t*>(data)[1]) <<
                (reinterpret_cast<const int32_t*>(data)[2]) <<
                YAML::EndSeq;
            break;
        case VType::VTYPE_VEC_3F:
            node <<
                YAML::BeginSeq << YAML::Flow <<
                (reinterpret_cast<const int32_t*>(data)[0]) <<
                (reinterpret_cast<const int32_t*>(data)[1]) <<
                (reinterpret_cast<const int32_t*>(data)[2]) <<
                YAML::EndSeq;
            break;
        case VType::VTYPE_VEC_4I:
            node <<
                YAML::BeginSeq << YAML::Flow <<
                (reinterpret_cast<const int32_t*>(data)[0]) <<
                (reinterpret_cast<const int32_t*>(data)[1]) <<
                (reinterpret_cast<const int32_t*>(data)[2]) <<
                (reinterpret_cast<const int32_t*>(data)[3]) <<
                YAML::EndSeq;
            break;
        case VType::VTYPE_VEC_4F:
            node <<
                YAML::BeginSeq << YAML::Flow <<
                (reinterpret_cast<const float*>(data)[0]) <<
                (reinterpret_cast<const float*>(data)[1]) <<
                (reinterpret_cast<const float*>(data)[2]) <<
                (reinterpret_cast<const float*>(data)[3]) <<
                YAML::EndSeq;
            break;
        case VType::VTYPE_MATRIX:
            node <<
                YAML::BeginSeq << YAML::Block <<
                (reinterpret_cast<const float*>(data)[0x0]) <<
                (reinterpret_cast<const float*>(data)[0x1]) <<
                (reinterpret_cast<const float*>(data)[0x2]) <<
                (reinterpret_cast<const float*>(data)[0x3]) <<

                (reinterpret_cast<const float*>(data)[0x4]) <<
                (reinterpret_cast<const float*>(data)[0x5]) <<
                (reinterpret_cast<const float*>(data)[0x6]) <<
                (reinterpret_cast<const float*>(data)[0x7]) <<

                (reinterpret_cast<const float*>(data)[0x8]) <<
                (reinterpret_cast<const float*>(data)[0x9]) <<
                (reinterpret_cast<const float*>(data)[0xA]) <<
                (reinterpret_cast<const float*>(data)[0xB]) <<

                (reinterpret_cast<const float*>(data)[0xC]) <<
                (reinterpret_cast<const float*>(data)[0xD]) <<
                (reinterpret_cast<const float*>(data)[0xE]) <<
                (reinterpret_cast<const float*>(data)[0xF]) <<
                YAML::EndSeq;
            break;
        case VType::VTYPE_RECTI:
            node << YAML::BeginMap <<
                YAML::Key << "min" << YAML::Value << YAML::BeginSeq << YAML::Flow <<
                (reinterpret_cast<const int32_t*>(data)[0]) <<
                (reinterpret_cast<const int32_t*>(data)[1]) <<
                (reinterpret_cast<const int32_t*>(data)[2]) <<
                (reinterpret_cast<const int32_t*>(data)[3]) <<
                YAML::EndSeq;

            node << YAML::Key << "max" << YAML::Value <<
                YAML::BeginSeq << YAML::Flow <<
                (reinterpret_cast<const int32_t*>(data)[4]) <<
                (reinterpret_cast<const int32_t*>(data)[5]) <<
                (reinterpret_cast<const int32_t*>(data)[6]) <<
                (reinterpret_cast<const int32_t*>(data)[7]) <<
                YAML::EndSeq;
            break;
        case VType::VTYPE_RECTF:
            node << YAML::BeginMap <<
                YAML::Key << "min" << YAML::Value << YAML::BeginSeq << YAML::Flow <<
                (reinterpret_cast<const float*>(data)[0]) <<
                (reinterpret_cast<const float*>(data)[1]) <<
                (reinterpret_cast<const float*>(data)[2]) <<
                (reinterpret_cast<const float*>(data)[3]) <<
                YAML::EndSeq;

            node << YAML::Key << "max" << YAML::Value <<
                YAML::BeginSeq << YAML::Flow <<
                (reinterpret_cast<const float*>(data)[4]) <<
                (reinterpret_cast<const float*>(data)[5]) <<
                (reinterpret_cast<const float*>(data)[6]) <<
                (reinterpret_cast<const float*>(data)[7]) <<
                YAML::EndSeq << YAML::EndMap;
            break;
        case VType::VTYPE_COLOR32:
            node << YAML::BeginMap <<
                YAML::Key << "r" << YAML::Value << reinterpret_cast<const uint8_t*>(data)[0] <<
                YAML::Key << "g" << YAML::Value << reinterpret_cast<const uint8_t*>(data)[1] <<
                YAML::Key << "b" << YAML::Value << reinterpret_cast<const uint8_t*>(data)[2] <<
                YAML::Key << "a" << YAML::Value << reinterpret_cast<const uint8_t*>(data)[3] <<
                YAML::EndMap;
            break;
        case VType::VTYPE_COLOR24:
            node << YAML::BeginMap <<
                YAML::Key << "r" << YAML::Value << reinterpret_cast<const uint8_t*>(data)[0] <<
                YAML::Key << "g" << YAML::Value << reinterpret_cast<const uint8_t*>(data)[1] <<
                YAML::Key << "b" << YAML::Value << reinterpret_cast<const uint8_t*>(data)[2] <<
                YAML::EndMap;
            break;
        case VType::VTYPE_COLOR:
            node << YAML::BeginMap <<
                YAML::Key << "r" << YAML::Value << reinterpret_cast<const float*>(data)[0] <<
                YAML::Key << "g" << YAML::Value << reinterpret_cast<const float*>(data)[1] <<
                YAML::Key << "b" << YAML::Value << reinterpret_cast<const float*>(data)[2] <<
                YAML::Key << "a" << YAML::Value << reinterpret_cast<const float*>(data)[3] <<
                YAML::EndMap;
            break;

        case VType::VTYPE_STRING: {
            static std::string str{};
            auto& temp = *reinterpret_cast<const String*>(data);

            str.clear();
            str.append(temp.data(), temp.length());
            node << str;
            break;
        }

        case VType::VTYPE_LAYER_MASK:
            node << YAML::Hex << *reinterpret_cast<const uint32_t*>(data);
            break;
        case VType::VTYPE_SORTING_LAYER: {
            const SortingLayer& sort = *reinterpret_cast<const SortingLayer*>(data);
            node << YAML::BeginMap <<
                YAML::Key << "layer" << YAML::Value << sort.getLayerIndex() <<
                YAML::Key << "order" << YAML::Value << sort.getOrder() << YAML::EndMap;
            break;
        }

        case VType::VTYPE_GAME_OBJECT_REF:
        case VType::VTYPE_COMPONENT_REF:
        case VType::VTYPE_ASSET_REF:
            node << YAML::Hex << *reinterpret_cast<const uint64_t*>(data);
            break;
        }
    }
    static void deserializeBuiltIn(const yamlNode& node, size_t size, VType type, std::function<void* (size_t&)> getData) {
        void* data = getData(size);
        switch (type)
        {
        default: break;
        case VType::VTYPE_INT8:
            *reinterpret_cast<int8_t*>(data) = node.as<int8_t>();
            break;
        case VType::VTYPE_UINT8:
            *reinterpret_cast<uint8_t*>(data) = node.as<int8_t>();
            break;
        case VType::VTYPE_INT16:
            *reinterpret_cast<int16_t*>(data) = node.as<int16_t>();
            break;
        case VType::VTYPE_UINT16:
            *reinterpret_cast<uint16_t*>(data) = node.as<uint16_t>();
            break;
        case VType::VTYPE_INT32:
            *reinterpret_cast<int32_t*>(data) = node.as<int32_t>();
            break;
        case VType::VTYPE_UINT32:
            *reinterpret_cast<uint32_t*>(data) = node.as<uint32_t>();
            break;
        case VType::VTYPE_INT64:
            *reinterpret_cast<int64_t*>(data) = node.as<int64_t>();
            break;
        case VType::VTYPE_UINT64:
            *reinterpret_cast<uint64_t*>(data) = node.as<uint64_t>();
            break;
        case VType::VTYPE_FLOAT:
            *reinterpret_cast<float*>(data) = node.as<float>();
            break;
        case VType::VTYPE_DOUBLE:
            *reinterpret_cast<double*>(data) = node.as<double>();
            break;
        case VType::VTYPE_BOOL:
            *reinterpret_cast<bool*>(data) = node.as<bool>();
            break;
        case VType::VTYPE_VEC_2I: {
            int32_t* iPtr = reinterpret_cast<int32_t*>(data);
            iPtr[0] = node[0].as<int32_t>();
            iPtr[1] = node[1].as<int32_t>();
            break;
        }
        case VType::VTYPE_VEC_2F: {
            float* fPtr = reinterpret_cast<float*>(data);
            fPtr[0] = node[0].as<float>();
            fPtr[1] = node[1].as<float>();
            break;
        }
        case VType::VTYPE_VEC_3I: {
            int32_t* iPtr = reinterpret_cast<int32_t*>(data);
            iPtr[0] = node[0].as<int32_t>();
            iPtr[1] = node[1].as<int32_t>();
            iPtr[2] = node[2].as<int32_t>();
            break;
        }
        case VType::VTYPE_VEC_3F: {
            float* fPtr = reinterpret_cast<float*>(data);
            fPtr[0] = node[0].as<float>();
            fPtr[1] = node[1].as<float>();
            fPtr[2] = node[2].as<float>();
            break;
        }
        case VType::VTYPE_VEC_4I: {
            int32_t* iPtr = reinterpret_cast<int32_t*>(data);
            iPtr[0] = node[0].as<int32_t>();
            iPtr[1] = node[1].as<int32_t>();
            iPtr[2] = node[2].as<int32_t>();
            iPtr[3] = node[3].as<int32_t>();
            break;
        }
        case VType::VTYPE_VEC_4F: {
            float* fPtr = reinterpret_cast<float*>(data);
            fPtr[0] = node[0].as<float>();
            fPtr[1] = node[1].as<float>();
            fPtr[2] = node[2].as<float>();
            fPtr[3] = node[3].as<float>();
            break;
        }
        case VType::VTYPE_MATRIX: {
            float* fPtr = reinterpret_cast<float*>(data);
            fPtr[0x0] = node[0x0].as<float>();
            fPtr[0x1] = node[0x1].as<float>();
            fPtr[0x2] = node[0x2].as<float>();
            fPtr[0x3] = node[0x3].as<float>();

            fPtr[0x4] = node[0x4].as<float>();
            fPtr[0x5] = node[0x5].as<float>();
            fPtr[0x6] = node[0x6].as<float>();
            fPtr[0x7] = node[0x7].as<float>();

            fPtr[0x8] = node[0x8].as<float>();
            fPtr[0x9] = node[0x9].as<float>();
            fPtr[0xA] = node[0xA].as<float>();
            fPtr[0xB] = node[0xB].as<float>();

            fPtr[0xC] = node[0xC].as<float>();
            fPtr[0xD] = node[0xD].as<float>();
            fPtr[0xE] = node[0xE].as<float>();
            fPtr[0xF] = node[0xF].as<float>();
            break;
        }
        case VType::VTYPE_RECTI: {
            int32_t* iPtr = reinterpret_cast<int32_t*>(data);
            iPtr[0] = node["min"][0].as<int32_t>();
            iPtr[1] = node["min"][1].as<int32_t>();

            iPtr[2] = node["max"][0].as<int32_t>();
            iPtr[3] = node["max"][1].as<int32_t>();
            break;
        }
        case VType::VTYPE_RECTF: {
            float* fPtr = reinterpret_cast<float*>(data);
            fPtr[0] = node["min"][0].as<float>();
            fPtr[1] = node["min"][1].as<float>();

            fPtr[2] = node["max"][0].as<float>();
            fPtr[3] = node["max"][1].as<float>();
            break;
        }
        case VType::VTYPE_COLOR32: {
            uint8_t* iPtr = reinterpret_cast<uint8_t*>(data);
            iPtr[0] = node["r"][0].as<uint8_t>();
            iPtr[1] = node["g"][1].as<uint8_t>();
            iPtr[2] = node["b"][0].as<uint8_t>();
            iPtr[3] = node["a"][1].as<uint8_t>();
            break;
        }
        case VType::VTYPE_COLOR24: {
            uint8_t* iPtr = reinterpret_cast<uint8_t*>(data);
            iPtr[0] = node["r"][0].as<uint8_t>();
            iPtr[1] = node["g"][1].as<uint8_t>();
            iPtr[2] = node["b"][0].as<uint8_t>();
            break;
        }
        case VType::VTYPE_COLOR: {
            float* fPtr = reinterpret_cast<float*>(data);
            fPtr[0] = node["r"][0].as<float>();
            fPtr[1] = node["g"][1].as<float>();
            fPtr[2] = node["b"][0].as<float>();
            fPtr[3] = node["a"][1].as<float>();
            break;
        }

        case VType::VTYPE_STRING: {
            std::string str = node.as<std::string>();
            size = str.length();
            String* data = reinterpret_cast<String*>(data);
            data->resize(size);

            break;
        }

        case VType::VTYPE_LAYER_MASK:
            *reinterpret_cast<uint32_t*>(data) = node.as<uint32_t>();
            break;
        case VType::VTYPE_SORTING_LAYER: {
            *reinterpret_cast<SortingLayer*>(data) = SortingLayer(node["order"].as<int32_t>(), node["layer"].as<int32_t>());
            break;
        }

        case VType::VTYPE_GAME_OBJECT_REF:
        case VType::VTYPE_COMPONENT_REF:
        case VType::VTYPE_ASSET_REF:
            *reinterpret_cast<uint64_t*>(data) = node.as<uint64_t>();
            break;
        }
    }

    static void serializeBuiltIn(json& jsonF, const void* data, size_t size, VType type) {
        switch (type)
        {
        default:
            jsonF = nullptr_t{};
            break;
        case VType::VTYPE_INT8:
            jsonF = *reinterpret_cast<const int8_t*>(data);
            break;
        case VType::VTYPE_UINT8:
            jsonF = *reinterpret_cast<const uint8_t*>(data);
            break;
        case VType::VTYPE_INT16:
            jsonF = *reinterpret_cast<const int16_t*>(data);
            break;
        case VType::VTYPE_UINT16:
            jsonF = *reinterpret_cast<const uint16_t*>(data);
            break;
        case VType::VTYPE_INT32:
            jsonF = *reinterpret_cast<const int32_t*>(data);
            break;
        case VType::VTYPE_UINT32:
            jsonF = *reinterpret_cast<const uint32_t*>(data);
            break;
        case VType::VTYPE_INT64:
            jsonF = *reinterpret_cast<const int64_t*>(data);
            break;
        case VType::VTYPE_UINT64:
            jsonF = *reinterpret_cast<const uint64_t*>(data);
            break;
        case VType::VTYPE_FLOAT:
            jsonF = *reinterpret_cast<const float*>(data);
            break;
        case VType::VTYPE_DOUBLE:
            jsonF = *reinterpret_cast<const double*>(data);
            break;
        case VType::VTYPE_BOOL:
            jsonF = *reinterpret_cast<const bool*>(data);
            break;
        case VType::VTYPE_VEC_2I: {
            jsonF = json::array_t{};
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[0]));
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[1]));
            break;
        }
        case VType::VTYPE_VEC_2F: {
            jsonF = json::array_t{};
            jsonF.push_back((reinterpret_cast<const float*>(data)[0]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[1]));
            break;
        }
        case VType::VTYPE_VEC_3I: {
            jsonF = json::array_t{};
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[0]));
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[1]));
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[2]));
            break;
        }
        case VType::VTYPE_VEC_3F: {
            jsonF = json::array_t{};
            jsonF.push_back((reinterpret_cast<const float*>(data)[0]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[1]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[2]));
            break;
        }
        case VType::VTYPE_VEC_4I: {
            jsonF = json::array_t{};
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[0]));
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[1]));
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[2]));
            jsonF.push_back((reinterpret_cast<const int32_t*>(data)[3]));
            break;
        }
        case VType::VTYPE_VEC_4F: {
            jsonF = json::array_t{};
            jsonF.push_back((reinterpret_cast<const float*>(data)[0]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[1]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[2]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[3]));
            break;
        }
        case VType::VTYPE_MATRIX: {
            jsonF = json::array_t{};
            jsonF.push_back((reinterpret_cast<const float*>(data)[0x0]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0x1]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0x2]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0x3]));

            jsonF.push_back((reinterpret_cast<const float*>(data)[0x4]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0x5]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0x6]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0x7]));

            jsonF.push_back((reinterpret_cast<const float*>(data)[0x8]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0x9]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0xA]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0xB]));

            jsonF.push_back((reinterpret_cast<const float*>(data)[0xC]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0xD]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0xE]));
            jsonF.push_back((reinterpret_cast<const float*>(data)[0xF]));
            break;
        }
        case VType::VTYPE_RECTI: {
            auto& min = jsonF["min"] = json::array_t{};
            auto& max = jsonF["max"] = json::array_t{};
            min.push_back((reinterpret_cast<const int32_t*>(data)[0]));
            min.push_back((reinterpret_cast<const int32_t*>(data)[1]));

            max.push_back((reinterpret_cast<const int32_t*>(data)[2]));
            max.push_back((reinterpret_cast<const int32_t*>(data)[3]));
            break;
        }
        case VType::VTYPE_RECTF: {
            auto& min = jsonF["min"] = json::array_t{};
            auto& max = jsonF["max"] = json::array_t{};
            min.push_back((reinterpret_cast<const float*>(data)[0]));
            min.push_back((reinterpret_cast<const float*>(data)[1]));

            max.push_back((reinterpret_cast<const float*>(data)[2]));
            max.push_back((reinterpret_cast<const float*>(data)[3]));
            break;
        }
        case VType::VTYPE_COLOR32:
            jsonF["r"] = reinterpret_cast<const uint8_t*>(data)[0];
            jsonF["g"] = reinterpret_cast<const uint8_t*>(data)[1];
            jsonF["b"] = reinterpret_cast<const uint8_t*>(data)[2];
            jsonF["a"] = reinterpret_cast<const uint8_t*>(data)[3];
            break;
        case VType::VTYPE_COLOR24:
            jsonF["r"] = reinterpret_cast<const uint8_t*>(data)[0];
            jsonF["g"] = reinterpret_cast<const uint8_t*>(data)[1];
            jsonF["b"] = reinterpret_cast<const uint8_t*>(data)[2];
            break;
        case VType::VTYPE_COLOR:
            jsonF["r"] = reinterpret_cast<const float*>(data)[0];
            jsonF["g"] = reinterpret_cast<const float*>(data)[1];
            jsonF["b"] = reinterpret_cast<const float*>(data)[2];
            jsonF["a"] = reinterpret_cast<const float*>(data)[3];
            break;

        case VType::VTYPE_STRING: {
            jsonF = std::string_view(reinterpret_cast<const char*>(data));
            break;
        }

        case VType::VTYPE_LAYER_MASK:
            jsonF = *reinterpret_cast<const uint32_t*>(data);
            break;
        case VType::VTYPE_SORTING_LAYER: {
            const SortingLayer& sort = *reinterpret_cast<const SortingLayer*>(data);
            jsonF["layer"] = sort.getLayerIndex();
            jsonF["order"] = sort.getOrder();
            break;
        }

        case VType::VTYPE_GAME_OBJECT_REF:
        case VType::VTYPE_COMPONENT_REF:
        case VType::VTYPE_ASSET_REF:
            jsonF = *reinterpret_cast<const uint64_t*>(data);
            break;
        }
    }
    static void deserializeBuiltIn(const json& jsonF, size_t size, VType type, std::function<void* (size_t&)> getData) {
        void* data = getData(size);
        switch (type)
        {
        default: break;
        case VType::VTYPE_INT8:
            *reinterpret_cast<int8_t*>(data) = jsonF.get<int8_t>();
            break;
        case VType::VTYPE_UINT8:
            *reinterpret_cast<uint8_t*>(data) = jsonF.get<uint8_t>();
            break;
        case VType::VTYPE_INT16:
            *reinterpret_cast<int16_t*>(data) = jsonF.get<int16_t>();
            break;
        case VType::VTYPE_UINT16:
            *reinterpret_cast<uint16_t*>(data) = jsonF.get<uint16_t>();
            break;
        case VType::VTYPE_INT32:
            *reinterpret_cast<int32_t*>(data) = jsonF.get<int32_t>();
            break;
        case VType::VTYPE_UINT32:
            *reinterpret_cast<uint32_t*>(data) = jsonF.get<uint32_t>();
            break;
        case VType::VTYPE_INT64:
            *reinterpret_cast<int64_t*>(data) = jsonF.get<int64_t>();
            break;
        case VType::VTYPE_UINT64:
            *reinterpret_cast<uint64_t*>(data) = jsonF.get<uint64_t>();
            break;
        case VType::VTYPE_FLOAT:
            *reinterpret_cast<float*>(data) = jsonF.get<float>();
            break;
        case VType::VTYPE_DOUBLE:
            *reinterpret_cast<double*>(data) = jsonF.get<double>();
            break;
        case VType::VTYPE_BOOL:
            *reinterpret_cast<bool*>(data) = jsonF.get<bool>();
            break;
        case VType::VTYPE_VEC_2I: {
            reinterpret_cast<int32_t*>(data)[0] = jsonF[0].get<int32_t>();
            reinterpret_cast<int32_t*>(data)[1] = jsonF[1].get<int32_t>();
            break;
        }
        case VType::VTYPE_VEC_2F: {
            reinterpret_cast<float*>(data)[0] = jsonF[0].get<float>();
            reinterpret_cast<float*>(data)[1] = jsonF[1].get<float>();
            break;
        }
        case VType::VTYPE_VEC_3I: {
            reinterpret_cast<int32_t*>(data)[0] = jsonF[0].get<int32_t>();
            reinterpret_cast<int32_t*>(data)[1] = jsonF[1].get<int32_t>();
            reinterpret_cast<int32_t*>(data)[2] = jsonF[2].get<int32_t>();
            break;
        }
        case VType::VTYPE_VEC_3F: {
            reinterpret_cast<float*>(data)[0] = jsonF[0].get<float>();
            reinterpret_cast<float*>(data)[1] = jsonF[1].get<float>();
            reinterpret_cast<float*>(data)[2] = jsonF[2].get<float>();
            break;
        }
        case VType::VTYPE_VEC_4I: {
            reinterpret_cast<int32_t*>(data)[0] = jsonF[0].get<int32_t>();
            reinterpret_cast<int32_t*>(data)[1] = jsonF[1].get<int32_t>();
            reinterpret_cast<int32_t*>(data)[2] = jsonF[2].get<int32_t>();
            reinterpret_cast<int32_t*>(data)[3] = jsonF[3].get<int32_t>();
            break;
        }
        case VType::VTYPE_VEC_4F: {
            reinterpret_cast<float*>(data)[0] = jsonF[0].get<float>();
            reinterpret_cast<float*>(data)[1] = jsonF[1].get<float>();
            reinterpret_cast<float*>(data)[2] = jsonF[2].get<float>();
            reinterpret_cast<float*>(data)[3] = jsonF[3].get<float>();
            break;
        }
        case VType::VTYPE_MATRIX: {
            reinterpret_cast<float*>(data)[0x0] = jsonF[0x0].get<float>();
            reinterpret_cast<float*>(data)[0x1] = jsonF[0x1].get<float>();
            reinterpret_cast<float*>(data)[0x2] = jsonF[0x2].get<float>();
            reinterpret_cast<float*>(data)[0x3] = jsonF[0x3].get<float>();

            reinterpret_cast<float*>(data)[0x4] = jsonF[0x4].get<float>();
            reinterpret_cast<float*>(data)[0x5] = jsonF[0x5].get<float>();
            reinterpret_cast<float*>(data)[0x6] = jsonF[0x6].get<float>();
            reinterpret_cast<float*>(data)[0x7] = jsonF[0x7].get<float>();

            reinterpret_cast<float*>(data)[0x8] = jsonF[0x8].get<float>();
            reinterpret_cast<float*>(data)[0x9] = jsonF[0x9].get<float>();
            reinterpret_cast<float*>(data)[0xA] = jsonF[0xA].get<float>();
            reinterpret_cast<float*>(data)[0xB] = jsonF[0xB].get<float>();

            reinterpret_cast<float*>(data)[0xC] = jsonF[0xC].get<float>();
            reinterpret_cast<float*>(data)[0xD] = jsonF[0xD].get<float>();
            reinterpret_cast<float*>(data)[0xE] = jsonF[0xE].get<float>();
            reinterpret_cast<float*>(data)[0xF] = jsonF[0xF].get<float>();
            break;
        }
        case VType::VTYPE_RECTI: {
            auto& min = jsonF["min"];
            auto& max = jsonF["max"];

            reinterpret_cast<int32_t*>(data)[0] = min[0].get<int32_t>();
            reinterpret_cast<int32_t*>(data)[1] = min[1].get<int32_t>();

            reinterpret_cast<int32_t*>(data)[2] = max[0].get<int32_t>();
            reinterpret_cast<int32_t*>(data)[3] = max[1].get<int32_t>();
            break;
        }
        case VType::VTYPE_RECTF: {
            auto& min = jsonF["min"];
            auto& max = jsonF["max"];

            reinterpret_cast<float*>(data)[0] = min[0].get<float>();
            reinterpret_cast<float*>(data)[1] = min[1].get<float>();

            reinterpret_cast<float*>(data)[2] = max[0].get<float>();
            reinterpret_cast<float*>(data)[3] = max[1].get<float>();
            break;
        }
        case VType::VTYPE_COLOR32:
            reinterpret_cast<uint8_t*>(data)[0] = jsonF["r"].get<uint8_t>();
            reinterpret_cast<uint8_t*>(data)[1] = jsonF["g"].get<uint8_t>();
            reinterpret_cast<uint8_t*>(data)[2] = jsonF["b"].get<uint8_t>();
            reinterpret_cast<uint8_t*>(data)[3] = jsonF["a"].get<uint8_t>();
            break;
        case VType::VTYPE_COLOR24:
            reinterpret_cast<uint8_t*>(data)[0] = jsonF["r"].get<uint8_t>();
            reinterpret_cast<uint8_t*>(data)[1] = jsonF["g"].get<uint8_t>();
            reinterpret_cast<uint8_t*>(data)[2] = jsonF["b"].get<uint8_t>();
            break;
        case VType::VTYPE_COLOR:
            reinterpret_cast<float*>(data)[0] = jsonF["r"].get<float>();
            reinterpret_cast<float*>(data)[1] = jsonF["g"].get<float>();
            reinterpret_cast<float*>(data)[2] = jsonF["b"].get<float>();
            reinterpret_cast<float*>(data)[3] = jsonF["a"].get<float>();
            break;

        case VType::VTYPE_STRING: {
            std::string str = jsonF.get<std::string>();
            size = str.length() + 1;
            uint8_t* data = reinterpret_cast<uint8_t*>(getData(size));
            memcpy(data, str.c_str(), std::max(size, 1ULL) - 1);
            data[str.length()] = 0;
            break;
        }

        case VType::VTYPE_LAYER_MASK:
            *reinterpret_cast<uint32_t*>(data) = jsonF.get<uint32_t>();
            break;
        case VType::VTYPE_SORTING_LAYER: {
            *reinterpret_cast<SortingLayer*>(data) = SortingLayer(jsonF.value("order", 0), jsonF.value("layer", 0));
            break;
        }

        case VType::VTYPE_GAME_OBJECT_REF:
        case VType::VTYPE_COMPONENT_REF:
        case VType::VTYPE_ASSET_REF:
            *reinterpret_cast<uint32_t*>(data) = jsonF.get<uint32_t>();
            break;

        case VType::VTYPE_CUSTOM: {
            size = jsonF.size();
            uint8_t* dataP = reinterpret_cast<uint8_t*>(getData(size));
            for (size_t i = 0; i < size; i++) {
                dataP[i] = jsonF[i].get<uint8_t>();
            }
            break;
        }
        }
    }

    SerializedItem::~SerializedItem() {
        clear();
    }

    const SerializedItem* SerializedItem::operator[](std::string_view path) const {
        return findByPath(path);
    }

    SerializedItem* SerializedItem::operator[](std::string_view path) {
        return findByPath(path);
    }

    SerializedItem* SerializedItem::findByKey(std::string_view key) const {
        for (auto ch : _children) {
            if (ch && Helpers::strEquals(ch->_name, key)) {
                return ch;
            }
        }
        return nullptr;
    }

    SerializedItem* SerializedItem::findByPath(std::string_view path) {
        size_t pathInd = 0;

        std::string_view cur{ };
        SerializedItem* curNode{ this };
        while (curNode) {
            size_t next = path.find('/', pathInd);
            if (next != std::string_view::npos) {
                cur = path.substr(pathInd, next - pathInd);
                pathInd = next + 1;
            }
            else {
                cur = path.substr(pathInd);
            }
            curNode = curNode->findByKey(cur);
        }
        return curNode;
    }

    void SerializedItem::copyTo(SerializedItem& other) {
        other._name = _name;
        other._itemType = _itemType;
        other._type = _type;
        other._fieldInfo = _fieldInfo;

        switch (_itemType) {
        case IType::ITYPE_VALUE:
        case IType::ITYPE_ARRAY:
            _data.copyTo(other._data);
            return;
        }

        other._children.resize(_children.size());
        for (size_t i = 0; i < _children.size(); i++) {
            SerializedItem* ch = getSItemPool().allocate();
            other._children[i] = ch;
            if (ch && _children[i]) {
                _children[i]->copyTo(*ch);
            }
        }
    }

    const SerializedItem* SerializedItem::findByPath(std::string_view path) const {
        size_t pathInd = 0;

        std::string_view cur{ };
        const SerializedItem* curNode{ this };
        while (curNode) {
            size_t next = path.find('/', pathInd);
            if (next != std::string_view::npos) {
                cur = path.substr(pathInd, next - pathInd);
                pathInd = next + 1;
            }
            else {
                cur = path.substr(pathInd);
            }
            curNode = curNode->findByKey(cur);
        }
        return curNode;
    }

    void SerializedItem::serialize(const Stream& stream) const {
        if (_descriptor.nodeType == NodeType::ITYPE_ARRAY) {
            stream.writeValue<uint32_t>(uint32_t(_children.size()));
            for (auto ch : _children) {
                ch->serialize(stream);
            }
            return;
        }

        switch (_descriptor.valueType) {
        case ValueType::VTYPE_PROPERTY:
            for (auto ch : _children) {
                ch->serialize(stream);
            }
            break;
        default: {
            stream.write(this->getDataAs<uint8_t>(), uint32_t(_data.size));
            break;
        }
        }
    }

    void SerializedItem::deserialize(const Stream& stream) {
        if (_itemType == IType::ITYPE_ARRAY) {
            clear();
            uint32_t count = stream.readValue<uint32_t>(false);
            _children.resize(count);
            for (size_t i = 0; i < count; i++) {
                auto& ch = _children[i] = getSItemPool().allocate();
                JE_CORE_ASSERT(ch != nullptr, "Failed to allocate a SerializedItem!");
                ch->deserialize(stream);
            }
            return;
        }

        switch (_descriptor.valueType) {
        case ValueType::VTYPE_PROPERTY:
            for (auto ch : _children) {
                ch->deserialize(stream);
            }
            break;
        default: {
            stream.write(this->getDataAs<uint8_t>(), uint32_t(_data.size));
            break;
        }
        }
    }

    void SerializedItem::serialize(json& jsonF) const {
        switch (_descriptor.nodeType)
        {
        case NodeType::ITYPE_ARRAY:
        case NodeType::ITYPE_FIXED_BUFFER: {
            bool isProps = _descriptor.valueType == ValueType::VTYPE_PROPERTY;

            size_t sizeVal = valueTypeToSize(_descriptor.valueType);
            int32_t count = int32_t(isProps ? _children.size() : _data.size / sizeVal);

            if (isProps) {
                json& jsonP = jsonF = json::array_t{};
                for (size_t i = 0; i < _children.size(); i++) {
                    if (auto ch = _children[i]) {
                        ch->serialize(jsonP.emplace_back());
                    }
                }
            }
            else {
                // We use VTYPE_CUSTOM here since it will save the data as raw binary,
                // We could also iteratie over the data and serialize each individually as their own type
                serializeBuiltIn(jsonF, getData(), _data.size, ValueType::VTYPE_CUSTOM);
            }
            break;
        }
        case NodeType::ITYPE_VALUE:
            if (_descriptor.valueType == ValueType::VTYPE_PROPERTY) {
                json& jsonP = jsonF = json{};
                for (size_t i = 0; i < _children.size(); i++) {
                    if (auto ch = _children[i]) {
                        ch->serialize(jsonP[ch->_name]);
                    }
                }
                break;
            }
            serializeBuiltIn(jsonF, getData(), valueTypeToSize(_descriptor.valueType), _descriptor.valueType);
            break;
        default:
            jsonF = nullptr_t{};
            break;
        }
    }
    void SerializedItem::deserialize(const json& jsonF) {
    }

    void SerializedItem::serialize(yamlEmit& emit) const {
        switch (_itemType)
        {
        case IType::ITYPE_OBJECT:

            break;

        case IType::ITYPE_ARRAY:
        case IType::ITYPE_FIXED_BUFFER: {
            size_t sizeVal = valueTypeToSize(_valueType);
            int32_t count = int32_t(isProps ? _children.size() : _data.size / sizeVal);

            if (isProps) {
                emit << YAML::BeginSeq;
                for (size_t i = 0; i < _children.size(); i++) {
                    if (auto ch = _children[i]) {
                        ch->serialize(emit);
                    }
                }
                emit << YAML::EndSeq;
            }
            else {
                // We use VTYPE_CUSTOM here since it will save the data as raw binary,
                // We could also iteratie over the data and serialize each individually as their own type
                serializeBuiltIn(emit, getData(), _data.size, ValueType::VTYPE_CUSTOM);
            }
            break;
        }
        case IType::ITYPE_VALUE:
            if (_descriptor.valueType == ValueType::VTYPE_PROPERTY) {
                emit << YAML::BeginMap;
                for (size_t i = 0; i < _children.size(); i++) {
                    if (auto ch = _children[i]) {
                        emit << YAML::Key << ch->_name.data() << YAML::Value;
                        ch->serialize(emit);
                    }
                }
                emit << YAML::EndMap;
                break;
            }
            serializeBuiltIn(emit, getData(), valueTypeToSize(_descriptor.valueType), _descriptor.valueType);
            break;
        default:
            emit << YAML::Null;
            break;
        }
    }
    void SerializedItem::deserialize(const yamlNode& node) {
        size_t tmpSize{};
        switch (_descriptor.nodeType) {
        case NodeType::ITYPE_ARRAY:
        case NodeType::ITYPE_FIXED_BUFFER: {
            bool isProps = _descriptor.valueType == ValueType::VTYPE_PROPERTY;

            size_t sizeVal = valueTypeToSize(_descriptor.valueType);
            int32_t count = int32_t(isProps ? _children.size() : _data.size / sizeVal);
            bool isFixed = _descriptor.nodeType == NodeType::ITYPE_ARRAY;

            int32_t readCount = isFixed ? int32_t(node.size()) : std::min(count, int32_t(node.size()));
            JE_CORE_ASSERT(node.IsSequence(), "YAML Node isn't a sequence!");

            if (isProps) {
                if (!isFixed) {
                    _children.resize(readCount);
                }

                for (int32_t i = 0; i < readCount; i++) {
                    auto& ch = _children[i];
                    if (!ch) {
                        ch = getSItemPool().allocate();
                    }
                    ch->deserialize(node[i]);
                }
            }
            else {
                if (!isFixed) {
                    _data.allocate(readCount * sizeVal);
                }

                deserializeBuiltIn(node, _data.size, ValueType::VTYPE_CUSTOM, [this](size_t& size) {

                    });
            }
            break;
        }
        case NodeType::ITYPE_VALUE:
            if (_descriptor.valueType == ValueType::VTYPE_PROPERTY) {
                node << YAML::BeginMap;
                for (size_t i = 0; i < _children.size(); i++) {
                    if (auto ch = _children[i]) {
                        node << YAML::Key << ch->_name.c_str() << YAML::Value;
                        ch->serialize(node);
                    }
                }
                node << YAML::EndMap;
                break;
            }
            deserializeBuiltIn(node, getData(), valueTypeToSize(_descriptor.valueType), _descriptor.valueType);
            break;
        default:
            break;
        }
    }

    SerializedItem* SerializedItem::fromFieldInfo(const Type* type, const FieldInfo* field) {
        JE_CORE_RET_IF_FALSE_MSG(field != nullptr, "Could not build SerializedItem! (Given FieldInfo was null)", nullptr);
       
        SerializedItem* item = getSItemPool().allocate();
        JE_CORE_RET_IF_FALSE_MSG(item != nullptr, "Could not allocate SerializedItem!", nullptr);

        item->_type = type;
        item->_fieldInfo = field;
        item->_data.clear();

        return item;
    }

    SerializedItem* SerializedItem::fromType(const Type* type) {
        JE_CORE_RET_IF_FALSE_MSG(type != nullptr, "Could not build SerializedItem! (Given Type was null)", nullptr);

        SerializedItem* item = getSItemPool().allocate();
        JE_CORE_RET_IF_FALSE_MSG(item != nullptr, "Could not allocate SerializedItem!", nullptr);

        item->_type = type;
        item->_fieldInfo = nullptr;
        item->_name = String();
        item->_children.resize(type->fields.size());
        for (size_t i = 0; i < type->fields.size(); i++)
        {
            SerializedItem* ch = fromFieldInfo(type, &type->fields[i]);
            item->_children[i] = ch;

            if (ch) {
                ch->_root = item;
            }
        }
        return item;
    }

    SerializedItem* SerializedItem::fromComponent(uint32_t typeHash) {
        auto type = TypeHelpers::getTypeByHash(typeHash);
        JE_CORE_RET_IF_FALSE_MSG(type != nullptr, "Could not find a type by given hash!", nullptr);

        auto item = fromType(type);
        JE_CORE_RET_IF_FALSE(item != nullptr, nullptr);
        // TODO: Potentially some Component specific init?
        return item;
    }


    bool SerializedItem::drawSelf() {
        return drawSelf(_name.c_str(), _descriptor.nodeType, _descriptor.valueType, _descriptor.subValueType, getData());
    }

    bool SerializedItem::drawSelf(const char* label, NodeType type, ValueType valueType, SubValueType subVType, void* data) {
        if (ensureStyle()->is(GUIStyle::FLAG_NO_DRAW)) { return false; }

        ImGui::PushID(_name.c_str());
        bool changed = false;

        bool hasMin = _style->is(GUIStyle::FLAG_MIN);
        bool hasMax = _style->is(GUIStyle::FLAG_MAX);
        bool doClamp = hasMin || hasMax;

        switch (type) {
        case NodeType::ITYPE_VALUE: {
            switch (valueType) {
            default:
                changed |= Gui::drawBuiltIn(label, valueType, data, *_style);
                break;
            case ValueType::VTYPE_CUSTOM: {
                if (_descriptor.type) {
                    uint8_t ret = CustomSerializables::customDraw(*this, _descriptor.type->hash);
                    if ((ret & 0x2) == 0) {
                        changed |= (ret & 0x1) != 0;
                        break;
                    }
                }
                __fallthrough;
            }
            case ValueType::VTYPE_PROPERTY:
            {
                if (ImGui::CollapsingHeader(label)) {
                    ImGui::Indent();
                    for (size_t i = 0; i < _children.size(); i++) {
                        ImGui::PushID(int32_t(i));
                        if (auto& ch = _children[i]) {
                            changed |= ch->drawSelf();
                        }
                        ImGui::PopID();
                    }
                    ImGui::Unindent();
                }
                break;
            }
            }
            break;
        }

        case NodeType::ITYPE_ARRAY:
        case NodeType::ITYPE_FIXED_BUFFER:
        {
            bool header = ImGui::CollapsingHeader(_name.c_str());
            bool isProps = _descriptor.valueType == ValueType::VTYPE_PROPERTY;

            size_t sizeVal = valueTypeToSize(_descriptor.valueType);
            int32_t count = int32_t(isProps ? _children.size() : _data.size / sizeVal);

            ImGui::BeginDisabled(_descriptor.nodeType == NodeType::ITYPE_FIXED_BUFFER);
            if (ImGui::DragInt("Size##Array", &count, 1, 0, INT_MAX - 1, "%d", ImGuiSliderFlags_AlwaysClamp)) {
                if (isProps) {
                    _children.resize(count);
                }
                else {
                    _data.allocate(count * valueTypeToSize(_descriptor.valueType));
                }
            }
            ImGui::EndDisabled();

            if (header) {
                ImGui::Indent();
                char temp[96]{ 0 };
                for (int32_t i = 0; i < count; i++) {
                    ImGui::PushID(i);
                    sprintf_s(temp, "Element #%d", i);
                    if (isProps) {
                        if (auto& ch = _children[i]) {
                            changed |= ch->drawSelf(temp, ch->_descriptor.nodeType, ch->_descriptor.valueType, ch->_descriptor.subValueType, ch->getData());
                        }
                    }
                    else {
                        uint8_t* dataPtr = reinterpret_cast<uint8_t*>(data);
                        changed |= this->drawSelf(temp, NodeType::ITYPE_VALUE, _descriptor.valueType, _descriptor.subValueType, dataPtr);
                        dataPtr += sizeVal;
                    }
                    ImGui::PopID();
                }
                ImGui::Unindent();
            }
            break;
        }
        }

        ImGui::PopID();
        return changed;
    }

    static void destructData(void* data, VType type) {
        if (!data) { return; }
        switch (type)
        {
        case VType::VTYPE_STRING:
            memDestruct(reinterpret_cast<String*>(data));
            break;
        }
    }

    void SerializedItem::clearData() {
        if (_fieldInfo) {
            switch (_itemType)
            {
            case IType::ITYPE_OBJECT:
                return;

            case IType::ITYPE_VALUE:
                destructData(_data.get(), _fieldInfo->type);
                break;
            case IType::ITYPE_ARRAY:
            case IType::ITYPE_FIXED_BUFFER: {
                size_t elementSize = _fieldInfo->size;
                uint8_t* items = _data.getAs<uint8_t>();
                uint8_t* end = _data.getEndAs<uint8_t>();
                while (items < end) {
                    destructData(items, _fieldInfo->type);
                    items += elementSize;
                }
                break;
            }
            }
            JE_ZERO_MEM(_data.get(), size_t(_data.getEndAs<uint8_t>() - _data.getAs<uint8_t>()));
        }
    }

    void* SerializedItem::getDataIn(size_t& size) {
        bool isProp = _descriptor.valueType == ValueType::VTYPE_PROPERTY;
        if (_descriptor.nodeType == NodeType::ITYPE_FIXED_BUFFER) {
            size = std::min(isProp ? _children.size() : _data.size, size);
            return getData();
        }

        switch (_descriptor.valueType)
        {
        default:
            break;
        }

        return getData();
    }
}