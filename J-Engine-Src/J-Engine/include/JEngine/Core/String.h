#pragma once
#include <cstdint>
#include <cstdlib>
#include <string_view>
#include <JEngine/Core/Memory.h>

namespace JEngine {
    class String {
    public:
        enum : size_t {
            npos = SIZE_MAX
        };

        constexpr String() : _length(0), _capacity(0), _buffer(nullptr) {}
        String(size_t capacity);
        String(std::string_view view);
        String(const String& other);
        String(String&& other) noexcept;
        ~String();

        char* data() { return _buffer; }
        const char* data() const { return _buffer; }

        size_t length() const { return _length; }
        size_t capacity() const { return _capacity; }

        String& operator=(std::string_view other);
        String& operator=(const String& other);

        operator std::string_view() const {
            return std::string_view(_buffer, _length);
        }

        char& operator[](size_t i);
        const char operator[](size_t i) const;

        String& operator+=(const std::string_view& view);
        String& operator+=(const String& other);

        String operator+(const std::string_view& view) const;
        String operator+(const String& other) const;

        void reserve(size_t charsNeeded);
        void resize(size_t charsNeeded);
        void release();

        String& append(const std::string_view& view);
        String& append(const String& other);

        String substr(size_t index) const;
        String substr(size_t index, size_t length) const;
    private:
        size_t _length{};
        size_t _capacity{};
        char* _buffer{};
    };
}