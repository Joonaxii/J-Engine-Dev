#pragma once
#include <JEngine/Collections/IReferenceProvider.h>
#include <JEngine/Algorithm/BinarySearch.h>
#include <math.h>
#include <assert.h>

namespace JEngine {
    template<typename T>
    class ReferenceVector : public IReferenceProvider<T> {
    public:
        ReferenceVector() : ReferenceVector(8, false) {}
        ReferenceVector(const bool isOrdered) : ReferenceVector(8, isOrdered) {}
        ReferenceVector(const uint32_t capacity, const bool isOrdered) : _capacity(0), IReferenceProvider<T>(isOrdered) {
            reserve(capacity);
        }

        ~ReferenceVector() {
            if (_items) {
                delete[] _items;
            }

            _items = nullptr;
            _count = 0;
            _capacity = 0;
        }

        T& operator[](int i) {
            assert(i > -1 && i < _count);
            return _items[i];
        }

        const T& operator[](int i) const {
            assert(i > -1 && i < _count);
            return _items[i];
        }

        size_t size() const { return _count; }

        T& push_back(const T& ref) {
            int32_t newC = _count + 1;

            if (newC >= _capacity) {
                auto cap = _capacity;

                if (cap > 0) {
                    while (cap < newC) {
                        cap <<= 1;
                    }
                }
                else {
                    cap = newC;
                }              
                reserve(cap);
            }
            _items[_count] = ref;
            auto& item = _items[_count++];
            uint32_t id = T::CUR_ID++;

            item.setId(id);
            return item;
        }

        bool remove(const T& ref) override {
            auto ind = IReferenceProvider<T>::indexOf(ref);
            return removeAt(ind);
        }

        bool removeByID(const uint32_t id) override {
            auto ind = IReferenceProvider<T>::indexOfID(id);
            return removeAt(ind);
        }

        bool removeAt(const uint32_t index) {
            if (index >= _count) { return false; }

            uint32_t count = _count - index - 1;
            if (count > 0) {
                memcpy((_items + index), (_items + index + 1), (count * sizeof(T)));
            }
            _count--;
            return true;
        }

        void resize(uint32_t count, const bool clear) {
            if (count == _capacity) { return; }

            if (count <= 0) {
                if (_items) {
                    delete[] _items;
                }
                _capacity = 0;
                _count = 0;
                _items = nullptr;
                IReferenceProvider<T>::updateItems(_items, 0, true);
            }
            else {
                _count = clear ? 0 : std::min(_count, count);
                reserve(count, true);
            }
        }

        void reserve(uint32_t amount, const bool force = false) {
            if (amount < _capacity && !force) { return; }

            auto prev = _items;
            _items = new T[amount]{T()};

            if (prev) {
                const int minV = std::min(_count, amount);
                if (minV > 0) {
                    memcpy(_items, prev, minV * sizeof(T));
                }
                delete[] prev;
            }
            _capacity = amount;
        }

        uint32_t indexOf(const T& type) const {
            for (uint32_t i = 0; i < _count; i++) {
                if (_items[i] == type) { return i; }
            }
            return UINT_24_MAX;
        }

    private:
        uint32_t _capacity;
    };
}