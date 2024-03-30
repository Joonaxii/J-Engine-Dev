#include <JEngine/Core/String.h>
#include <JEngine/Core/Assert.h>
#include <JEngine/Math/Math.h>

namespace JEngine {
    String::String(size_t capacity) : String() {
        resize(capacity);
    }

    String::String(std::string_view view) : String() {
        resize(view.length());
        JE_COPY(_buffer, view.data(), view.length());
        _length = view.length();
    }

    String::String(const String& other) : String() {
        resize(other._length);
        JE_COPY(_buffer, other._buffer, other._length);
        _length = other._length;
    }

    String::String(String&& other) noexcept : 
        _capacity(std::move(other._capacity)),
        _length(std::move(other._length)),
        _buffer(std::exchange(other._buffer,  nullptr)){ }

    String::~String() {
        release();
    }

    String& String::operator=(std::string_view other) {
        resize(other.length());
        JE_COPY(_buffer, other.data(), other.length());
        _length = other.length();
        return *this;
    }

    String& String::operator=(const String& other) {
        resize(other.length());
        JE_COPY(_buffer, other.data(), other.length());
        _length = other.length();
        return *this;
    }

    char& String::operator[](size_t i) {
        JE_CORE_ASSERT(i < _length, "Index out of range!");
        return _buffer[i];
    }

    const char String::operator[](size_t i) const {
        JE_CORE_ASSERT(i < _length, "Index out of range!");
        return _buffer ? _buffer[i] : '\0';
    }

    void String::reserve(size_t charsNeeded) {
        if (_buffer && (charsNeeded + 1) <= _capacity) { return; }
        resize(charsNeeded);
    }
    void String::resize(size_t charsNeeded) {
        JE_CORE_ASSERT(charsNeeded < npos, "Too many chars!");

        charsNeeded++;
        if (!_buffer) {
            _buffer = reinterpret_cast<char*>(malloc(charsNeeded));
            JE_CORE_ASSERT(_buffer, "Couldn't allocate String!");
  
            _length = Math::min(_length, charsNeeded - 1);
            if (_buffer) {
                _buffer[_length] = 0;
            }
            _capacity = charsNeeded;
            return;
        }

        if (charsNeeded == _capacity) { return; }
        void* reloc = JE_REALLOC(_buffer, charsNeeded);
        JE_CORE_ASSERT(reloc, "Couldn't reallocate String!");
        if (!reloc) { return; }
        _buffer = reinterpret_cast<char*>(reloc);

        _length = Math::min(_length, charsNeeded - 1);
        if (_buffer) {
            _buffer[_length] = 0;
        }
        _capacity = charsNeeded;
    }
    void String::release() {
        if (_buffer) {
            JE_FREE(_buffer);
        }
        _capacity = 0;
        _length = 0;
    }

    String& String::append(const std::string_view& view) {
        reserve(view.length() + _length + 1);
        JE_COPY(_buffer + _length, view.data(), view.length());
        _length += view.length();
        if (_buffer) {
            _buffer[_length] = 0;
        }
        return *this;
    }
    String& String::append(const String& other) {
        reserve(other._length + _length + 1);
        JE_COPY(_buffer + _length, other._buffer, other._length);
        _length += other._length;
        if (_buffer) {
            _buffer[_length] = 0;
        }
        return *this;
    }

    String& String::operator+=(const std::string_view & view) {
        return this->append(view);
    }

    String& String::operator+=(const String& other) {
        return this->append(other);
    }

    String String::operator+(const std::string_view& view) const {
        String newStr(view.length() + _length);
        JE_COPY(newStr._buffer, _buffer, _length);
        JE_COPY(newStr._buffer + _length, view.data(), view.length());
        newStr._length = view.length() + _length;
        if (newStr._buffer) {
            newStr._buffer[newStr._length] = 0;
        }
        return newStr;
    }

    String String::operator+(const String& other) const {
        String newStr(other._length + _length);
        JE_COPY(newStr._buffer, _buffer, _length);
        JE_COPY(newStr._buffer + _length, other._buffer, other._length);
        newStr._length = other._length + _length;
        if (newStr._buffer) {
            newStr._buffer[newStr._length] = 0;
        }
        return newStr;
    }

    String String::substr(size_t index) const {
        return substr(index, _length - index);
    }

    String String::substr(size_t index, size_t length) const {
        JE_CORE_ASSERT(index < _length, "Index out of range!");
        JE_CORE_ASSERT(length < _length - index, "String is too short!");
        return String(std::string_view(_buffer + index, length));
    }
}