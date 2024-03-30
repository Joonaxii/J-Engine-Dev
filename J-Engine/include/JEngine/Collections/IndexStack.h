#pragma once
#include <cstdint>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <new>
#include <JEngine/Math/Math.h>

namespace JEngine {
    namespace detail {
        struct Index {
            uint64_t index{};
            int32_t bit{};

            constexpr Index() : index(0), bit(0) {}
            constexpr Index(uint64_t index, int32_t bit) : index(index), bit(bit) {}

            constexpr bool operator==(const Index& other) const {
                return index == other.index && bit == other.index;
            }

            constexpr bool operator!=(const Index& other) const {
                return index != other.index || bit != other.index;
            }
        };

        struct IDXMarkType {
            enum : uint8_t {
                MARK_SUCCESS = 0x0,
                MARK_ALREADY_MARKED = 0x1,
                MARK_OUT_OF_RANGE = 0x2,
                MARK_INDEX_INVALID = 0x3,
            };

            uint8_t result{};
            Index index{};

            constexpr IDXMarkType() : result(0), index() {}
            constexpr IDXMarkType(uint8_t result, const Index& index) : result(result), index(index){}
        };

        static constexpr float INDEX_MAX_EXPANSION = 1.6f;
        static constexpr Index INVALID_INDEX = Index(0xFFFFFFFFU, -1);

        inline constexpr uint32_t getExpandedSize(uint32_t current) {
            return current + std::max(uint32_t(INDEX_MAX_EXPANSION * current), 1U);
        }

        inline constexpr Index extractIndex(uint64_t index) {
            const uint64_t lInd = index >> 6;
            return Index(lInd, int32_t(index - (lInd << 6)));
        }

        inline constexpr uint64_t to1DIndex(const Index& index) {
            return (index.index << 6) + index.bit;
        }
    }

    class IndexStack {
    public:
        IndexStack() : _availMask{nullptr}, _capacity(0) {}
        IndexStack(uint64_t initCap) : IndexStack() {
            reserve(initCap);
        }

        ~IndexStack() {
            release();
        }

        detail::Index getIfValid(uint64_t index) const {
            detail::Index ret = detail::extractIndex(index);
            if (ret.index >= _capacity) { return detail::INVALID_INDEX; }

            if (_availMask[ret.index] & (1ULL << ret.bit)) {
                return ret;
            }
            return detail::INVALID_INDEX;
        }

        uint64_t popNextFree(detail::Index* outIdx = nullptr, bool allowReserve = true) {
            auto ret = findNextFree();

            if (outIdx) { *outIdx = ret; }
            if (ret == detail::INVALID_INDEX) {
                if(!allowReserve) { return detail::INVALID_INDEX.index; }
                uint64_t oldCap = _capacity;
                if (reserve(detail::getExpandedSize(_capacity))) {
                    ret = detail::extractIndex(oldCap);
                    if (ret == detail::INVALID_INDEX) { return detail::INVALID_INDEX.index; }
                }
            }
            _availMask[ret.index] |= (1ULL << ret.bit);
            return detail::to1DIndex(ret);
        }

        detail::IDXMarkType markAsFree_Internal(uint64_t index) {
            if (index == detail::INVALID_INDEX.index) { return {detail::IDXMarkType::MARK_INDEX_INVALID, detail::INVALID_INDEX}; }

            auto ind = detail::extractIndex(index);
            if (ind.index >= _capacity) {
                return { detail::IDXMarkType::MARK_OUT_OF_RANGE, ind };
            }
            uint64_t mask = (1ULL << ind.bit);
            if (_availMask[ind.index] & mask) {
                _availMask[ind.index] &= ~mask;
                return { detail::IDXMarkType::MARK_SUCCESS, ind };
            }
            return { detail::IDXMarkType::MARK_ALREADY_MARKED, ind };
        }

        bool markAsFree(uint64_t index) {
            return markAsFree_Internal(index).result == detail::IDXMarkType::MARK_SUCCESS;
        }

        void release() {
            if (_availMask) {
                free(_availMask);
                _availMask = nullptr;
            }
            _capacity = 0;
        }

