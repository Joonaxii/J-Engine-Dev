#pragma once
#include <cstdint>
#include <assert.h>
#include <typeinfo>
#include <string>
#include <iostream>
#include <JEngine/Utility/Span.h>
#include <JEngine/Helpers/StringConsts.h>

namespace JEngine {
    template<typename T, size_t size = sizeof(T)>
    class ConstSpan {
    public:
        static constexpr size_t SIZE = size;

        ConstSpan() : _ptr(nullptr), _len(0) {}
        ConstSpan(const T* ptr, const size_t len) : _ptr(ptr), _len(len) {}
        ConstSpan(const T* beg, const T* end) : _ptr(beg), _len(end - beg) {}
        ConstSpan(const Span<T>& span) : _ptr(span.begin()), _len(span.length()) {}

        template<typename iter>
        ConstSpan(const iter beg, const iter end);

        template<typename TIn>
        ConstSpan(const TIn value);

        const T& operator[](const size_t index) const {
            assert(_ptr && index >= 0 && index < _len);
            return _ptr[index];
        }

        const T* begin() const { return _ptr; }
        const T* end() const { return _ptr + _len; }

        const size_t length() const { return _len; }

        template<typename TOut>
        const TOut read() const {
            assert(sizeof(TOut) <= (_len * sizeof(T)) && "Target type is larger than data in Span");
            return *reinterpret_cast<TOut*>(_ptr);
        }

        template<typename TOut>
        const TOut read(const size_t offset) const {
            assert(sizeof(TOut) <= ((_len * sizeof(T)) - offset) && "Target type is larger than data in Span");
            return *reinterpret_cast<const TOut*>(reinterpret_cast<const char*>(_ptr) + offset);
        }

        const void set(const T* ptr, const size_t len) const {
            _len = len;
            _ptr = ptr;
        }

        const int32_t lastIndexOf(const T& input) const {
            assert(_ptr && "Internal pointer is null!");

            for (size_t i = _len - 1; i >= 0; i--) {
                if (_ptr[i] == input) { return int32_t(i); }
            }
            return -1;
        }

        const int32_t indexOf(const T& input) const {
            assert(_ptr && "Internal pointer is null!");

            for (size_t i = 0; i < _len; i++) {
                if (_ptr[i] == input) { return int32_t(i); }
            }
            return -1;
        }

        const int32_t lastIndexOfAny(const T* input, const size_t length) const {
            assert(_ptr && "Internal pointer is null!");

            auto end = input + length;
            auto selfEnd = this->end();
            for (size_t i = _len - 1; i >= 0; i--) {
                if (std::find(input, end, _ptr[i]) != selfEnd) { return int32_t(i); }
            }
            return -1;
        }

        const int32_t indexOfAny(const T* input, const size_t length) const {
            assert(_ptr && "Internal pointer is null!");

            auto end = input + length;
            auto selfEnd = this->end();
            for (size_t i = 0; i < _len; i++) {
                if (std::find(input, end, _ptr[i]) != selfEnd) { return int32_t(i); }
            }
            return -1;
        }

        void copyTo(Span<T> other) const {
            assert(other._len >= _len && "Target length too small!");
            memcpy(other._ptr, _ptr, _len * sizeof(T));
        }

        void copyTo(T* other) const {
            assert(other != nullptr && "Target is null!");
            memcpy(other, _ptr, _len * sizeof(T));
        }

        const ConstSpan<T> trim() const {
            static_assert("Not Implemented!");
        }

        const ConstSpan<T> trimStart() const {
            static_assert("Not Implemented!");
        }

        const ConstSpan<T> trimEnd() const {
            static_assert("Not Implemented!");
        }

        ConstSpan<T> slice(const size_t start) const {
            assert(start < _len&& start >= 0 && "Start position is invalid!");
            return ConstSpan<T>(_ptr + start, _len - start);
        }

        ConstSpan<T> slice(const size_t start, const size_t length) const {
            assert(start < _len&& start >= 0 && "Start position is invalid!");
            assert(length >= 0 && length <= (_len - start) && "Length is invalid!");
            return ConstSpan<T>(_ptr + start, length);
        }

        template<typename TOut, size_t sizeO = ConstSpan<TOut>::SIZE>
        ConstSpan<TOut> castTo() const {
            const size_t sizeSrc = _len * SIZE;
            assert(sizeSrc > 0 && "Source size must be more than 0!");

            const size_t sizeTrg = sizeSrc - (sizeSrc % sizeO);
            assert(sizeTrg > 0 && sizeO > 0 && "Target type too small for Span cast");
            return ConstSpan<TOut>(reinterpret_cast<TOut*>(_ptr), sizeTrg / sizeO);
        }

    private:
        const T* _ptr;
        const size_t _len;
    };

    template<>
    template<>
    inline ConstSpan<char>::ConstSpan(const std::string value) : _ptr(&value[0]), _len(value.length()) { }

    template<>
    template<>
    inline ConstSpan<char>::ConstSpan(const char* value) : _ptr(&value[0]), _len(strlen(value)) { }

    template<>
    template<>
    inline ConstSpan<wchar_t>::ConstSpan(const std::wstring value) : _ptr(&value[0]), _len(value.length()) { }

    template<>
    template<>
    inline ConstSpan<wchar_t>::ConstSpan(const wchar_t* value) : _ptr(&value[0]), _len(wcslen(value)) { }

    template<typename T, size_t size = sizeof(T)>
    inline std::ostream& operator<<(std::ostream& os, const ConstSpan<T, size>& span) {
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
    inline std::ostream& operator<<(std::ostream& os, const ConstSpan<char, sizeof(char)>& span) {
        const char* ptr = span.begin();
        for (size_t i = 0; i < span.length(); i++, ptr++) {
            os << *ptr;
        }
        return os;
    }

    template<typename T, size_t size>
    template<typename iter>
    inline ConstSpan<T, size>::ConstSpan(const iter beg, const iter end) : _ptr(beg), _len(end - beg) { }

    template<>
    inline const ConstSpan<char, sizeof(char)> ConstSpan<char, sizeof(char)>::trim() const {
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
        return ConstSpan<char, sizeof(char)>(this->begin() + stI, _len - stI - ndI);
    }

    template<>
    inline const ConstSpan<char, sizeof(char)> ConstSpan<char, sizeof(char)>::trimStart() const {
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
        return ConstSpan<char, sizeof(char)>(this->begin() + stI, _len - stI);
    }

    template<>
    inline const ConstSpan<char, sizeof(char)> ConstSpan<char, sizeof(char)>::trimEnd() const {
        auto nd = end() - 1;

        size_t stI = _len;

        auto endC = WHITE_CHARS + WHITE_CHAR_LEN;
        for (size_t i = 0; i < _len; i++, nd--, stI--) {
            auto find = std::find(WHITE_CHARS, endC, *nd);
            if (find == endC) {
                break;
            }
        }
        return ConstSpan<char, sizeof(char)>(this->begin(), stI);
    }
}