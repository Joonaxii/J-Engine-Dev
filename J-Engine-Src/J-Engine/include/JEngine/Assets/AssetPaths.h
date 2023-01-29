#pragma once
#include <string>

namespace JEngine {
    namespace AssetPaths {
#if _DEBUG
        static const std::string DEBUG_ASSET_PATH = "../../../Game/res";
#endif

        static const std::string ASSET_PACKAGE_EXT = ".jpak";
        static const std::string ASSET_ROOT = "res/";

        static const std::string SPRITE_ROOT = ASSET_ROOT + "Sprites/";
        static const std::string FONT_ROOT = ASSET_ROOT + "Fonts/";
        static const std::string SCENE_ROOT = ASSET_ROOT + "Scenes/";
        static const std::string TILESET_ROOT = ASSET_ROOT + "Tilesets/";
        static const std::string MAP_ROOT = ASSET_ROOT + "Maps/";
    }
}