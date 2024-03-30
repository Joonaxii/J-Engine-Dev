#pragma once
#include <JEngine/Algorithm/BinarySearch.h>
#include <cstdint>

namespace JEngine {
    template<typename T>
    class IReferenceProvider {
    public:
        virtual T& operator[](size_t index) = 0;
        virtual const T& operator[](size_t index) const = 0;

        virtual size_t size() const = 0;

        virtual uint32_t push(const T& value) = 0;
        virtual T pop(size_t index) = 0;
        virtual T pop() = 0;

        virtual int32_t indexOf(const T& value) const = 0;
        virtual int32_t lastIndexOf(const T& value) const = 0;

        virtual void clear() = 0;
    };
}