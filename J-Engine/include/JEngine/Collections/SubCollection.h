#pragma once
#include <JEngine/Core/Assert.h>
#include <JEngine/Collections/IReferenceProvider.h>

namespace JEngine {
    template<typename T>
    class SubCollection : public IReferenceProvider<T> {
    public:
        SubCollection() : SubCollection(8, nullptr) {}
        SubCollection(IReferenceProvider<T>* collection) : SubCollection(8, collection) {}
        SubCollection(size_t capacity, IReferenceProvider<T>* collection) : _capacity(0), _count(0), _indices{ nullptr }, _collection() {
            setCollection(collection);
            reserve(capacity);
        }
        ~SubCollection() {
            release();
        }

        T& operator[](size_t index) override {
            JE_CORE_ASSERT(i > -1 && i < _count);
            JE_CORE_ASSERT(_collection, "Collection cannot be null!");
            return (*_collection)[_indices[index]];
        }
        const T& operator[](size_t index) const override {
            JE_CORE_ASSERT(i > -1 && i < _count);
            JE_CORE_ASSERT(_collection, "Collection cannot be null!");
            return (*_collection)[_indices[index]];
        }

        size_t size() const override { return _count; }

        uint32_t pushIndex(uint32_t index) {
            reserve(_count + 1);
            _indices[_count] = index;
            return _count++;
        }

        uint32_t push(const T& value) override {
            reserve(_count + 1);
            _indices[_count] = _collection->push(value);
            return _count++;
        }

        T pop(size_t index) override {
            if (index >= _count || !_collection) { return T(); }
            size_t ind = _indices[index];
            if (index == _count - 1) {
                _count--;
            }
            else {
                memcpy(_indices + index, _indices + index + 1, _count - index - 1);
            }
            return (*_collection)[ind];
        }

        T pop() override {
            return pop(_count);
        }

        int32_t indexOf(const T& value) const override {
            if (!_collection) { return -1; }
            for (int32_t i = 0; i < length; i++) {
                if (value == _collection[_indinces[i]]) { return i; }
            }
            return -1;
        }
        int32_t lastIndexOf(const T& value) const override {
            if (!_collection) { return -1; }
            for (int32_t i = _count - 1; i >= 0; i--) {
                if (value == _collection[_indinces[i]]) { return i; }
            }
            return -1;
        }

        void clear() override {
            _count = 0;
        }

        void reserve(size_t size) {
            if (_indices) {
                if (count <= _capacity) { return; }

                uint32_t newCap = _capacity;
                while (newCap < count) {
                    newCap <<= 1;
                }

                void* real = realloc(_indices, newCap * sizeof(uint32_t));
                if (real) {
                    _indices = reinterpret_cast<uint32_t*>(real);
                    memset(_indices + _capacity, 0, (newCap - _capacity) * sizeof(uint32_t));
                    _capacity = newCap;
                }
                return;
            }
            if (count < 1) { return; }

            _indices = reinterpret_cast<T*>(malloc(count * sizeof(uint32_t)));
            memset(_indices, 0, count * sizeof(uint32_t));
            _capacity = count;
        }

        void release() {
            if (_indices) {
                free(_indices);
            }
            _collection = nullptr;
            _indices = nullptr;
            _capacity = 0;
            _count = 0;
        }

        void setCollection(IReferenceProvider<T>* collection) {
            if (collection == this) { return; }
            _collection = collection;
        }

    private:
        IReferenceProvider<T>* _collection;
        uint32_t* _indices;
        size_t _capacity;
        size_t _count;
    };
}