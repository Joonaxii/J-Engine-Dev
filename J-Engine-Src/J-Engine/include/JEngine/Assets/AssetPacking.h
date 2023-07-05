#pragma once
#include <string>
#include <fstream>
#include <JEngine/IO/VFS/FileEntry.h>
#include <JEngine/Utility/Version.h>

namespace JEngine::AssetPacking {
    static constexpr const char* PakExtension = ".jpak";
    static const JVersion PakVersion = JVersion(0, 0, 0);

    //const bool packAssets(const FileEntry& folder, std::ofstream& stream, const uint32_t flags = 0U);
}
