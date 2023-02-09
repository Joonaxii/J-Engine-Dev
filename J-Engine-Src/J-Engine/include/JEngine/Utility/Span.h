#pragma once
#include <cstdint>
#include <assert.h>
#include <typeinfo>
#include <string>
#include <iostream>
#include <JEngine/Helpers/StringConsts.h>

namespace JEngine {
    template<typename T, size_t size = sizeof(T)>
    class Span {
    public:
        static constexpr size_t SIZE = size;

        Span() : _ptr(nullptr), _len(0) {}
        Span(T* ptr, const size_t len) : _ptr(ptr), _len(len) {}
        Span(void* ptr, const size_t len) : _ptr(reinterpret_cast<T*>(ptr)), _len(len) {}

        Span(T* beg, const T* end) : _ptr(beg), _len(end - beg) {}
        Span(void* beg, const void* end) : _ptr(reinterpret_cast<T*>(beg)), _len(reinterpret_cast<T*>(end) - reinterpret_cast<T*>(beg)) {}

        template<typename TIn>
        Span(TIn& value);

        T& operator[](const size_t index) {
            assert(_ptr && index >= 0 && index < _len);
            return _ptr[index];
        }
        const T& operator[](const size_t index) const {
            assert(_ptr && index >= 0 && index < _len);
            return _ptr[index];
        }

        T* begin() { return _ptr; }
        T* end() { return _ptr + _len; }

        const T* begin() const { return _ptr; }
        const T* end() const { return _ptr + _len; }

        size_t length() const { return _len; }

        bool equals(const Span<T> other) const {
            for (size_t i = 0; i < _len; i++) {
                if (_ptr[i] != other._ptr[i]) { return false; }
            }
            return true;
        }

        bool equals(const T* other) const {
            for (size_t i = 0; i < _len; i++) {
                if (_ptr[i] != other[i]) { return false; }
            }
            return true;
        }

        void set(T* ptr, const size_t len) {
            _len = len;
            _ptr = ptr;
        }

        int32_t lastIndexOf(const T& input) const {
            assert(_ptr && "Internal pointer is null!");

            for (size_t i = _len - 1; i >= 0; i--) {
                if (_ptr[i] == input) { return int32_t(i); }
            }
            return -1;
        }

        int32_t indexOf(const T& input) const {
            assert(_ptr && "Internal pointer is null!");

            for (size_t i = 0; i < _len; i++) {
                if (_ptr[i] == input) { return int32_t(i); }
            }
            return -1;
        }

        int32_t indexOf(const Span<T>& span) const {
            assert(_ptr && "Internal pointer is null!");
            assert(span.length() <= _len && "Given sequence is longer than Span!");

            size_t inARow = 0;
            int32_t index = -1;
            for (size_t i = 0; i < _len; i++) {
                if (span._ptr[inARow] == _ptr[i]) {
                    if (inARow == 0) {
                        index = i;
                    }
                    inARow++;
                    if (inARow >= span.length()) { return index; }
                    continue;
                }
                inARow = 0;
            }
            return -1;
        }

        int32_t indexOf(const T* sequence) const {
            assert(_ptr && "Internal pointer is null!");
            assert(sequence && "Input sequence is null!");

            size_t length = 0;
            while (true) {
                if (sequence[length] == '\0') { break; }
                length++;
            }

            assert(length <= _len && "The given sequence is longer than Span!");

            size_t inARow = 0;
            int32_t index = -1;
            for (int32_t i = 0; i < _len; i++) {
                if (sequence[inARow] == _ptr[i]) {
                    if (inARow == 0) {
                        index = i;
                    }
                    inARow++;
                    if (inARow >= length) { return index; }
                    continue;
                }
                inARow = 0;
            }
            return -1;
        }

        int32_t lastIndexOfAny(const T* input, const size_t length) const {
            assert(_ptr && "Internal pointer is null!");

            auto end = input + length;
            auto selfEnd = this->end();
            for (size_t i = _len - 1; i >= 0; i--) {
                if (std::find(input, end, _ptr[i]) != selfEnd) { return int32_t(i); }
            }
            return -1;
        }

