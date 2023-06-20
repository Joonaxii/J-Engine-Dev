#include <JEngine/Components/CTransform.h>

namespace JEngine {
    CTransform::~CTransform() {

    }

    CTransform::CTransform(GameObject* obj, uint16_t flags) : Component(obj, flags) {

    }
}