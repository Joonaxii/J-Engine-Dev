#pragma once
#include <JEngine/Algorithm/BinarySearch.h>
#include <stdint.h>

namespace JEngine {
    template<typename T>
    class IReferenceProvider {
    public:
        IReferenceProvider(const bool isOrdered) : _ordered(isOrdered), _version(0), _count(0), _items(nullptr) {}

        void setVersion(const uint32_t& version) { _version = version; }
        const uint32_t getVersion() const { return _version; }

        const bool versionChanged(const uint32_t& version) const { return _version != version; }

        const int32_t indexOfID(const int32_t id) const {
            if (_ordered)
            {
                auto lambda = [](const T& item, const int32_t& idIn) -> const int32_t { auto cID = item.getId(); if (cID == idIn) { return 0; } return idIn < cID ? 1 : -1; };
                int ind = binSearch<const T, int32_t>(_items, 0, _count, id, lambda);
                return ind;
            }

            for (size_t i = 0; i < _count; i++) {
                if (_items[i].getId() == id) { return int32_t(i); }
            }
            return -1;
        }

        T* getPtr(const int32_t& id, const uint32_t& version) {
            if (_items == nullptr) { return nullptr; }

            const int32_t ind = indexOfID(id);
            return ind < 0 ? nullptr : &_items[ind];
        }
        
        const T* getPtr(const int32_t& id, const uint32_t& version) const {
            if (_items == nullptr) { return nullptr; }

            auto ind = indexOfID(id);
            return ind < 0 ? nullptr : &_items[ind];
        }

        void updateItems(T* items, const int count, const bool changeVersion) {
            _items = items;
            _count = count;

            if (changeVersion) {
                _version++;
            }
        }

        virtual const bool removeById(const T& ref) = 0;
        virtual const bool removeById(const int32_t id) = 0;

    protected:
        int _count;
        T* _items;
        uint32_t _version;
        bool _ordered;
    };
}