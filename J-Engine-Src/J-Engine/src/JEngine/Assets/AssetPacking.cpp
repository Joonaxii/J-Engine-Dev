#include <JEngine/assets/AssetPacking.h>
#include <JEngine/Assets/AssetDataType.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <vector>

namespace JEngine::AssetPacking {

    const bool packAssets(const FileEntry& folder, std::ofstream& stream, const uint32_t flags) {
        stream.write(assetDataFormatToHeader(AssetDataFormat::JPAK), 4);

        Serialization::serialize(PakVersion, stream);
        return true;
    }
}