#pragma once
#include <JEngine/Collections/PoolAllocator.h>
#include <JEngine/IO/Serialization/Serialize.h>
#include <JEngine/Utility/EnumUtils.h>

namespace JEngine {
    enum MessageFlagType : size_t {
        MSG_Components = 0x00,
        MSG_Flags,
    };

    enum class MessageFlags : uint32_t {
        None            = 0x00,

        Component0      = 0x0001,
        Component1      = 0x0002,
        Component2      = 0x0004,
        Component3      = 0x0008,
        Component4      = 0x0010,
        Component5      = 0x0020,
        Component6      = 0x0040,
        Component7      = 0x0080,

        AllComponents   = 0xFF,

        AllowPass       = 0x0100,
        AllowInactive   = 0x0200,
        KeepMask        = 0x0400,
    };
    CREATE_ENUM_OPERATORS(MessageFlags);
    DEFINE_ENUM_ID(MessageFlags, MSG_Components, true, 0, 8, "Component 0", "Component 1", "Component 2", "Component 3", "Component 4", "Component 5", "Component 6", "Component 7");
    DEFINE_ENUM_ID(MessageFlags, MSG_Flags     , true, 8, 3, "Allow Pass-through", "Allow Inactive", "Keep Component Mask");

    struct MessageData {

    };

    struct Message {
        CompRef sender{};
        MessageFlags flags{};
        MessageData* data{};
    };
}