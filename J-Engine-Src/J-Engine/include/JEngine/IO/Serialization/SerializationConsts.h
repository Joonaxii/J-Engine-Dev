#pragma once
#include <cstdint>
#include <JEngine/Helpers/TypeHelpers.h>

namespace JEngine {
    enum class ValueType : uint8_t {
        VTYPE_NONE = 0x00,

        VTYPE_INT8,
        VTYPE_UINT8,

        VTYPE_INT16,
        VTYPE_UINT16,

        VTYPE_INT32,
        VTYPE_UINT32,

        VTYPE_INT64,
        VTYPE_UINT64,
        
        VTYPE_FLOAT,
        VTYPE_DOUBLE,

        VTYPE_BOOL,

        VTYPE_VEC_2I,
        VTYPE_VEC_2F,

        VTYPE_VEC_3I,
        VTYPE_VEC_3F,

        VTYPE_VEC_4I,
        VTYPE_VEC_4F,

        VTYPE_MATRIX,

        VTYPE_RECTI,
        VTYPE_RECTF,

        VTYPE_COLOR32,
        VTYPE_COLOR24,
        VTYPE_COLOR,

        VTYPE_STRING,
        
        //----------------------------------------
        //Note: For now, ignore these color types
        //Most likely will never be used in the
        //inspector/serialization context anyways
        //----------------------------------------
        //VTYPE_COLOR565,
        //VTYPE_COLOR555,
        //VTYPE_COLOR4444,

        //VTYPE_COLOR48,
        //VTYPE_COLOR64,

        VTYPE_LAYER_MASK,
        VTYPE_SORTING_LAYER,

        VTYPE_GAME_OBJECT_REF,
        VTYPE_COMPONENT_REF,
        VTYPE_ASSET_REF,

        VTYPE_CUSTOM,
        VTYPE_PROPERTY,
    };
    enum class SubValueType : uint8_t {
        SUB_VTYPE_NONE = 0x00,
        SUB_VTYPE_BIT_MASK = 0x1,
        SUB_VTYPE_ENUM = 0x2,
    };
    enum class NodeType : uint8_t {
        NTYPE_VALUE = 0x00,
        NTYPE_ARRAY = 0x01,
        NTYPE_FIXED_BUFFER = 0x02,
    };

    static constexpr size_t valueTypeToSize(ValueType type) {
        switch (type) {
            case ValueType::VTYPE_INT8:
                return sizeof(int8_t);
            case ValueType::VTYPE_UINT8:
                return sizeof(uint8_t);

            case ValueType::VTYPE_INT16:
                return sizeof(int16_t);
            case ValueType::VTYPE_UINT16:
                return sizeof(uint16_t);

            case ValueType::VTYPE_INT32:
                return sizeof(int32_t);
            case ValueType::VTYPE_UINT32:
                return sizeof(uint32_t);

            case ValueType::VTYPE_INT64:
                return sizeof(int64_t);
            case ValueType::VTYPE_UINT64:
                return sizeof(uint64_t);

            case ValueType::VTYPE_FLOAT:
                return sizeof(float);
            case ValueType::VTYPE_DOUBLE:
                return sizeof(double);

            case ValueType::VTYPE_VEC_2I:
            case ValueType::VTYPE_VEC_2F:
                return 2 * sizeof(int32_t);

            case ValueType::VTYPE_VEC_3I:
            case ValueType::VTYPE_VEC_3F:
                return 3 * sizeof(int32_t);

            case ValueType::VTYPE_VEC_4I:
            case ValueType::VTYPE_VEC_4F:
                return 4 * sizeof(int32_t);

            case ValueType::VTYPE_MATRIX:
                return sizeof(float) * 16;

            case ValueType::VTYPE_RECTI:
            case ValueType::VTYPE_RECTF:
                return (2 * (sizeof(int32_t) * 2));

            case ValueType::VTYPE_COLOR24:
                return 3;
            case ValueType::VTYPE_COLOR32:
                return 4;
            case ValueType::VTYPE_COLOR:
                return 16;

            case ValueType::VTYPE_LAYER_MASK:
            case ValueType::VTYPE_SORTING_LAYER:
                return sizeof(uint32_t);

            case ValueType::VTYPE_GAME_OBJECT_REF:
            case ValueType::VTYPE_COMPONENT_REF:
            case ValueType::VTYPE_ASSET_REF:
                return sizeof(uint32_t);
            default: return 0;
        }
    }

    namespace detail {
        struct TSDescriptor {
            Type const* type;

            ValueType valueType;
            SubValueType subValueType;
            NodeType nodeType;

            constexpr TSDescriptor() :
                type{ nullptr },
                valueType{ ValueType::VTYPE_PROPERTY },
                subValueType{ SubValueType::SUB_VTYPE_NONE },
                nodeType{ NodeType::NTYPE_VALUE }
            {}

            constexpr TSDescriptor(Type const& type) :
                type{ &type },
                valueType{ ValueType::VTYPE_PROPERTY },
                subValueType{ SubValueType::SUB_VTYPE_NONE },
                nodeType{ NodeType::NTYPE_VALUE }
            {}

            constexpr TSDescriptor(Type const& type, ValueType vType) :
                type{ &type },
                valueType{ vType },
                subValueType{ SubValueType::SUB_VTYPE_NONE },
                nodeType{ NodeType::NTYPE_VALUE }
            {}

            constexpr TSDescriptor(Type const& type, ValueType vType, SubValueType sVType) :
                type{ &type },
                valueType{ vType },
                subValueType{ sVType },
                nodeType{ NodeType::NTYPE_VALUE }
            {}

            constexpr TSDescriptor(Type const& type, ValueType vType, SubValueType sVType, NodeType nType) :
                type{ &type },
                valueType{ vType },
                subValueType{ sVType },
                nodeType{ nType }
            {}
        };
    }

    namespace Serialization {
        template<typename T>
        static detail::TSDescriptor const& getDescriptor() {
            return detail::TSDescriptor(TypeHelpers::getType<T>());
        }

        template<typename T>
        static detail::TSDescriptor const& getDescriptor(ValueType vType) {
            return detail::TSDescriptor(TypeHelpers::getType<T>(), vType);
        }

        template<typename T>
        static detail::TSDescriptor const& getDescriptor(ValueType vType, SubValueType sVType) {
            return detail::TSDescriptor(TypeHelpers::getType<T>(), vType, sVType);
        }

        template<typename T>
        static detail::TSDescriptor const& getDescriptor(ValueType vType, SubValueType sVType, NodeType nType) {
            return detail::TSDescriptor(TypeHelpers::getType<T>(), vType, sVType, nType);
        }
    }
}