#include <JEngine/Math/Graphics/JColor64.h>
#include <algorithm>

namespace JEngine {

    void JColor64::set(uint16_t gray) {
        r = g = b = gray;
    }

    void JColor64::set(uint16_t gray, uint16_t alpha) {
        r = g = b = gray;
        a = alpha;
    }

    void JColor64::set(uint16_t r, uint16_t g, uint16_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void JColor64::set(uint16_t r, uint16_t g, uint16_t b, uint16_t a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    void JColor64::flipRB() {
        std::swap(r, b);
    }
}