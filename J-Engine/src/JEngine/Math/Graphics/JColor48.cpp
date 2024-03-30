#include <JEngine/Math/Graphics/JColor48.h>
#include <algorithm>

namespace JEngine {

    void JColor48::set(uint16_t gray) {
        r = g = b = gray;
    }

    void JColor48::set(uint16_t r, uint16_t g, uint16_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void JColor48::flipRB() {
        std::swap(r, b);
    }
}