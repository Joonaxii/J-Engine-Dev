#pragma once
#include <cstdint>
#include <JEngine/IO/Serialization/SerializationConsts.h>

namespace JEngine {
    struct GUIStyle {
        union Value {
            bool b;
            int8_t i8;
            uint8_t ui8;
            int16_t i16;
            uint16_t ui16;
            int32_t i32;
            uint32_t ui32;
            int64_t i64;
            uint64_t ui64;
            float f;
            double d;
            uint8_t buffer[16];

            constexpr Value() : buffer{ 0 } {}

            constexpr Value(bool v) : b(v) {}

            constexpr Value(int8_t v) : i8(v) {}
            constexpr Value(uint8_t v) : ui8(v) {}

            constexpr Value(int16_t v) : i16(v) {}
            constexpr Value(uint16_t v) : ui16(v) {}

            constexpr Value(int32_t v) : i32(v) {}
            constexpr Value(uint32_t v) : ui32(v) {}

            constexpr Value(int64_t v) : i64(v) {}
            constexpr Value(uint64_t v) : ui64(v) {}

            constexpr Value(float v) : f(v) {}
            constexpr Value(double v) : d(v) {}
        };

        enum StyleFlags : uint32_t {
            FLAG_NONE = 0x00,
            FLAG_MIN = 0x01,
            FLAG_MAX = 0x02,
            FLAG_RANGE = 0x04,
            FLAG_ANGLE = 0x08,

            FLAG_HDR = 0x01,
            FLAG_PICKER = 0x02,

            FLAG_SELECT_SINGLE = 0x01,

            FLAG_FIXED_BUFFER = 0x10,
            FLAG_NO_DRAW = 0x20,

            FLAG_TRANSFORM = 0x1000,

            STYLE_SLIDER = FLAG_MIN | FLAG_MAX | FLAG_RANGE
        };

        StyleFlags flags;
        Value extra[2];
        Value def;
        const char* format;

        constexpr GUIStyle() : flags{ FLAG_NONE }, extra{}, def{}, format{ nullptr } {}
        constexpr GUIStyle(uint32_t flags) : flags{ StyleFlags(flags) }, extra{}, def{}, format{ nullptr } {}

        constexpr GUIStyle(uint32_t flags, int8_t min, int8_t max, int8_t def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, uint8_t min, uint8_t max, uint8_t def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, int16_t min, int16_t max, int16_t def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, uint16_t min, uint16_t max, uint16_t def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, int32_t min, int32_t max, int32_t def) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, uint32_t min, uint32_t max, uint32_t def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, int64_t min, int64_t max, int64_t def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, uint64_t min, uint64_t max, uint64_t def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, float min, float max, float def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        constexpr GUIStyle(uint32_t flags, double min, double max, double def, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), extra{ min, max }, def{ def } { }

        template<typename T>
        explicit GUIStyle(uint32_t flags,
            const T& min = {}, const T& max = {}, const T& def = {}, const char* format = nullptr) :
            flags{ StyleFlags(flags) }, format(format), min{}, max{}, def{} {
            memcpy(extra + 0, &min, std::min(sizeof(T), sizeof(Value)));
            memcpy(extra + 1, &max, std::min(sizeof(T), sizeof(Value)));
            memcpy(&this->def, &def, std::min(sizeof(T), sizeof(Value)));
        }

        constexpr bool is(uint32_t mask) const { return (flags & mask) == mask; }
        constexpr bool has(uint32_t mask) const { return (flags & mask) != 0; }

        constexpr const void* getDefaultValue() const { return &def; }
        constexpr const void* getMin() const { return is(FLAG_MIN) ? extra + 0 : nullptr; }
        constexpr const void* getMax() const { return is(FLAG_MAX) ? extra + 1 : nullptr; }

        template<typename T>
        const T* getDefaultValueAs() const { return reinterpret_cast<const T*>(&def); }
    };

    namespace Gui {
        struct Styles {
            static constexpr GUIStyle Default = {};
            static constexpr GUIStyle Color = { GUIStyle::FLAG_HDR };
            static constexpr GUIStyle ColorPicker = { GUIStyle::FLAG_HDR | GUIStyle::FLAG_PICKER };
            static constexpr GUIStyle Color32Picker = { GUIStyle::FLAG_PICKER };
            static constexpr GUIStyle Slider01F = GUIStyle(GUIStyle::STYLE_SLIDER, 0.0f, 1.0f, 0.0f);

            static constexpr GUIStyle TransformVec = GUIStyle(GUIStyle::FLAG_TRANSFORM);
            static constexpr GUIStyle TransformRot = GUIStyle(GUIStyle::FLAG_TRANSFORM | GUIStyle::FLAG_ANGLE, -360.0f, 360.0f, 0.0f, "%.0f deg");
        };

        bool drawScalar(const char* label, bool isSlider, bool isAngle, int32_t dataType, void* value, float speed, const void* min, const void* max, const char* format, int32_t flags);
        bool drawScalar(const char* label, int32_t dataType, void* value, const GUIStyle& style);


        bool drawScalars(const char* label, bool isSlider, int32_t dataType, void* value, int32_t count, float speed, const void* min, const void* max, const char* format, int32_t flags);
        bool drawScalars(const char* label, int32_t dataType, void* value, int32_t count, const GUIStyle& style);


        bool drawColor(const char* label, float color[4], bool hasAlpha, const GUIStyle& style);
        bool drawColor(const char* label, ValueType colorType, void* value, const GUIStyle& style);

        static bool drawLayerMask(const char* label, void* value, const GUIStyle& style);

        static bool drawSortingLayer(const char* label, void* value, const GUIStyle& style);
        bool drawBuiltIn(const char* label, ValueType type, void* data, const GUIStyle& style);
    }
}