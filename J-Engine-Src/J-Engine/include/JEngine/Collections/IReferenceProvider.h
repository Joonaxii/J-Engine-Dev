#pragma once
#include <JEngine/Algorithm/BinarySearch.h>
#include <stdint.h>

namespace JEngine {
    static constexpr uint32_t UINT_20_MAX = 0xFFFFFU;
    static constexpr uint32_t UINT_24_MAX = 0xFFFFFFU;
    static constexpr uint32_t UINT_12_MAX = 0xFFFU;

    struct RefID {
        uint32_t data;

        RefID(const uint32_t id, const uint32_t version) {
            data = id & UINT_20_MAX;
            data |= (version & UINT_12_MAX) << 20;
        }

        void setId(const uint32_t id) {
            data &= UINT_12_MAX;
            data |= id & UINT_20_MAX;
        }

        uint32_t getId() const {
            return (data & UINT_20_MAX);
        }

        void setVersion(const uint16_t version) {
            data &= UINT_20_MAX;
            data |= uint32_t(version & UINT_12_MAX) << 20;
        }

        uint16_t getVersion() const {
            return uint16_t((data >> 20) & UINT_12_MAX);
        }
    };

    template<typename T>
    class IReferenceProvider {
    public:
        IReferenceProvider(const bool isOrdered) : _ordered(isOrdered), _version(0), _count(0), _items(nullptr) {}

        void setVersion(const uint16_t& version) { _version = (version & UINT_12_MAX); }
        const uint16_t getVersion() const { return _version; }

        bool hasToUpdate(const uint16_t& version) const  { 
            return _version != (version & UINT_12_MAX);
        }

        uint32_t indexOfID(const uint32_t id, const uint32_t defaultVal = -1) const {
            if (_ordered)
            {
                auto lambda = [](const T& item, const uint32_t& idIn) -> const int32_t { auto cID = item.getId(); if (cID == idIn) { return 0; } return int32_t(idIn < cID ? 1 : -1); };
                int32_t ind = binSearch<const T, uint32_t, int32_t>(_items, 0, _count, id, lambda);
                return ind < 0 ? defaultVal : uint32_t(ind);
            }

            for (uint32_t i = 0; i < _count; i++) {
                if (_items[i].getId() == id) { return i; }
            }
            return defaultVal;
        }

        T* getPtrByIndex(const uint32_t index, RefID& refId) {
            if (_items == nullptr) { return nullptr; }
            refId.setVersion(_version);
            return index >= _count ? nullptr : &_items[index];
        }

        const T* getPtrByIndex(const uint32_t index, RefID& refId) const {
            if (_items == nullptr) { return nullptr; }
            refId.setVersion(_version);
            return index >= _count ? nullptr : &_items[index];
        }

        T* getPtr(const uint32_t id, RefID& refId) {
            if (_items == nullptr) { return nullptr; }

            refId.setVersion(_version);
            auto ind = indexOfID(id);
            return ind < 0 ? nullptr : &_items[ind];
        }

        const T* getPtr(const uint32_t id, RefID& refId) const {
            if (_items == nullptr) { return nullptr; }

            refId.setVersion(_version);
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

        virtual bool removeById(const T& ref) = 0;
        virtual bool removeById(const uint32_t id) = 0;

    protected:
        uint32_t _count;
        T* _items;
        uint16_t _version;
        bool _ordered;
    };
}