#pragma once
#include <cstdint>
#include <string>
#include <cassert>
#include <malloc.h>

namespace JEngine {
    template<typename T>
    class HeapVector {
    public:
        HeapVector() : HeapVector(8) { }

        HeapVector(const int32_t capacity) : _items(nullptr), _capacity(0), _count(0) {
            reserve(capacity);
        }

        HeapVector(const int32_t size, const T& init) : _items(nullptr), _capacity(0), _count(0) {
            resize(size);
            std::fill_n(_items, size, init);
        }

        HeapVector(const T* beg, const T* end) : _items(nullptr), _capacity(0), _count(0) {
            int32_t cap = int32_t(end - beg);
            reserve(cap);
            memcpy(_items, beg, cap * sizeof(T));
            _count = cap;
        }

        ~HeapVector() {
            if (_items) {
                free(_items);
            }
        }

        static bool isDefault(const T& value);
        static void makeDefault(T& value);

        T& operator [](int i) { return _items[i]; }
        const T& operator [](int i) const { return _items[i]; }

        size_t size() const { return _count; }
        size_t capacity() const { return _capacity; }

        const T* begin() const { return _items; }
        const T* end() const { return _items + _count; }

        T* data() { return _items; }
        //Same as begin, here just to have a const version of data()
        const T* data() const { return _items; }

        void clear() {
            _count = 0;
        }

        void push(const T& item, const bool skipIfDefault) {
            if (skipIfDefault && isDefault(item)) { return; }

            checkForExpansion();

            T empt{};
            makeDefault(empt);
            memcpy(_items + _count, &empt, sizeof(T));

            _items[_count++] = item;
        }

        void push(const T& item) { push(item, false); }

        int32_t indexOf(const T& item) const {
            for (int32_t i = 0; i < _count; i++) {
                if (_items[i] == item) {
                    return i;
                }
            }
            return -1;
        }

        template<typename U>
        int32_t indexOf(const U& item) const {
            for (int32_t i = 0; i < _count; i++) {
                if (_items[i] == item) {
                    return i;
                }
            }
            return -1;
        }

        template<typename U>
        int32_t indexOf(const U& item, bool(*predicate)(const T& a, const U& b)) const {
            for (int32_t i = 0; i < _count; i++) {
                if (predicate(_items[i], item)) {
                    return i;
                }
            }
            return -1;
        }

        template<typename U>
        int32_t indexOf(const T& item, bool(*predicate)(const T& a, const U& b)) const {
            for (int32_t i = 0; i < _count; i++) {
                if (predicate(_items[i], item)) {
                    return i;
                }
            }
            return -1;
        }

        bool removeAt(const int32_t index, const bool markDefault) {
            if (index < 0 || index >= _count) { return false; }

            if (markDefault) {
                makeDefault(_items[index]);
                return true;
            }

            if (index == _count - 1) {
                _count--;
                return true;
            }

            if (index == 0) {
                memcpy(_items, _items + 1, --_count);
                return true;
            }

            memcpy(_items + index, _items + index + 1, (_count - index - 1));
            _count--;
            return true;
        }
        bool removeAt(const int32_t index) { return removeAt(false); }

        bool removeAt(const int32_t* indices, int32_t count, const bool defrag) {
            if (!indices || !count) { return false; }

            while (count--) {
                int32_t index = *indices++;
                if (index < 0 || index >= _count) { continue; }
                makeDefault(_items[index]);
            }

            if (defrag) {
                defragment();
            }
            return true;
        }

        bool remove(const T& item, const bool markDefault) {
            int32_t ind = indexOf(item);
            return ind >= 0 && removeAt(ind, markDefault);
        }
        bool remove(const T& item) { return remove(item, false); }

        bool remove(const T* items, const int32_t count, const bool defrag) {
            if (!items || !count) { return false; }

            int32_t* temp = reinterpret_cast<int32_t*>(_malloca(count * sizeof(int32_t)));
            if (!temp) { return false; }

            for (size_t i = 0; i < count; i++) {
                temp[i] = indexOf(items[i]);
            }

            bool deletedAny = removeAt(temp, count, defrag);
            _freea(temp);
            return deletedAny;
        }

