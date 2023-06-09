#include <JEngine/assets/AssetPacking.h>
#include <JEngine/Utility/DataFormatUtils.h>
#include <JEngine/IO/Serialization/Serializable.h>
#include <vector>

namespace JEngine::AssetPacking {

    const bool packAssets(const FileEntry& folder, const Stream& stream, const uint32_t flags) {
        stream.writeValue(DataHeader<FMT_JPAK>::Value);
        Serialization::serialize(PakVersion, stream);
        return true;
    }
}