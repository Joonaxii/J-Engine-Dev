#include <JEngine/assets/AssetPacking.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <vector>

namespace JEngine::AssetPacking {

    const bool packAssets(const FileEntry& folder, std::ofstream& stream, const uint32_t flags) {
        stream.write(ASSET_PAK_HEADER, 4);

        Serialization::serialize(PakVersion, stream);
        return true;
    }
}