        int32_t indexOfAny(const T* input, const size_t length) const {
            assert(_ptr && "Internal pointer is null!");

            auto end = input + length;
            auto selfEnd = this->end();
            for (size_t i = 0; i < _len; i++) {
                if (std::find(input, end, _ptr[i]) != selfEnd) { return int32_t(i); }
            }
            return -1;
        }

        Span<T> trim() const {
            static_assert("Not Implemented!");
        }

        Span<T> trimStart() const {
            static_assert("Not Implemented!");
        }

        Span<T> trimEnd() const {
            static_assert("Not Implemented!");
        }


        void copyTo(Span<T> other) const {
            assert(other._len >= _len && "Target length too small!");
            memcpy(other._ptr, _ptr, _len * sizeof(T));
        }

        void copyTo(T* other) const {
            assert(other != nullptr && "Target is null!");
            memcpy(other, _ptr, _len * sizeof(T));
        }

        template<typename TIn>
        void write(const TIn& value, const bool bigEndian = false) {
            assert(sizeof(TIn) <= (_len * sizeof(T)) && "Input type is larger than data in Span");
            *reinterpret_cast<TIn*>(_ptr) = value;
        }

        template<typename TIn>
        void writeBigEndian(const TIn& value) {
            assert(sizeof(TIn) <= (_len * sizeof(TIn)) && "Input type is larger than data in Span");

            char* ptrBytes = reinterpret_cast<char*>(_ptr);
            const char* data = reinterpret_cast<const char*>(&value);
            for (size_t i = 0, j = sizeof(TIn) - 1; i < sizeof(TIn); i++) {
                ptrBytes[i] = data[j];
            }
        }

        template<typename TIn>
        void write(const size_t offset, const TIn& value) {
            assert(sizeof(TIn) <= ((_len * sizeof(TIn)) - offset) && "Input type is larger than data in Span");
            *reinterpret_cast<const TIn*>(ptrBytes) = value;
        }

        template<typename TIn>
        void writeBigEndian(const size_t offset, const TIn& value) {
            assert(sizeof(TIn) <= ((_len * sizeof(TIn)) - offset) && "Input type is larger than data in Span");

            char* ptrBytes = reinterpret_cast<char*>(_ptr) + offset;
            const char* data = reinterpret_cast<const char*>(&value);
            for (size_t i = 0, j = sizeof(TIn) - 1; i < sizeof(TIn); i++) {
                ptrBytes[i] = data[j];
            }
        }

        template<typename TOut>
        TOut read() const {
            assert(sizeof(TOut) <= (_len * sizeof(T)) && "Target type is larger than data in Span");
            return *reinterpret_cast<TOut*>(_ptr);
        }

        template<typename TOut>
        TOut readBigEndian() const {
            assert(sizeof(TOut) <= (_len * sizeof(TOut)) && "Target type is larger than data in Span");
            const char* data = reinterpret_cast<const char*>(_ptr);

            char* buffer = reinterpret_cast<char*>(alloca(sizeof(TOut)));
            memcpy(buffer, data, sizeof(TOut));

            for (size_t i = 0, j = sizeof(TOut) - 1; i < sizeof(TOut) >> 1; i++, j--) {
                std::swap(buffer[i], buffer[j]);
            }
            return *reinterpret_cast<const TOut*>(buffer);
        }

        template<typename TOut>
        TOut read(const size_t offset) const {
            assert(sizeof(TOut) <= ((_len * sizeof(T)) - offset) && "Target type is larger than data in Span");
            return *reinterpret_cast<const TOut*>(reinterpret_cast<const char*>(_ptr) + offset);
        }

        template<typename TOut>
        TOut readBigEndian(const size_t offset) const {
            assert(sizeof(TOut) <= ((_len * sizeof(TOut)) - offset) && "Target type is larger than data in Span");
            const char* data = reinterpret_cast<const char*>(_ptr) + offset;

            char* buffer = reinterpret_cast<char*>(alloca(sizeof(TOut)));
            memcpy(buffer, data, sizeof(TOut));

            for (size_t i = 0, j = sizeof(TOut) - 1; i < sizeof(TOut) >> 1; i++, j--) {
                std::swap(buffer[i], buffer[j]);
            }
            return *reinterpret_cast<const TOut*>(buffer);
        }

