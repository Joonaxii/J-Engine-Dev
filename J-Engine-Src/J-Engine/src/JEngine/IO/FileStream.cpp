#include <JEngine/IO/FileStream.h>
#include <Windows.h>

#include <io.h>
#define F_OK 0
#define access _access

uint8_t getIOFlags(const char* mode) {
    uint8_t io = 0;
    if (mode) {
        for (size_t i = 0; i < strlen(mode); i++) {
            switch (mode[i]) {
                case 'r':
                case 'R': io |= Stream::READ_FLAG; break;
                case 'w':
                case 'W': io |= Stream::WRITE_FLAG; break;
            }
        }
    }
    return io;
}

FileStream::FileStream() : Stream(), _file(nullptr), _filepath() {}
FileStream::FileStream(const char* filepath) :Stream(), _file(nullptr), _filepath(filepath) { }
FileStream::FileStream(const wchar_t* filepath) : Stream(), _file(nullptr), _filepath() {
    setFilepath(filepath);
}

FileStream::FileStream(const char* filepath, size_t length) : Stream(), _file(nullptr), _filepath(filepath, filepath + length) {}

FileStream::FileStream(const wchar_t* filepath, size_t length) : Stream(), _file(nullptr), _filepath() {
    setFilepath(filepath, length);
}

FileStream::FileStream(const char* filepath, const char* mode, const int shared) : Stream(getIOFlags(mode)), _file(nullptr), _filepath() {
    open(filepath, mode, shared);
}

FileStream::FileStream(const wchar_t* filepath, const char* mode, const int shared) : Stream(getIOFlags(mode)), _file(nullptr), _filepath() {
    open(filepath, mode, shared);
}

FileStream::FileStream(const FileStream& other) : Stream(other._flags, other._length, other._capacity), _filepath(other._filepath), _file(nullptr) {
    _position = other._position;
}

FileStream::FileStream(FileStream&& other) noexcept :
    Stream(std::exchange(other._flags, 0), other._length, other._capacity),
    _file(std::exchange(other._file, nullptr)), 
    _filepath(std::move(other._filepath))
{
    _position = other._position;
}
FileStream::~FileStream() { close(); }

FileStream& FileStream::operator=(FileStream&& other) noexcept {
    _flags = std::move(other._flags);
    _position = std::move(other._position);
    _length = std::move(other._length);
    _capacity = std::move(other._capacity);
    _filepath = std::move(other._filepath);
    _file = std::exchange(other._file, nullptr);
    return *this;
}

void FileStream::setFilepath(const char* filePath, size_t size) const {
    if (filePath) {
        _filepath.clear();
        _filepath.append(filePath, size);
    }
}

void FileStream::setFilepath(const char* filePath) const {
    if (filePath) {
        _filepath.clear();
        _filepath.append(filePath);
    }
}

void FileStream::setFilepath(const std::string& filePath) const {
    _filepath = filePath;
}

void FileStream::setFilepath(const wchar_t* filePath, size_t size) const {
    if (filePath && size > 0) {
        size_t len = WideCharToMultiByte(CP_UTF8, 0, filePath, int32_t(size), NULL, 0, NULL, NULL);
        char* temp = reinterpret_cast<char*>(_malloca(len + 1));
        if (!temp) { return; }
        temp[len] = 0;

        WideCharToMultiByte(CP_UTF8, 0, filePath, size, temp, int32_t(len), NULL, NULL);
        setFilepath(temp);
        _freea(temp);
    }
}

void FileStream::setFilepath(const wchar_t* filePath) const {
    setFilepath(filePath, wcslen(filePath));
}

void FileStream::setFilepath(const std::wstring& filePath) const {
    setFilepath(filePath.c_str(), filePath.length());
}

const std::string& FileStream::getFilePath() const { return _filepath; }

