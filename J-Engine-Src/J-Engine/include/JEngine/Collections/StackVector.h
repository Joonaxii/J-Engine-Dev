#pragma once
#include <JEngine/Core/Memory.h>
namespace JEngine {
    template<typename T, size_t SIZE>
    class StackVector {
    public:
        constexpr StackVector() : _count(0), _buffer() {}

        T& operator[](size_t i) {
            JE_CORE_ASSERT(i < SIZE, "Index out of buffer range!");
            return _buffer[i];
        }

        const T& operator[](size_t i) const {
            JE_CORE_ASSERT(i < SIZE, "Index out of buffer range!");
            return _buffer[i];
        }

        constexpr size_t capacity() const { return SIZE; }
        constexpr size_t size() const { return _size; }

        void clear() {
            _size = 0;
        }

        void push_back(const T& item) {
            if (_size >= SIZE) {
                JE_CORE_WARN("Cannot push more items into StackVector!");
                return;
            }
            _buffer[_size++] = item;
        }

    private:
        size_t _size{};
        T _buffer[SIZE];
    };
}