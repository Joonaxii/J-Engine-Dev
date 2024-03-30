#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <JEngine/Core.h>
#include <JEngine/Core/Assert.h>
#include <JEngine/Utility/DataUtilities.h>
#include <spdlog/fmt/bundled/core.h>

namespace JEngine {
    template<typename T> 
    class ConstSpan;
    template<typename T>
    class Span {
    public:
        constexpr Span() : _ptr(nullptr), _length() {}
        Span(void* ptr, size_t length) : _ptr(reinterpret_cast<T*>(ptr)), _length(length) {}
        Span(T* ptr, size_t length) : _ptr(ptr), _length(length) {}
    
        Span(T* val);

        template<typename U>
        Span(U& val);

        size_t length() const { return _length; }

        const T& operator[](size_t index) const { return *(_ptr + index); }
        T& operator[](size_t index) { return *(_ptr + index); }

        T* get() { return _ptr; }
        const T* get() const { return _ptr; }

        Span<T> slice(int64_t index, size_t length) const {
            JE_CORE_ASSERT(_length - index >= length, "Invalid index or length!");
            return Span<T>(_ptr + index, length);
        }

        Span<T> slice(int64_t index) const {
            return slice(index, _length - index);
        }

        void copyTo(Span<T>& other) const {
            JE_CORE_ASSERT(_length <= other._length, "Target Span is too small!");
            memcpy(other._ptr, _ptr, _length * sizeof(T));
        }

        void copyTo(void* other) const {
            memcpy(other, _ptr, _length * sizeof(T));
        }

        size_t indexOf(const T& find) const {
            for (size_t i = 0; i < _length; i++) {
                if (_ptr[i] == find) { return i; }
            }
            return SIZE_MAX;
        }

        size_t indexOfLast(const T& find) const {
            if (_length < 1) { return SIZE_MAX; }

            for (size_t i = _length - 1; i >= 0; i--) {
                if (_ptr[i] == find) { return i; }
            }
            return SIZE_MAX;
        }

        size_t indexOfAny(T const* find, size_t count) const {
            for (size_t i = 0; i < _length; i++) {
                const auto& val = _ptr[i];
                for (size_t k = 0; k < count; k++) {
                    if (val == find[k]) { return i; }
                }
            }
            return SIZE_MAX;
        }

        size_t indexOfAnyLast(T const* find, size_t count) const {
            if (_length < 1) { return SIZE_MAX; }

            for (size_t i = _length - 1; i >= 0; i--) {
                const auto& val = _ptr[i];
                for (size_t k = 0; k < count; k++) {
                    if (val == find[k]) { return i; }
                }
            }
            return SIZE_MAX;
        }

        size_t split(Span<T>* buffer, size_t maxCount, const T& value) const {
            size_t start = 0;
            size_t len = 0;
            size_t split = 0;
            for (size_t i = 0; i < _length; i++) {
                const T& cur = _ptr[i];
                if (cur == value) {
                    if (len > 0) {
                        buffer[split++] = Span<T>(_ptr + start, len);
                    }
                    len = 0;
                    start = i + 1;
                    if (split >= maxCount) { break; }
                    continue;
                }
                len++;
            }
            if (len > 0) {
                buffer[split++] = Span<T>(_ptr + start, len);
            }
            return split;
        }

        size_t split(ConstSpan<T>* buffer, size_t maxCount, const T& value) const {
            size_t start = 0;
            size_t len = 0;
            size_t split = 0;
            for (size_t i = 0; i < _length; i++) {
                const T& cur = _ptr[i];
                if (cur == value) {
                    if (len > 0) {
                        buffer[split++] = ConstSpan<T>(_ptr + start, len);
                    }
                    len = 0;
                    start = i + 1;
                    if (split >= maxCount) { break; }
                    continue;
                }
                len++;
            }
            if (len > 0) {
                buffer[split++] = ConstSpan<T>(_ptr + start, len);
            }
            return split;
        }