bool FileStream::isOpen() const { return _file != nullptr; }
bool FileStream::open(const char* filepath, const char* mode, const int shared, bool keepPosition) const {
    if (isOpen()) { return false; }

    _length = 0;
    _capacity = 0;
    setFilepath(filepath);

    if (!mode) { return false; }
    return open(mode, shared, keepPosition);
}

bool FileStream::open(const wchar_t* filepath, const char* mode, const int shared, bool keepPosition) const {
    if (isOpen()) { return false; }

    _length = 0;
    _capacity = 0;
    setFilepath(filepath);

    if (!mode) { return false; }
    return open(mode, shared, keepPosition);
}

bool FileStream::open(const char* mode, const int shared, bool keepPosition) const {
    if (isOpen()) { return true; }

    _flags &= uint8_t(~0x3);
    _flags |= getIOFlags(mode);

    _length = 0;
    _capacity = 0;
    size_t oldPos = _position;

    if (access(_filepath.c_str(), F_OK) == 0) {
        if (shared) {
            _file = _fsopen(_filepath.c_str(), mode, shared);
        }
        else {
            fopen_s(&_file, _filepath.c_str(), mode);
        }

        _position = 0;
        if (_file) {
            if (_flags & READ_FLAG) {
                _fseeki64_nolock(_file, 0, SEEK_END);
                _length = _ftelli64_nolock(_file);
                _capacity = _length;
                _fseeki64_nolock(_file, 0, SEEK_SET);
                _position = keepPosition ? std::min(oldPos, _capacity) : 0;
            }
            else {
                _position = keepPosition ? oldPos : 0;
            }
            return true;
        }
    }
    return false;
}

bool FileStream::flush() const {
    if (!_file || !(_flags & WRITE_FLAG)) { return false; }
    _fflush_nolock(_file);
    return true;
}

bool FileStream::close() const {
    _position = 0;
    _length = 0;
    _capacity = 0;

    if (_file) {
        if (_flags & WRITE_FLAG) {
            _fflush_nolock(_file);
        }
        _fclose_nolock(_file);
        _file = nullptr;
        return true;
    }
    return false;
}

size_t FileStream::read(void* buffer, size_t elementSize, size_t count, const bool bigEndian) const {
    if (!_file || !canRead()) { return 0; }

    size_t ret = _fread_nolock(buffer, elementSize, count, _file);
    _position += ret * elementSize;
    _position = _position > _length ? _length : _position;

    if (bigEndian) {
        JEngine::Data::reverseEndianess(reinterpret_cast<uint8_t*>(buffer), elementSize, count);
    }
    return ret * elementSize;
}

size_t FileStream::write(const void* buffer, size_t elementSize, const size_t count, const bool bigEndian) const {
    if (!_file || !canWrite()) { return 0; }

    size_t size = count * elementSize;
    _position += size;
    _length = _position > _length ? _position : _length;
    _capacity = _length;

    if (bigEndian) {
        uint8_t* tempBuf = reinterpret_cast<uint8_t*>(_malloca(size));

        if (tempBuf) {
            memcpy(tempBuf, buffer, size);
            JEngine::Data::reverseEndianess(tempBuf, elementSize, count);
            size_t ret = _fwrite_nolock(tempBuf, elementSize, count, _file);
            _freea(tempBuf);
            return ret * elementSize;
        }
    }
    return _fwrite_nolock(buffer, elementSize, count, _file) * elementSize;
}
size_t FileStream::seek(int64_t offset, int origin) const {
    if (!_file) { return _position; }

    if (origin == SEEK_SET) {
        _position = offset;
        _fseeki64_nolock(_file, offset, SEEK_SET);
        return _position;
    }
    else if (origin == SEEK_CUR) {
        _position += offset;
        _fseeki64_nolock(_file, offset, SEEK_CUR);
        return _position;
    }

    _position = _length - offset;
    _fseeki64_nolock(_file, _position, SEEK_SET);
    return _position;
}