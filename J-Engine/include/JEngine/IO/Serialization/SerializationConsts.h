#pragma once
#include <cstdint>

//Builtin types
#include <JEngine/Math/Graphics/JColor.h>
#include <JEngine/Math/Graphics/JColor24.h>
#include <JEngine/Math/Graphics/JColor32.h>
#include <JEngine/Math/Graphics/JColor4444.h>
#include <JEngine/Math/Units/JMatrix.h>
#include <JEngine/Math/Units/JRect.h>
#include <JEngine/Math/Units/JVector.h>
#include <JEngine/Core/LayerMask.h>
#include <JEngine/Rendering/SortingLayer.h>

namespace JEngine {
    enum class VType : uint16_t {
        VTYPE_NONE = 0x00,

        VTYPE_INT8,
        VTYPE_UINT8,

        VTYPE_INT16,
        VTYPE_UINT16,

        VTYPE_INT32,
        VTYPE_UINT32,

        VTYPE_INT64,
        VTYPE_UINT64,

        VTYPE_ENUM,

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

        VTYPE_COLOR24,
        VTYPE_COLOR32,
        VTYPE_COLOR,

        VTYPE_STRING,

        VTYPE_LAYER_MASK,
        VTYPE_SORTING_LAYER,

        VTYPE_GAME_OBJECT_REF,
        VTYPE_COMPONENT_REF,
        VTYPE_ASSET_REF,

        VTYPE_DATA,
    };

    static constexpr size_t valueTypeToSize(VType type) {
        switch (type) {
        case VType::VTYPE_INT8:
            return sizeof(int8_t);
        case VType::VTYPE_UINT8:
            return sizeof(uint8_t);

        case VType::VTYPE_INT16:
            return sizeof(int16_t);
        case VType::VTYPE_UINT16:
            return sizeof(uint16_t);

        case VType::VTYPE_INT32:
            return sizeof(int32_t);
        case VType::VTYPE_UINT32:
            return sizeof(uint32_t);

        case VType::VTYPE_INT64:
            return sizeof(int64_t);
        case VType::VTYPE_UINT64:
            return sizeof(uint64_t);

        case VType::VTYPE_FLOAT:
            return sizeof(float);
        case VType::VTYPE_DOUBLE:
            return sizeof(double);

        case VType::VTYPE_VEC_2I:
            return sizeof(JVector2i);
        case VType::VTYPE_VEC_2F:
            return sizeof(JVector2f);

        case VType::VTYPE_VEC_3I:
            return sizeof(JVector3i);
        case VType::VTYPE_VEC_3F:
            return sizeof(JVector3f);

        case VType::VTYPE_VEC_4I:
            return sizeof(JVector4i);
        case VType::VTYPE_VEC_4F:
            return sizeof(JVector4f);

        case VType::VTYPE_MATRIX:
            return sizeof(JMatrix4f);

        case VType::VTYPE_RECTI:
            return sizeof(JRecti);
        case VType::VTYPE_RECTF:
            return sizeof(JRectf);

        case VType::VTYPE_COLOR24:
            return sizeof(JColor24);
        case VType::VTYPE_COLOR32:
            return sizeof(JColor32);
        case VType::VTYPE_COLOR:
            return sizeof(JColor);

        case VType::VTYPE_LAYER_MASK:
            return sizeof(LayerMask);
        case VType::VTYPE_SORTING_LAYER:
            return sizeof(SortingLayer);

        case VType::VTYPE_GAME_OBJECT_REF:
        case VType::VTYPE_COMPONENT_REF:
        case VType::VTYPE_ASSET_REF:
            return sizeof(Ref);
        default: return 0;
        }
    }

}