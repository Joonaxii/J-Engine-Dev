#pragma once
#include <cstdint>

namespace JEngine {
    enum class AssetFileType : uint8_t {
        None = 0x0,

        Source = 0x1,
        Binary = 0x2,
        JSON = 0x4,

        All = Source | Binary | JSON,
    };

    inline const AssetFileType operator~(const AssetFileType& lhs) {
        return static_cast<AssetFileType>(~static_cast<uint32_t>(lhs));
    }

    inline const AssetFileType operator|(const AssetFileType& lhs, const AssetFileType& rhs) {
        return static_cast<AssetFileType>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
    }

    inline const AssetFileType operator&(const AssetFileType& lhs, const AssetFileType& rhs) {
        return static_cast<AssetFileType>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
    }

    enum class AssetType : uint32_t {
        None = 0,

        Other = 0x1,

        RenderTexture = 0x2,
        Texture2D = 0x4,
        Sprite = 0x8,
        Atlas = 0x10,

        Scene = 0x20,

        Font = 0x40,

        Audio = 0x80,

        Shader = 0x100,
        Material = 0x200,

        Text = 0x400,
        Binary = 0x800,
    };

    inline const AssetType operator~(const AssetType& lhs) {
        return static_cast<AssetType>(~static_cast<uint32_t>(lhs));
    }

    inline const AssetType operator|(const AssetType& lhs, const AssetType& rhs) {
        return static_cast<AssetType>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
    }

    inline const AssetType operator&(const AssetType& lhs, const AssetType& rhs) {
        return static_cast<AssetType>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
    }
}