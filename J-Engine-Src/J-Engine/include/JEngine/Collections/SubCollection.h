#pragma once
#include <JEngine/Collections/IReferenceProvider.h>

namespace JEngine {
    template<typename T>
    class SubCollection {
    public:

    private:
        IReferenceProvider<T>* _collection{};
        
    };
}