        void clear(bool fullClear = false) {
            if (fullClear) {
                release();
                return;
            }

            if (_capacity > 0 && _availMask) {
                memset(_availMask, 0, _capacity * sizeof(uint64_t));
            }
        }

        uint64_t getChunkMask(uint32_t ch) const { return _availMask[ch]; }

    private:
        uint64_t _capacity{};
        uint64_t* _availMask{};

        bool reserve(uint32_t count) {
            if (_availMask == nullptr) {
                _availMask = reinterpret_cast<uint64_t*>(malloc(count * sizeof(uint64_t)));
                _capacity = count;
                return _availMask != nullptr;
            }
            if (_capacity >= count) { return true; }

            uint64_t* reall = reinterpret_cast<uint64_t*>(realloc(_availMask, count * sizeof(uint64_t)));
            if (reall) {
                memset(reall + _capacity, 0, size_t(count) - _capacity);
                _availMask = reall;
                _capacity = count;
                return true;
            }
            return false;
        }

        detail::Index findNextFree() const {
            if (!_availMask) { return detail::INVALID_INDEX; }
            for (uint64_t i = 0; i < _capacity; i++) {
                uint64_t mask = ~_availMask[i];
                if (mask != 0x00) {
                    return detail::Index(i, Math::findFirstLSB(mask));
                }
            }
            return detail::INVALID_INDEX;
        }
    };

    template<uint64_t size>
    class FixedIndexStack {
    public:
        FixedIndexStack() : _availMask{ nullptr }, _capacity(0) {}
        FixedIndexStack(uint64_t initCap) : IndexStack() {
            reserve(initCap);
        }

        ~FixedIndexStack() {
            release();
        }

        detail::Index getIfValid(uint64_t index) const {
            detail::Index ret = detail::extractIndex(index);
            if (ret.index >= _capacity) { return detail::INVALID_INDEX; }

            if (_availMask[ret.index] & (1ULL << ret.bit)) {
                return ret;
            }
            return detail::INVALID_INDEX;
        }

        uint32_t popNextFree(detail::Index* outIdx = nullptr, bool allowReserve = true) {
            auto ret = findNextFree();

            if (outIdx) { *outIdx = ret; }
            if (ret == detail::INVALID_INDEX) {
                if (!allowReserve) { return detail::INVALID_INDEX.index; }
                uint32_t oldCap = _capacity;
                if (reserve(detail::getExpandedSize(_capacity))) {
                    ret = detail::extractIndex(oldCap);
                    if (ret == detail::INVALID_INDEX) { return detail::INVALID_INDEX.index; }
                }
            }
            _availMask[ret.index] |= (1ULL << ret.bit);
            return detail::to1DIndex(ret);
        }

        detail::IDXMarkType markAsFree_Internal(uint64_t index) {
            if (index == detail::INVALID_INDEX.index) { return { detail::IDXMarkType::MARK_INDEX_INVALID, detail::INVALID_INDEX }; }

            auto ind = detail::extractIndex(index);
            if (ind.index >= _capacity) {
                return { detail::IDXMarkType::MARK_OUT_OF_RANGE, ind };
            }
            uint64_t mask = (1ULL << ind.bit);
            if (_availMask[ind.index] & mask) {
                _availMask[ind.index] &= ~mask;
                return { detail::IDXMarkType::MARK_SUCCESS, ind };
            }
            return { detail::IDXMarkType::MARK_ALREADY_MARKED, ind };
        }

        bool markAsFree(uint64_t index) {
            return markAsFree_Internal(index).result == detail::IDXMarkType::MARK_SUCCESS;
        }

        void clear(bool fullClear = false) {
            if (fullClear) {
                release();
                return;
            }
            memset(_availMask, 0, BUF_CAPACITY * sizeof(uint64_t));
        }

        uint64_t getChunkMask(uint64_t ch) const { return _availMask[ch]; }

    private:
        static constexpr uint64_t BUF_CAPACITY = Math::nextDivByPowOf2<uint64_t, 64>(size) >> 6;
        uint64_t _availMask[BUF_CAPACITY]{};

