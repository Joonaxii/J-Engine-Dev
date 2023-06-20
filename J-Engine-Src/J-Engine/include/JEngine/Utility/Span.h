#pragma once
#include <JEngine/Core/Log.h>
#include <JEngine/Core/Assert.h>
#include <JEngine/Utility/DataUtilities.h>

namespace JEngine {
    template<typename T> 
    class ConstSpan;
    template<typename T>
    class Span {
    public:
        Span() : _ptr(nullptr), _length() {}
        Span(void* ptr, size_t length) : _ptr(reinterpret_cast<T*>(ptr)), _length(length) {}
        Span(T* ptr, size_t length) : _ptr(ptr), _length(length) {}

        size_t length() const { return _length; }

        const T& operator[](size_t index) const { return *(_ptr + index); }
        T& operator[](size_t index) { return *(_ptr + index); }

        T* get() { return _ptr; }
        const T* get() const { return _ptr; }

        Span<T> slice(int64_t index, size_t length) const {
            JENGINE_CORE_ASSERT(_length - index >= length, "Invalid index or length!");
            return Span<T>(_ptr + index, length);
        }

        Span<T> slice(int64_t index) const {
            return slice(index, _length - index);
        }

        void copyTo(Span<T>& other) const {
            JENGINE_CORE_ASSERT(_length <= other._length, "Target Span is too small!");
            memcpy(other._ptr, _ptr, _length * sizeof(T));
        }

        void copyTo(void* other) const {
            memcpy(other, _ptr, _length * sizeof(T));
        }

        int32_t indexOf(const T& find) const {
            for (int32_t i = 0; i < _length; i++) {
                if (_ptr[i] == find) { return i; }
            }
            return -1;
        }

        int32_t indexOfLast(const T& find) const {
            for (int32_t i = _length - 1; i >= 0; i--) {
                if (_ptr[i] == find) { return i; }
            }
            return -1;
        }

        Span<T> trim() const;

        bool equals(const ConstSpan<T>& other);
        bool equals(const void* data, size_t size) {
            if (size != (_length * sizeof(T))) { return false; }

            const T* asT = reinterpret_cast<const T*>(data);
            for (size_t i = 0; i < _length; i++) {
                if (asT[i] != _ptr[i]) { return false; }
            }
            return true;
        }

        bool equals(const Span<T>& other) {
            if(_length != other._length) { return false; }
            for (size_t i = 0; i < _length; i++) {
                if (other._ptr[i] != _ptr[i]) { return false; }
            }
            return true;
        }

        template<typename U>
        U read(const size_t start, const bool isBigEndian = false) const {
            if ((_length - start) < 1 || !_ptr) { return U(); }
            JENGINE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");

            U value = *reinterpret_cast<U*>(_ptr + start);
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        U& read(const size_t start, U& value, const bool isBigEndian = false) const {
            if ((_length - start) < 1 || !_ptr) {
                value = 0;
                return value;
            }
            JENGINE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");
            value = *reinterpret_cast<U*>(_ptr + start);
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        U read(const bool isBigEndian = false) const {
            if (_length < 1 || !_ptr) { return U(); }
            JENGINE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");

            U value = *reinterpret_cast<U*>(_ptr);
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        U& read(U& value, const bool isBigEndian = false) const {
            if (_length < 1 || !_ptr)  {
                value = 0;
                return value;
            }
            JENGINE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");
            value = *reinterpret_cast<U*>(_ptr);
            
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        void writeAt(const size_t start, const U& value, const bool isBigEndian = false) const {
            JENGINE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span isn't large enough!");
            memcpy(reinterpret_cast<uint8_t*>(_ptr) + start, &value, sizeof(U));

            if (isBigEndian) {
                Data::reverseEndianess(reinterpret_cast<uint8_t*>(_ptr) + start, sizeof(U), 1);
            }
        }  
        
        template<typename U>
        void writeValuesAt(const size_t start, const U& value, size_t count, const bool isBigEndian = false) const {
            JENGINE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U) * count, "Span isn't large enough!");
            std::fill_n(reinterpret_cast<U*>(reinterpret_cast<uint8_t*>(_ptr) + start), count, value);

            if (isBigEndian) {
                Data::reverseEndianess(reinterpret_cast<uint8_t*>(_ptr) + start, sizeof(U), count);
            }
        }

        template<typename U>
        void writeAt(const size_t start, const U* value, const size_t count, const bool isBigEndian = false) const {
            JENGINE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U) * count, "Span isn't large enough!");
            memcpy(reinterpret_cast<uint8_t*>(_ptr) + start, value, sizeof(U) * count);

            if (isBigEndian) {
                Data::reverseEndianess(reinterpret_cast<uint8_t*>(_ptr) + start, sizeof(U), count);
            }
        }

