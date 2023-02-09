#pragma once

namespace JEngine {
    class NoCopy {
    public:
        NoCopy() {}
        ~NoCopy() {}

    private:
        NoCopy(const NoCopy&) = delete;
        NoCopy& operator =(const NoCopy&) = delete;
    };
}
