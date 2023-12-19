#pragma once
#include <JEngine/Core/Assert.h>
#include <JEngine/Collections/IReferenceProvider.h>
#include <JEngine/Collections/SubCollection.h>
#include <JEngine/Algorithm/BinarySearch.h>
#include <math.h>
#include <assert.h>

namespace JEngine {
    template<typename T>
    class ReferenceVector : public IReferenceProvider<T> {
    public:
        template<typename U>
        using DoCompare = bool(*)(const T&, const U&);

        ReferenceVector() : ReferenceVector(8, T()) {}
        ReferenceVector(uint32_t capacity) : ReferenceVector(capacity, T()) {}
        ReferenceVector(uint32_t capacity, const T& defVal) :
            _items(nullptr), _auxBuffer(nullptr),
            _capacity(0), _tail(0), _auxCount(0), _count(0), _default(defVal) {
            reserve(capacity);
        }

        ~ReferenceVector() {
            release();
        }

        T& operator[](size_t i) override {
            JENGINE_CORE_ASSERT(i >= 0 && i < _tail);
            return _items[i];
        }

        const T& operator[](size_t i) const override {
            JENGINE_CORE_ASSERT(i >= 0 && i < _tail);
            return _items[i];
        }
        size_t size() const override { return _count; }
        size_t tail() const { return _tail; }

        uint32_t push(const T& value) override {
            if (_auxCount) {
                uint32_t index = _auxBuffer[_auxCount--];
                _items[index] = value;
                _count++;
                return index;
            }
            reserve(_tail + 1);
            _items[_tail] = value;
            _count++;
            return _tail++;
        }

        template<class... Args>
        uint32_t pushArgs(Args&&... args) {
            if (_auxCount) {
                uint32_t index = _auxBuffer[_auxCount--];
                _items[index] = T(args...);
                _count++;
                return index;
            }
            reserve(_tail + 1);
            _items[_tail] = T(args...);
            _count++;
            return _tail++;
        }

        T pop(size_t index) override {
            if (index >= _tail) { return _default; }
            if (_items[index] == _default) { return _default; }
            T temp = _items[index];
            _items[index] = _default;
            _auxBuffer[_auxCount++] = uint32_t(index);
            _count--;
            return temp;
        }

        T pop() override {
            for (int32_t i = int32_t(_tail) - 1; i >= 0; i--) {
                if (_items[i] == _default) { continue; }
                return pop(i);
            }
            return T();
        }

        void build(SubCollection<T>& other) const {
            if (&other == this) { return; }
            for (uint32_t i = 0; i < _tail; i++) {
                if (_items[i] == _default) { continue; }
                other.pushIndex(i);
            }
        }

        size_t build(uint32_t* buffer, size_t maxCount) const {
            if (buffer == nullptr || maxCount < 1) { return 0; }
            size_t count = 0;
            for (uint32_t i = 0; i < _tail; i++) {
                if (_items[i] == _default) { continue; }
                buffer[count++] = i;
                if (count >= maxCount) { break; }
            }
            return count;
        }

        template<size_t bufSize>
        size_t build(uint32_t buffer[bufSize]) const {
            return build(buffer, bufSize);
        }

        size_t copyTo(T* buffer, size_t maxCount) const {
            if (buffer == nullptr || maxCount < 1) { return 0; }
            size_t count = 0;
            for (uint32_t i = 0; i < _tail; i++) {
                if (_items[i] == _default) { continue; }
                buffer[count++] = _itmes[i];
                if (count >= maxCount) { break; }
            }
            return count;
        }

        template<typename T, size_t bufSize>
        size_t copyTo(T buffer[bufSize]) const {
            return copyTo(buffer, bufSize);
        }

        void reserve(uint32_t count) {
            if (_items) {
                if (count <= _capacity) { return; }

                uint32_t newCap = _capacity;
                while (newCap < count) {
                    newCap <<= 1;
                }

                void* real = realloc(_items, newCap * sizeof(T));
                if (real) {
                    _items = reinterpret_cast<T*>(real);
                    memset(_items + _capacity, 0, (newCap - _capacity) * sizeof(T));

                    real = realloc(_auxBuffer, newCap * sizeof(uint32_t));
                    if (real) {
                        _auxBuffer = reinterpret_cast<uint32_t*>(real);
                        memset(_auxBuffer + _capacity, 0, (newCap - _capacity) * sizeof(uint32_t));
                    }
                    _capacity = newCap;
                }
                return;
            }
            if (count < 1) { return; }

            _items = reinterpret_cast<T*>(malloc(count * sizeof(T)));
            _auxBuffer = reinterpret_cast<uint32_t*>(malloc(count * sizeof(uint32_t)));

            if (_auxBuffer && _items) {
                memset(_items, 0, count * sizeof(T));
                memset(_auxBuffer, 0, count * sizeof(uint32_t));
                _capacity = count;
            }
        }

        void release() {
            if (_items) {
                free(_items);
            }

            if (_auxBuffer) {
                free(_auxBuffer);
            }

            _items = nullptr;
            _auxBuffer = nullptr;
            _count = 0;
            _tail = 0;
            _auxCount = 0;
            _capacity = 0;
        }

        int32_t indexOf(const T& value) const override {
            if (value == _default) { return -1; }
            for (size_t i = 0; i < _tail; i++) {
                if (_items[i] == value) {
                    return int32_t(i);
                }
            }
            return -1;
        }

        int32_t lastIndexOf(const T& value) const override {
            if (value == _default) { return -1; }
            for (int32_t i = int32_t(_tail) - 1; i >= 0; i--) {
                if (_items[i] == value) {
                    return int32_t(i);
                }
            }
            return -1;
        }
 
        template<typename U>
        int32_t indexOfType(const U& value, DoCompare<U> isEqual) const {
            for (uint32_t i = 0; i < _tail; i++) {
                if (_items[i] != _default && isEqual(_items[i], value)) {
                    return int32_t(i);
                }
            }
            return -1;
        }

        template<typename U>
        int32_t lastIndexOfType(const U& value, DoCompare<U> isEqual) const {
            for (size_t i = _tail - 1; i >= 0; i--) {
                if (_items[i] != _default && isEqual(_items[i], value)) {
                    return int32_t(i);
                }
            }
            return -1;
        }

        void clear() override {
            _tail = 0;
            _count = 0;
            _auxCount = 0;
        }

    private:
        T _default;
        T* _items;
        uint32_t _capacity;
        uint32_t _count;
        uint32_t _tail;

        uint32_t* _auxBuffer;
        uint32_t _auxCount;
    };

}