        Span<T> slice(const size_t start) const {
            assert(start < _len&& start >= 0 && "Start position is invalid!");
            return Span<T>(_ptr + start, _len - start);
        }

        Span<T> slice(const size_t start, const size_t length) const {
            assert(start < _len&& start >= 0 && "Start position is invalid!");
            assert(length >= 0 && length <= (_len - start) && "Length is invalid!");

            return Span<T>(_ptr + start, length);
        }

        template<typename TOut, size_t sizeO = Span<TOut>::SIZE>
        Span<TOut> castTo() {
            const size_t sizeSrc = _len * SIZE;
            assert(sizeSrc > 0 && "Source size must be more than 0!");

            const size_t sizeTrg = sizeSrc - (sizeSrc % sizeO);
            assert(sizeTrg > 0 && sizeO > 0 && "Target type too small for Span cast");
            return Span<TOut>(reinterpret_cast<TOut*>(_ptr), sizeTrg / sizeO);
        }

    private:
        T* _ptr;
        size_t _len;
    };

    template<>
    template<>
    inline Span<char>::Span(std::string& value) : _ptr(&value[0]), _len(value.length()) { }

    template<>
    template<>
    inline Span<char>::Span(char*& value) : _ptr(&value[0]), _len(strlen(value)) { }

    template<>
    template<>
    inline Span<wchar_t>::Span(std::wstring& value) : _ptr(&value[0]), _len(value.length()) { }

    template<>
    template<>
    inline Span<wchar_t>::Span(wchar_t*& value) : _ptr(&value[0]), _len(wcslen(value)) { }

    template<typename T, size_t size = sizeof(T)>
    inline std::ostream& operator<<(std::ostream& os, const Span<T, size>& span) {
        os << '[';
        auto ptr = span.begin();
        for (size_t i = 0; i < span.length(); i++) {
            os << ptr[i] << ", ";
            if (i < span.length() - 1) {
                os << i << ", ";
            }
            else {
                os << i;
            }
        }
        os << ']';
        return os;
    }

    template<>
    inline std::ostream& operator<<(std::ostream& os, const Span<char, sizeof(char)>& span) {
        const char* ptr = span.begin();
        for (size_t i = 0; i < span.length(); i++, ptr++) {
            os << *ptr;
        }
        return os;
    }

    template<>
    inline Span<char, sizeof(char)> Span<char, sizeof(char)>::trim() const {
        auto st = begin();
        auto nd = end() - 1;

        size_t stI = 0;
        size_t ndI = 0;

        uint8_t check = 0x3;
        auto endC = WHITE_CHARS + WHITE_CHAR_LEN;
        for (size_t i = 0; i < _len && check; i++, st++, nd--) {
            if (check & 0x1) {
                auto find = std::find(WHITE_CHARS, endC, *st);
                if (find == endC) {
                    stI = i;
                    check &= 0x2;
                }
            }

            if (check & 0x2) {
                auto find = std::find(WHITE_CHARS, endC, *nd);
                if (find == endC) {
                    ndI = i;
                    check &= 0x1;
                }
            }
        }
        return Span<char, sizeof(char)>(_ptr + stI, _len - stI - ndI);
    }

    template<>
    inline Span<char, sizeof(char)> Span<char, sizeof(char)>::trimStart() const {
        auto st = begin();

        size_t stI = 0;

        auto endC = WHITE_CHARS + WHITE_CHAR_LEN;
        for (size_t i = 0; i < _len; i++, st++) {
            auto find = std::find(WHITE_CHARS, endC, *st);
            if (find == endC) {
                stI = i;
                break;
            }
        }
        return Span<char, sizeof(char)>(_ptr + stI, _len - stI);
    }

    template<>
    inline Span<char, sizeof(char)> Span<char, sizeof(char)>::trimEnd() const {
        auto nd = end() - 1;

        size_t stI = _len;

        auto endC = WHITE_CHARS + WHITE_CHAR_LEN;
        for (size_t i = 0; i < _len; i++, nd--, stI--) {
            auto find = std::find(WHITE_CHARS, endC, *nd);
            if (find == endC) {
                break;
            }
        }
        return Span<char, sizeof(char)>(_ptr, stI);
    }
}