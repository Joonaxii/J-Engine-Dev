#include <JEngine/Utility/EnumUtils.h>

namespace JEngine {
    std::vector<EnumType*>& Enums::getEnums() {
        static std::vector<EnumType*> enums{};
        return enums;
    }
}