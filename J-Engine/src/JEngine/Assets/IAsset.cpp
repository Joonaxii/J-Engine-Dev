#include <JEngine/Assets/IAsset.h>

namespace JEngine {
    void IAsset::init(const char* name, uint32_t uuid, uint16_t flags) {
        INamedObject::initObj(name, uuid, flags);
    }
}

