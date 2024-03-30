#pragma once
#include <cstdint>

namespace JEngine {
    class IRef {
    public:
        static constexpr uint32_t NULL_ID = 0xFFFFFFU;

        IRef() : _id(NULL_ID) {}
        IRef(const uint32_t id) : _id(id & NULL_ID) {}

        void setId(const uint32_t id) { _id = id & NULL_ID; }
        uint32_t getId() const { return _id; }

    private:
        uint32_t _id;
    };
}