        size_t split(std::vector<Span<T>>& buffer, const T& value) const {
            size_t start = 0;
            size_t len = 0;
            size_t split = 0;
            for (size_t i = 0; i < _length; i++) {
                const T& cur = _ptr[i];
                if (cur == value) {
                    if (len > 0) {
                        buffer.push_back(Span<T>(_ptr + start, len));
                        split++;
                    }
                    len = 0;
                    start = i + 1;
                    continue;
                }
                len++;
            }
            if (len > 0) {
                buffer.push_back(Span<T>(_ptr + start, len));
                split++;
            }
            return split;
        }
        size_t split(std::vector<ConstSpan<T>>& buffer, const T& value) const {
            size_t start = 0;
            size_t len = 0;
            size_t split = 0;
            for (size_t i = 0; i < _length; i++) {
                const T& cur = _ptr[i];
                if (cur == value) {
                    if (len > 0) {
                        buffer.push_back(ConstSpan<T>(_ptr + start, len));
                        split++;
                    }
                    len = 0;
                    start = i + 1;
                    continue;
                }
                len++;
            }
            if (len > 0) {
                buffer.push_back(ConstSpan<T>(_ptr + start, len));
                split++;
            }
            return split;
        }

        size_t getOccurenceCount(const T& v) const {
            size_t count = 0;
            for (size_t i = 0; i < _length; i++) {
                if (_ptr[i] == v) { count++; }
            }
            return count;
        }

        Span<T> trim() const;
        Span<T> trim(const T& value) const;

        bool equals(const ConstSpan<T>& other) const;
        bool equals(const void* data, size_t size) const {
            if (size != (_length * sizeof(T))) { return false; }

            const T* asT = reinterpret_cast<const T*>(data);
            for (size_t i = 0; i < _length; i++) {
                if (asT[i] != _ptr[i]) { return false; }
            }
            return true;
        }

        bool equals(const Span<T>& other) const {
            if(_length != other._length) { return false; }
            for (size_t i = 0; i < _length; i++) {
                if (other._ptr[i] != _ptr[i]) { return false; }
            }
            return true;
        }

        template<typename U>
        U read(const size_t start, const bool isBigEndian = false) const {
            if ((_length - start) < 1 || !_ptr) { return U(); }
            JE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");

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
            JE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");
            value = *reinterpret_cast<U*>(_ptr + start);
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        U read(const bool isBigEndian = false) const {
            if (_length < 1 || !_ptr) { return U(); }
            JE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");

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
            JE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");
            value = *reinterpret_cast<U*>(_ptr);
            
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        void writeAt(const size_t start, const U& value, const bool isBigEndian = false) const {
            JE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span isn't large enough!");
            memcpy(reinterpret_cast<uint8_t*>(_ptr) + start, &value, sizeof(U));

            if (isBigEndian) {
                Data::reverseEndianess(reinterpret_cast<uint8_t*>(_ptr) + start, sizeof(U), 1);
            }
        }  
        
        template<typename U>
        void writeValuesAt(const size_t start, const U& value, size_t count, const bool isBigEndian = false) const {
            JE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U) * count, "Span isn't large enough!");
            std::fill_n(reinterpret_cast<U*>(reinterpret_cast<uint8_t*>(_ptr) + start), count, value);

            if (isBigEndian) {
                Data::reverseEndianess(reinterpret_cast<uint8_t*>(_ptr) + start, sizeof(U), count);
            }
        }

        template<typename U>
        void writeAt(const size_t start, const U* value, const size_t count, const bool isBigEndian = false) const {
            JE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U) * count, "Span isn't large enough!");
            memcpy(reinterpret_cast<uint8_t*>(_ptr) + start, value, sizeof(U) * count);