        void resize(const int32_t newSize) {
            if (newSize == _count || newSize < 0) { return; }

            T def{};
            makeDefault(def);

            if (_items) {
                void* reLoc = realloc(_items, newSize * sizeof(T));
                if (reLoc) {
                    _items = reinterpret_cast<T*>(reLoc);
                    int32_t lastC = _count;
                    _count = newSize;
                    if (lastC < _count) {
                        std::fill_n(_items + lastC, _count - lastC, def);
                    }
                    _capacity = newSize;
                }
                return;
            }

            _items = reinterpret_cast<T*>(malloc(newSize * sizeof(T)));
            if (_items) {
                _capacity = newSize;
                _count = newSize;
                std::fill_n(_items, _count, def);
            }
        }

        void reserve(const int32_t capacity) {
            if (!_items) {
                _items = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
                _capacity = capacity;
                memset(_items, 0, capacity * sizeof(T));
                return;
            }

            if (capacity > _capacity) {
                void* reLoc = malloc(capacity * sizeof(T));
                if (reLoc) {
                    _items = reinterpret_cast<T*>(reLoc);
                    memset(_items + _capacity, 0, (capacity - _capacity) * sizeof(T));
                    _capacity = capacity;
                }
                return;
            }
        }

        void defragment() {
            if (!_items) { return; }

            if (_count < 2) {
                if (_count && isDefault(_items[0])) {
                    _count = 0;
                }
                return;
            }

            //Node used to determine blocks of contiguous data
            struct Node {
                int32_t count;
                int32_t index;
                int32_t newIndex;
            };

            //Allocate Nodes on stack (if possible) half the amount of items in the vector. 
            //Worst case should be every other element is default, hence half the size + 1 as buffer.
            size_t stackSize = ((_count * sizeof(Node)) >> 1) + sizeof(Node);

            Node* nodes = reinterpret_cast<Node*>(_malloca(stackSize));
            if (!nodes) { return; }
            memset(nodes, 0, stackSize);

            int32_t nodeN = 0;
            int32_t total = 0;

            bool wasDef = false;

            //Loop over every item in the vector, test if it's "default" and build the node stack
            for (int32_t i = 0; i < _count; i++) {
                bool isDef = isDefault(_items[i]);

                Node& cNode = nodes[nodeN];
                //If this item is default
                if (isDef) {
                    //Check if the previous item was default
                    //If yes, then push this block into the node stack
                    if (!wasDef) {
                        if (cNode.count > 0) {
                            //New index is determined by total number of "non-default" items found so far
                            cNode.newIndex = total - cNode.count;
                            nodeN++;
                        }
                    }
                }
                else {
                    //If this item is not "default", check if it's the first in the node 
                    //and set the starting index accordingly
                    if (cNode.count == 0) { cNode.index = i; }
                    cNode.count++;
                    total++;
                }
                wasDef = isDef;

                //Push last node to the node stack if required
                if (cNode.count && i == _count - 1) {
                    cNode.newIndex = total - cNode.count;
                    nodeN++;
                }
            }

            //Loop over nodes and check if a memcpy is required
            for (size_t i = 0; i < nodeN; i++) {
                const Node& node = nodes[i];
                assert(node.newIndex <= node.index && "New index should always be equal or less than current!");

                //If the calculated new index is less than the actual index, copy block backwards to the new index
                if (node.newIndex >= node.index) { continue; }
                memcpy(_items + node.newIndex, _items + node.index, node.count * sizeof(T));
            }
            _count = total;

            //Remember to free node stack if that's required
            _freea(nodes);
        }

    private:
        T* _items;
        int32_t _capacity;
        int32_t _count;

        void checkForExpansion() {
            if (!_items && _count >= _capacity) {
                size_t newCap = _capacity ? _capacity : 1;
                while (newCap < _count) {
                    newCap <<= 1;
                }
                reserve(int32_t(newCap));
            }
        }
    };

    template<typename T>
    inline bool HeapVector<T>::isDefault(const T& value) {
        return !value;
    }

    template<typename T>
    inline void HeapVector<T>::makeDefault(T& value) {
        value = {};
    }
}