        detail::Index findNextFree() const {
            for (uint64_t i = 0; i < BUF_CAPACITY; i++) {
                uint64_t mask = ~_availMask[i];
                if (mask != 0x00) {
                    return detail::Index(i, Math::findFirstLSB(mask));
                }
            }
            return detail::INVALID_INDEX;
        }
    };

    template<typename T>
    class ChunkedLUT {
    private:
        static constexpr size_t CHUNK_SIZE = 64;
        struct Chunk {
        public:
            T items[CHUNK_SIZE]{};

            T& operator[](size_t i) {
                return items[i];
            }  
            
            const T& operator[](size_t i) const {
                return items[i];
            }

            void destruct(uint64_t mask) {
                for (size_t i = 0, j = 1; i < CHUNK_SIZE; i++, j <<= 1) {
                    if (j & mask) {
                        items[i].~T();
                    }
                }
            }

        private:
            Chunk() = delete;
        };
    public:
        ChunkedLUT() : _chunks{}, _indexStack{} {}
        ~ChunkedLUT() {
            clear(true);
        }

        bool isIndexUsed(uint32_t uuid) const {
            return _indexStack.getIfValid(uuid) != detail::INVALID_INDEX;
        }

        T* getAt(uint32_t index) const {
            auto ret = _indexStack.getIfValid(index);
            if (ret == detail::INVALID_INDEX || ret.index >= _chunks.size()) { return nullptr; }
            auto ch = _chunks[ret.index];
            return ch ? &ch->items[ret.bit] : nullptr;
        }

        uint32_t tryReserve(uint32_t index, T** valueOut = nullptr) {
            detail::Index idx = _indexStack.getIfValid(index);
            if (idx == detail::INVALID_INDEX) { return detail::INVALID_INDEX.index; }

            auto ret = getInstance(idx.index);
            if (ret) {
                auto val = &ret->items[idx.bit];
                val = new (val) T();

                if (valueOut) {
                    *valueOut = val;
                }
                return index;
            }
            return detail::INVALID_INDEX.index;
        }

        uint32_t popNext(T** valueOut = nullptr) {
            detail::Index idx{};
            uint32_t index = _indexStack.popNextFree(&idx);
            if (idx == detail::INVALID_INDEX) { return detail::INVALID_INDEX.index; }

            auto ret = getInstance(index);
            if (ret) {
                T* val = &ret->items[idx.bit];
                val = new (val) T;

                if (valueOut) {
                    *valueOut = val;
                }
                return index;
            }
            return detail::INVALID_INDEX.index;
        }
        bool markFree(uint32_t index) {
            if (_indexStack.markAsFree(index)) {
                auto idx = detail::extractIndex(index);
                (*(_chunks[idx.index]))[idx.bit].~T();
                return true;
            }
            return false;
        }

        void clear(bool fullClear = false) {
            uint32_t chI = 0;
            for (Chunk* ch : _chunks) {
                if (ch) {
                    ch->destruct(_indexStack.getChunkMask(chI));
                    if (fullClear) {
                        free(ch);
                    }
                }
                chI++;
            }
            if (fullClear) { _chunks.clear(); }
            _indexStack.clear(fullClear);
        }

    private:
        std::vector<Chunk*> _chunks{};
        IndexStack _indexStack;

        Chunk* getInstance(uint32_t index) {
            if (_chunks.size() <= index) {
                _chunks.resize(index + 1);
            }
            auto& ptr = _chunks[index];
            if (!ptr) { ptr = reinterpret_cast<Chunk*>(malloc(sizeof(Chunk))); }
            return ptr;
        }
    };

    template<typename T>
    class IndexedLUT {
    public:
        IndexedLUT() : _capacity(0), _count(0), _items{nullptr} {}

        T& operator[](uint64_t i) {
            return _items[i];
        }
        const T& operator[](uint64_t i) const {
            return _items[i];
        }

        uint64_t size() const { return _count; }

        T* getAt(uint64_t i) const {
            if (i >= _count) { return nullptr; }
            auto idx = _indexStack.getIfValid(i);
            if (idx == detail::INVALID_INDEX) { return nullptr; }
            return &_items[i];
        }

