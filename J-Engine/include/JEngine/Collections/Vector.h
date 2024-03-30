#pragma once
#include <JEngine/Core/Memory.h>

namespace JEngine {
    template<typename T>
    class Vector {
    public:
        constexpr Vector() : _length(0), _capacity(0), _buffer{nullptr} {}

        T& at(size_t i) {
            JE_CORE_ASSERT(i < _length, "Index out of range!");
            JE_CORE_ASSERT(_buffer, "Vector not intialized!");
            return _buffer[i];
        }

        const T& at(size_t i) const {
            JE_CORE_ASSERT(i < _length, "Index out of range!");
            JE_CORE_ASSERT(_buffer, "Vector not intialized!");
            return _buffer[i];
        }

        template<class... Args>
        T& emplace_back(Args&&... args) {

        }

        void reserve(size_t amount) {
            if(!_buffer && amount)
        }

        constexpr size_t size() const { return _length; }

        void resize(size_t amount) {
            if (std::is_trivially_copyable<T>::value) {
                T* newAlloc = JE_REALLOC_T(T, _buffer, amount);
                if (newAlloc != _buffer) {
                    _buffer = newAlloc;
                    _capacity = amount;

                    _length = Math::min(_length, _capacity);
                }
            }
            else {

            }
        }

        void release() {
            if (_buffer) {
                A::free(_buffer);
                _buffer = nullptr;
                _length = 0;
                _capacity = 0;
            }
        }

    private:
        size_t _length;
        size_t _capacity;
        T* _buffer;
    };
}