            if (isBigEndian) {
                Data::reverseEndianess(reinterpret_cast<uint8_t*>(_ptr) + start, sizeof(U), count);
            }
        }

        template<typename U>
        void write(const U& value, const bool isBigEndian = false) const {
            JE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span isn't large enough!");
            memcpy(_ptr, &value, sizeof(U));

            if (isBigEndian) {
                Data::reverseEndianess(_ptr, sizeof(U), 1);
            }
        }

        template<typename U>
        void write(const U& value, size_t count, const bool isBigEndian = false) const {
            JE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U) * count, "Span isn't large enough!");
            std::fill_n(reinterpret_cast<U*>(_ptr), count, value);

            if (isBigEndian) {
                Data::reverseEndianess(_ptr, sizeof(U), count);
            }
        }

        template<typename U>
        void write(const U* value, const size_t count, const bool isBigEndian = false) const {
            JE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U) * count, "Span isn't large enough!");
            memcpy(_ptr, value, sizeof(U) * count);

            if (isBigEndian) {
                Data::reverseEndianess(_ptr, sizeof(U), count);
            }
        }

        template<typename U>
        Span<U> castTo() const {
            return Span<U>(_ptr, (_length * sizeof(T) / sizeof(U)));
        }

        template<typename U>
        ConstSpan<U> castToConst() const;

    private:
        T* _ptr;
        size_t _length;
    };

    template<typename T>
    class ConstSpan {
    public:
        constexpr ConstSpan() : _ptr(nullptr), _length() {}
        ConstSpan(const void* ptr, size_t length) : _ptr(reinterpret_cast<const T*>(ptr)), _length(length) {}
        constexpr ConstSpan(const T* ptr, size_t length) : _ptr(ptr), _length(length) {}
 
        ConstSpan(Span<T> input) : _ptr(input.get()), _length(input.length()) {}
        ConstSpan(const T* input);
        
        template<typename U>
        ConstSpan(const U& input);

        size_t length() const { return _length; }

        const T& operator[](size_t index) const { return *(_ptr + index); }
        const T* get() const { return _ptr; }

        size_t getOccurenceCount(const T& v) const {
            size_t count = 0;
            for (size_t i = 0; i < _length; i++) {
                if (_ptr[i] == v) { count++; }
            }
            return count;
        }

        ConstSpan<T> slice(int64_t index, size_t length) const {
            JE_CORE_ASSERT(_length - index >= length, "Invalid index or length!");
            return ConstSpan<T>(_ptr + index, length);
        }

        ConstSpan<T> slice(int64_t index) const {
            return slice(index, _length - index);
        }

        void copyTo(Span<T>& other) const {
            JE_CORE_ASSERT(_length <= other._length, "Target Span is too small!");
            memcpy(other._ptr, _ptr, _length * sizeof(T));
        }

        void copyTo(void* other) const {
            memcpy(other, _ptr, _length * sizeof(T));
        }

        size_t indexOf(const T& find) const {
            for (size_t i = 0; i < _length; i++) {
                if (_ptr[i] == find) { return int32_t(i); }
            }
            return SIZE_MAX;
        }

        size_t indexOfLast(const T& find) const {
            if (_length < 1) { return SIZE_MAX; }

            for (size_t i = _length - 1; i >= 0; i--) {
                if (_ptr[i] == find) { return int32_t(i); }
            }
            return SIZE_MAX;
        }

        size_t indexNotOf(const T& find) const {
            for (size_t i = 0; i < _length; i++) {
                if (_ptr[i] != find) { return int32_t(i); }
            }
            return SIZE_MAX;
        }

        size_t indexNotOfLast(const T& find) const {
            if (_length < 1) { return SIZE_MAX; }

            for (size_t i = _length - 1; i >= 0; i--) {
                if (_ptr[i] != find) { return int32_t(i); }
            }
            return SIZE_MAX;
        }


        size_t indexOfAny(T const* find, size_t count) const {
            for (size_t i = 0; i < _length; i++) {
                const auto& val = _ptr[i];
                for (size_t k = 0; k < count; k++) {
                    if (val == find[k]) { return i; }
                }
            }
            return SIZE_MAX;
        }

        size_t indexOfAnyLast(T const* find, size_t count) const {
            if (_length < 1) { return SIZE_MAX; }

            for (size_t i = _length - 1; i >= 0; i--) {
                const auto& val = _ptr[i];
                for (size_t k = 0; k < count; k++) {
                    if (val == find[k]) { return i; }
                }
            }
            return SIZE_MAX;
        }

        bool equals(const Span<T>& other) const;
        bool equals(const void* data, size_t size) const {
            if (size != (_length * sizeof(T))) { return false; }

            const T* asT = reinterpret_cast<const T*>(data);
            for (size_t i = 0; i < _length; i++) {
                if (asT[i] != _ptr[i]) { return false; }
            }
            return true;
        }

        bool equals(const ConstSpan<T>& other) const {
            if (_length != other.length()) { return false; }

            for (size_t i = 0; i < _length; i++) {
                if (other._ptr[i] != _ptr[i]) { return false; }
            }
            return true;
        }

        ConstSpan<T> trim() const;
        ConstSpan<T> trim(const T& value) const;

        size_t split(ConstSpan<T>* buffer, size_t maxCount, const T& value) const {
            size_t start = 0;
            size_t len = 0;
            size_t split = 0;
            for (size_t i = 0; i < _length; i++) {
                const T& cur = _ptr[i];
                if (cur == value) {
                    if (len > 0) {
                        buffer[split++] = ConstSpan<T>(_ptr + start, len);
                    }
                    len = 0;
                    start = i + 1;
                    if (split >= maxCount) { break; }
                    continue;
                }
                len++;
            }
            if (len > 0) {
                buffer[split++] = ConstSpan<T>(_ptr + start, len);
            }
            return split;
        }

        size_t split(std::vector<ConstSpan<T>>& buffer, const T& value) const {
            size_t start = 0;
            size_t len   = 0;
            size_t split   = 0;
            for (size_t i = 0; i < _length; i++) {
                const T& cur = _ptr[i];
                if (cur == value) {
                    if(len > 0){
                        buffer.push_back(ConstSpan<T>(_ptr + start, len));
                        split++;
                    }
                    len = 0;
                    start = i + 1;
                    continue;
                }
                len++;
            }
            if (len > 0) {
                buffer.push_back(ConstSpan<T>(_ptr + start, len));
                split++;
            }
            return split;
        }

        template<typename U>
        U read(const size_t start, const bool isBigEndian = false) const {
            if ((_length - start) < 1 || !_ptr) { return U(); }
            JE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");

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
            JE_CORE_ASSERT((_length - start) * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");
            value = *reinterpret_cast<U*>(_ptr + start);
            if (isBigEndian) {
                Data::reverseEndianess(&value);
            }
            return value;
        }

        template<typename U>
        U read(const bool isBigEndian = false) const {
            if (_length < 1 || !_ptr) { return U(); }
            JE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");

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
            JE_CORE_ASSERT(_length * sizeof(T) >= sizeof(U), "Span doesn't contain enough data!");
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
}

template<typename T>
inline bool JEngine::Span<T>::equals(const JEngine::ConstSpan<T>& other) const {
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
inline bool JEngine::ConstSpan<T>::equals(const JEngine::Span<T>& other) const {
    if (_length != other.length()) { return false; }

    for (size_t i = 0; i < _length; i++) {
        if (other[i] != _ptr[i]) { return false; }
    }
    return true;
}

template<>
inline JEngine::ConstSpan<char>::ConstSpan(const char* input) : _ptr(input), _length(strlen(input)) { }

template<>
template<>
inline JEngine::ConstSpan<char>::ConstSpan(const std::string_view& input) : _ptr(input.data()), _length(input.length()) { }

template<>
template<>
inline JEngine::ConstSpan<char>::ConstSpan(const std::string& input) : _ptr(input.data()), _length(input.length()) { }

template<>
template<>
inline JEngine::Span<char>::Span(std::string& input) : _ptr(input.data()), _length(input.length()) { }

template<>
inline JEngine::Span<char>::Span(char* input) : _ptr(input), _length(strlen(input)) { }


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


template<>
inline JEngine::Span<char> JEngine::Span<char>::trim(const char& value) const {
    size_t startI = 0;
    size_t endI = _length;


    bool checkA = true;
    bool checkB = true;
    for (size_t i = 0, j = _length - 1; i < _length; i++, j--) {

        if (checkA) {
            if (_ptr[i] == value) {
                startI++;
            }
            else {
                checkA = false;
            }
        }

        if (checkB) {
            if (_ptr[j] == value) {
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
inline JEngine::ConstSpan<char> JEngine::ConstSpan<char>::trim(const char& value) const {
    size_t startI = 0;
    size_t endI = _length;


    bool checkA = true;
    bool checkB = true;
    for (size_t i = 0, j = _length - 1; i < _length; i++, j--) {

        if (checkA) {
            if (_ptr[i] == value) {
                startI++;
            }
            else {
                checkA = false;
            }
        }

        if (checkB) {
            if (_ptr[j] == value) {
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


template <>
struct fmt::formatter<JEngine::ConstSpan<char>> : formatter<string_view> {
    auto format(JEngine::ConstSpan<char> spn, format_context& ctx) const {
        return formatter<string_view>::format(string_view(spn.get(), spn.length()), ctx);
    }
};

template <>
struct fmt::formatter<JEngine::Span<char>> : formatter<string_view> {
    auto format(JEngine::Span<char> spn, format_context& ctx) const {
        return formatter<string_view>::format(string_view(spn.get(), spn.length()), ctx);
    }
};