        uint64_t getNext(T** outValue = nullptr) {
            uint64_t ind = _indexStack.popNextFree(nullptr, false);
            if (ind == detail::INVALID_INDEX.index) { return detail::INVALID_INDEX.index; }

            if (ind >= _capacity) {
                reserve(detail::getExpandedSize(ind + 1));
            }

            T* val = &_items[ind];
            val = new (val) T();
            if (outValue) {
                *outValue = val;
            }
            return ind;
        }

        bool markFree(uint64_t index) {
            if (_indexStack.markAsFree(index)) {
                if (!_items || _count >= index) { return true; }
                _items[index].~T();
                return true;
            }
            return false;
        }

        void clear(bool fullClear) {
            if (fullClear) {
                release();
                return;
            }

            if (_items) {
                for (uint64_t i = 0; i < _count; i++) {
                    if (_indexStack.getChunkMask(i >> 6) & (1ULL << (i & 63))) {
                        _items[i].~T();
                    }
                }
            }
            _indexStack.clear(false);
            _count = 0;
        }

    private:
        uint64_t _count;
        uint64_t _capacity;
        T* _items;
        IndexStack _indexStack;

        bool reserve(uint64_t count) {
            if (!_items) {
                _items = reinterpret_cast<T*>(malloc(sizeof(T) * count));
                if (_items) {
                    _capacity = count;
                    return true;
                }
                return false;
            }

            if (_capacity >= count) { return true; }
            T* reall = reinterpret_cast<T*>(realloc(_items, sizeof(T) * count));
            if (reall) {
                _capacity = count;
                _items = reall;
                return true;
            }
            return false;
        }

        void release() {
            if (_items) {
                for (uint64_t i = 0; i < _count; i++) {
                    if (_indexStack.getChunkMask(i >> 6) & (1ULL << (i & 63))) {
                        _items[i].~T();
                    }
                }
            }
            _indexStack.release();
            if (_items) {
                free(_items);
            }
            _capacity = 0;
            _count = 0;
        }
    };

    template<typename T, uint64_t size>
    class FixedLUT {
    public:
        FixedLUT() : _indexStack(size) {}
        ~FixedLUT() {
            clear(true);
        }

        T& operator[](uint64_t i) {
            return _items[i];
        }
        const T& operator[](uint64_t i) const {
            return _items[i];
        }

        T* getAt(uint64_t i) {
            if (i >= size) { return nullptr; }
            auto idx = _indexStack.getIfValid(i);
            if (idx == detail::INVALID_INDEX) { return nullptr; }
            return &_items[i];
        }

        const T* getAt(uint64_t i) const {
            if (i >= size) { return nullptr; }
            auto idx = _indexStack.getIfValid(i);
            if (idx == detail::INVALID_INDEX) { return nullptr; }
            return &_items[i];
        }

        uint64_t getNext(T** outValue = nullptr) {
            uint64_t ind = _indexStack.popNextFree(nullptr, false);
            if (ind == detail::INVALID_INDEX.index) { return detail::INVALID_INDEX.index; }

            T* val = &_items[ind];
            val = new (val) T();
            if (outValue) {
                *outValue = val;
            }
            return ind;
        }

        uint64_t setNext(const T& value) {
            uint64_t ind = _indexStack.popNextFree(nullptr, false);
            if (ind == detail::INVALID_INDEX.index) { return detail::INVALID_INDEX.index; }
            new(_items + ind) T(value);
            return ind;
        }

        bool markFree(uint64_t index) {
            if (_indexStack.markAsFree(index)) {
                _items[index].~T();
                return true;
            }
            return false;
        }

        void clear(bool fullClear) {
            uint64_t chI = 0;
            for (uint64_t i = 0; i < size; i += 64) {
                uint64_t chMask = _indexStack.getChunkMask(chI++);
                uint64_t len = Math::min(size - i, 64U);
                for (uint64_t j = 0, k = 1; j < len; j++, k <<= 1) {
                    if (chMask & k) {
                        _items[i + j].~T();
                    }
                }
            }
            _indexStack.clear(fullClear);
        }

    private:
        T _items[size];
        IndexStack _indexStack;
    };
}