#pragma once
#include <JEngine/Core/Memory.h>
#include <JEngine/Collections/Algorithm.h>

namespace JEngine {

    template<typename T, typename Comparer = DefaultComparer, bool AllowDuplicates = false, size_t ALIGNMENT = ALIGN_1>
    class SortedVector {
    public:
        constexpr SortedVector() : _length(0), _capacity(0), _buffer{ nullptr } {}
        SortedVector(std::initializer_list<T> il) : SortedVector() {
            resize(il.size());
            for (const T& in : il) {
                this->push_back(in);
            }
        }

        SortedVector(const SortedVector<T, Comparer, AllowDuplicates, ALIGNMENT>& other) : SortedVector() {
            resize(other._capacity);
            for (auto& in : il) {
                _buffer[_length++] = in;
            }
        }

        SortedVector(const T* beg, const T* end) : SortedVector() {
            resize(size_t(end - beg));
            while (beg < end) {
                this->push_back(*beg++);
            }
        }

        template<typename Collection>
        SortedVector(const Collection& collection) : SortedVector() {
            const T* beg = collection.begin();
            const T* end = collection.end();
            resize(size_t(end - beg));
            while (beg < end) {
                push_back(*beg++);
            }
        }

        ~SortedVector() {
            release();
        }

        const T& operator[](size_t i) const {
            JE_CORE_ASSERT(i < _length, "Index out of range!");
            JE_CORE_ASSERT(_buffer, "Vector not intialized!");
            return _buffer[i];
        }

        const T& at(size_t i) const {
            JE_CORE_ASSERT(i < _length, "Index out of range!");
            JE_CORE_ASSERT(_buffer, "Vector not intialized!");
            return _buffer[i];
        }

        bool push_back(const T& value) {
            size_t ind = this->nearest(item);
            if constexpr (!AllowDuplicates) {
                if (ind < _length && Comparer::isEqual(_buffer[ind], value)) {
                    return false;
                }
            }
            reserve(Math::max(index, _length) + 1, true);
            if(ind >= _length - 1) {
                _buffer[ind] = value;
                return true;
            }
            JE_MEMSHIFT_T(_buffer + ind, ind - _length, 1);
            _buffer[ind] = value;
            _length++;
            return true;
        }

        template<typename U = T>
        size_t nearest(const U& value) const {
            const T* beg = this->begin();
            const T* end = this->end();
            size_t m = 0;

            if (beg < end) {
                size_t l = 0;
                size_t r = size_t(end - beg) - 1;

                while (l <= r) {
                    m = l + (r - l) / 2;
                    if (Comparer::isEqual(beg[m], value)) {
                        return m;
                    }
                    else if (Comparer::isLess(beg[m], value)) {
                        l = m + 1;
                    }
                    else {
                        r = m - 1;
                    }
                }
            }
            return m;
        }

        template<typename U = T>
        size_t indexOf(const U& value) const { 
            return Algo::binSearch(begin(), end(), value);
        }

        T* begin() { return _buffer; }
        T* end() { return _buffer + _length; }

        const T const* begin() const { return _buffer; }
        const T const* end() const { return _buffer + _length; }
        constexpr size_t size() const { return _length; }

        void removeAt(size_t index) {
            JE_CORE_ASSERT(index < _length, "Index out of range!");
            --_length;
            if (index == _length) {
                return;
            }
            JE_COPY_T(_buffer + index, _buffer + index + 1, _length - index)
        }

        bool remove(const T& value) {
            size_t ind = Algo::indexOf<T>(begin(), end(), value);
            if (ind == SIZE_MAX) { return false; }
            removeAt(ind);
            return true;
        }

        void reserve(size_t amount, bool scale = false) {
            if (!_buffer || amount > _capacity) {
                resize(amount, scale);
            }
        }

        void resize(size_t amount, bool scale = false) {
            if (scale) {
                amount += Math::max(amount >> 1, 1);
            }

            if constexpr (std::is_trivially_copyable<T>::value) {
                T* newAlloc = nullptr;
                if constexpr (ALIGNMENT > 1) {
                    newAlloc = JE_REALLOC_ALIGNED_T(ALIGNMENT, T, _buffer, amount);
                }
                else {
                    newAlloc = JE_REALLOC_T(T, _buffer, amount);
                }

                if (newAlloc != _buffer) {
                    if (amount > _length) {
                        JE_ZERO_MEM(newAlloc + _length, amount - _length);
                    }
                    _buffer = newAlloc;
                    _capacity = amount;
                    _length = Math::min(_length, _capacity);
                }
            }
            else {
                T* newAlloc = nullptr;
                if constexpr (ALIGNMENT > 1) {
                    newAlloc = JE_ALLOC_ALIGNED_T(ALIGNMENT, T, amount);
                }
                else {
                    newAlloc = JE_ALLOC_T(T, amount);
                }

                if (newAlloc) {
                    size_t toCopy = Math::min(_length, amount);
                    std::move<T>(_buffer, _buffer + toCopy, newAlloc);
                    if (amount > _length) {
                        JE_ZERO_MEM(newAlloc + _length, amount - _length);
                    }
                    _capacity = amount;

                    JE_FREE(_buffer);
                    _buffer = newAlloc;
                    _length = Math::min(_length, _capacity);
                }
            }
        }

        void release() {
            if (_buffer) {
                if constexpr (ALIGNMENT > 1) {
                    JE_FREE_ALIGNED(ALIGNMENT, _buffer);
                }
                else {
                    JE_FREE(_buffer);
                }
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