        template<typename U>
        void write(const U& value, const bool isBigEndian = false) const {
            JENGINE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span isn't large enough!");
            memcpy(_ptr, &value, sizeof(U));

            if (isBigEndian) {
                Data::reverseEndianess(_ptr, sizeof(U), 1);
            }
        }

        template<typename U>
        void write(const U& value, size_t count, const bool isBigEndian = false) const {
            JENGINE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U) * count, "Span isn't large enough!");
            std::fill_n(reinterpret_cast<U*>(_ptr), count, value);

            if (isBigEndian) {
                Data::reverseEndianess(_ptr, sizeof(U), count);
            }
        }

        template<typename U>
        void write(const U* value, const size_t count, const bool isBigEndian = false) const {
            JENGINE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U) * count, "Span isn't large enough!");
            memcpy(_ptr, value, sizeof(U) * count);

            if (isBigEndian) {
                Data::reverseEndianess(_ptr, sizeof(U), count);
            }
        }

        template<typename U>
        Span<U> castTo() const {
            return Span<U>(_ptr, (_length * sizeof(T) / sizeof(U)));
        }

        operator ConstSpan<T>() const;

        template<typename U>
        ConstSpan<U> castToConst() const;

    private:
        T* _ptr;
        size_t _length;
    };

    template<typename T>
    class ConstSpan {
    public:
        ConstSpan() : _ptr(nullptr), _length() {}
        ConstSpan(const void* ptr, size_t length) : _ptr(reinterpret_cast<const T*>(ptr)), _length(length) {}
        ConstSpan(const T* ptr, size_t length) : _ptr(ptr), _length(length) {}

        size_t length() const { return _length; }

        const T& operator[](size_t index) const { return *(_ptr + index); }
        const T* get() const { return _ptr; }

        ConstSpan<T> slice(int64_t index, size_t length) const {
            JENGINE_CORE_ASSERT(_length - index >= length, "Invalid index or length!");
            return ConstSpan<T>(_ptr + index, length);
        }

        ConstSpan<T> slice(int64_t index) const {
            return slice(index, _length - index);
        }

        void copyTo(Span<T>& other) const {
            JENGINE_CORE_ASSERT(_length <= other._length, "Target Span is too small!");
            memcpy(other._ptr, _ptr, _length * sizeof(T));
        }

        void copyTo(void* other) const {
            memcpy(other, _ptr, _length * sizeof(T));
        }

        int32_t indexOf(const T& find) const {
            for (int32_t i = 0; i < _length; i++) {
                if (_ptr[i] == find) { return i; }
            }
            return -1;
        }

        int32_t indexOfLast(const T& find) const {
            for (int32_t i = _length - 1; i >= 0; i--) {
                if (_ptr[i] == find) { return i; }
            }
            return -1;
        }

        bool equals(const Span<T>& other);
        bool equals(const void* data, size_t size) {
            if (size != (_length * sizeof(T))) { return false; }

            const T* asT = reinterpret_cast<const T*>(data);
            for (size_t i = 0; i < _length; i++) {
                if (asT[i] != _ptr[i]) { return false; }
            }
            return true;
        }

        bool equals(const ConstSpan<T>& other) {
            if (_length != other.length()) { return false; }

            for (size_t i = 0; i < _length; i++) {
                if (other._ptr[i] != _ptr[i]) { return false; }
            }
            return true;
        }

        ConstSpan<T> trim() const;

        template<typename U>
        U read(const size_t start, const bool isBigEndian = false) const {
            if ((_length - start) < 1 || !_ptr) { return U(); }
            JENGINE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");

            U value = *reinterpret_cast<U*>(_ptr + start);
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        U& read(const size_t start, U& value, const bool isBigEndian = false) const {
            if ((_length - start) < 1 || !_ptr) {
                value = 0;
                return value;
            }
            JENGINE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");
            value = *reinterpret_cast<U*>(_ptr + start);
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        U read(const bool isBigEndian = false) const {
            if (_length < 1 || !_ptr) { return U(); }
            JENGINE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");

            U value = *reinterpret_cast<U*>(_ptr);
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        U& read(U& value, const bool isBigEndian = false) const {
            if (_length < 1 || !_ptr) {
                value = 0;
                return value;
            }
            JENGINE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");
            value = *reinterpret_cast<U*>(_ptr);

            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        ConstSpan<U> castTo() const {
            return ConstSpan<U>(_ptr, (_length * sizeof(T) / sizeof(U)));
        }

    private:
        const T* _ptr;
        size_t _length;
    };

    inline int32_t indexOf(Span<char> span, const char* string, size_t len) {
        if (len < 1) { return -1; }

        int32_t inRow = 0;
        int32_t index = -1;
        for (size_t i = 0; i < span.length(); i++) {
            if (span[i] == string[inRow]) {
                if (inRow == 0) {
                    index = int32_t(i);
                }
                inRow++;
                if (inRow >= len) { return index; }
            }
            else if (inRow > 0) {
                inRow = 0;
            }
        }
        return -1;
    }
    inline int32_t indexOf(ConstSpan<char> span, const char* string, size_t len) {
        if (len < 1) { return -1; }

        int32_t inRow = 0;
        int32_t index = -1;
        for (size_t i = 0; i < span.length(); i++) {
            if (span[i] == string[inRow]) {
                if (inRow == 0) {
                    index = int32_t(i);
                }
                inRow++;
                if (inRow >= len) { return index; }
            }
            else if (inRow > 0) {
                inRow = 0;
            }
        }
        return -1;
    }

    inline int32_t indexOf(Span<char> span, const char* string) {
        return indexOf(span, string, strlen(string));
    }
    inline int32_t indexOf(ConstSpan<char> span, const char* string) {
        return indexOf(span, string, strlen(string));
    }
    inline int32_t indexOf(Span<char> span, Span<char> string) {
        return indexOf(span, string.get(), string.length());
    }
    inline int32_t indexOf(ConstSpan<char> span, Span<char> string) {
        return indexOf(span, string.get(), string.length());
    }
    inline int32_t indexOf(Span<char> span, ConstSpan<char> string) {
        return indexOf(span, string.get(), string.length());
    }
    inline int32_t indexOf(ConstSpan<char> span, ConstSpan<char> string) {
        return indexOf(span, string.get(), string.length());
    }


    inline int32_t indexOfLast(Span<char> span, const char* string, size_t len) {
        if (len < 1) { return -1; }
        int32_t inRow = 0;
        int32_t index = -1;
        int32_t sLen = int32_t(span.length() - 1);
        for (int32_t i = sLen; i >= 0; i--) {
            if (span[i] == string[inRow]) {
                if (inRow == 0) {
                    index = i;
                }
                inRow++;
                if (inRow >= len) { return index; }
            }
            else if(inRow > 0) {
                inRow = 0;
            }
        }
        return -1;
    }
    inline int32_t indexOfLast(ConstSpan<char> span, const char* string, size_t len) {
        if (len < 1) { return -1; }
        int32_t inRow = 0;
        int32_t index = -1;
        int32_t sLen = int32_t(span.length() - 1);
        for (int32_t i = sLen; i >= 0; i--) {
            if (span[i] == string[inRow]) {
                if (inRow == 0) {
                    index = i;
                }
                inRow++;
                if (inRow >= len) { return index; }
            }
            else if (inRow > 0) {
                inRow = 0;
            }
        }
        return -1;
    }

    inline int32_t indexOfLast(Span<char> span, const char* string) {
        return indexOfLast(span, string, strlen(string));
    }
    inline int32_t indexOfLast(ConstSpan<char> span, const char* string) {
        return indexOfLast(span, string, strlen(string));
    }
    inline int32_t indexOfLast(Span<char> span, Span<char> string) {
        return indexOfLast(span, string.get(), string.length());
    }
    inline int32_t indexOfLast(ConstSpan<char> span, Span<char> string) {
        return indexOfLast(span, string.get(), string.length());
    }
    inline int32_t indexOfLast(Span<char> span, ConstSpan<char> string) {
        return indexOfLast(span, string.get(), string.length());
    }
    inline int32_t indexOfLast(ConstSpan<char> span, ConstSpan<char> string) {
        return indexOfLast(span, string.get(), string.length());
    }
}

template<typename T>
inline bool JEngine::Span<T>::equals(const JEngine::ConstSpan<T>& other) {
    if (_length != other.length()) { return false; }

    for (size_t i = 0; i < _length; i++) {
        if (other[i] != _ptr[i]) { return false; }
    }
    return true;
}

template<typename T>
template<typename U>
inline JEngine::ConstSpan<U> JEngine::Span<T>::castToConst() const {
    return JEngine::ConstSpan<U>(_ptr, (_length * sizeof(T) / sizeof(U)));
}

template<typename T>
inline bool JEngine::ConstSpan<T>::equals(const JEngine::Span<T>& other) {
    if (_length != other.length()) { return false; }

    for (size_t i = 0; i < _length; i++) {
        if (other[i] != _ptr[i]) { return false; }
    }
    return true;
}

template<typename T>
inline JEngine::Span<T>::operator JEngine::ConstSpan<T>() const {
    return JEngine::ConstSpan<T>(_ptr, _length);
}

template<>
inline JEngine::Span<char> JEngine::Span<char>::trim() const {
    size_t startI = 0;
    size_t endI = _length;


    bool checkA = true;
    bool checkB = true;
    for (size_t i = 0, j = _length-1; i < _length; i++, j--) {

        if (checkA) {
            if (isspace(_ptr[i])) {
                startI++;
            }
            else {
                checkA = false;
            }
        }

        if (checkB) {
            if (isspace(_ptr[j])) {
                endI--;
            }
            else {
                checkB = false;
            }
        }

        if (!checkA && !checkB) { break; }
    }

    if (startI >= endI) { return Span<char>(_ptr + startI, 0); }
    return Span<char>(_ptr + startI, endI - startI);
}

template<>
inline JEngine::ConstSpan<char> JEngine::ConstSpan<char>::trim() const {
    size_t startI = 0;
    size_t endI = _length;


    bool checkA = true;
    bool checkB = true;
    for (size_t i = 0, j = _length - 1; i < _length; i++, j--) {

        if (checkA) {
            if (isspace(_ptr[i])) {
                startI++;
            }
            else {
                checkA = false;
            }
        }

        if (checkB) {
            if (isspace(_ptr[j])) {
                endI--;
            }
            else {
                checkB = false;
            }
        }

        if (!checkA && !checkB) { break; }
    }

    if (startI >= endI) { return ConstSpan<char>(_ptr + startI, 0); }
    return ConstSpan<char>(_ptr + startI, endI - startI);
}