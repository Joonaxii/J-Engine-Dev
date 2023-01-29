#pragma once

namespace JEngine {
    class NoCopy {
    public:
        NoCopy() {}
        ~NoCopy() {}

    private:

        NoCopy(const NoCopy&);
        NoCopy& operator =(const NoCopy&);
    };
}
