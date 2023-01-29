#pragma once
#include <cstdint>

namespace JEngine {
    class IRef {
    public:
        IRef() : _id(-1) {}
        IRef(const int32_t id) : _id(id) {}

        void setId(const int32_t id) { _id = id; }
        const int32_t getId() const { return _id; }

    protected:
        int32_t _id;
    };
}