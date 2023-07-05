#pragma once
#include <JEngine/IO/Stream.h>

class FileStream : public Stream {
public:
    FileStream();
    FileStream(const char* filepath);
    FileStream(const wchar_t* filepath);
    FileStream(const char* filepath, size_t length);
    FileStream(const wchar_t* filepath, size_t length);
    FileStream(const char* filepath, const char* mode, const int shared = _SH_DENYWR);
    FileStream(const wchar_t* filepath, const char* mode, const int shared = _SH_DENYWR);

    FileStream(const FileStream& other);
    FileStream(FileStream&& other) noexcept;

    ~FileStream();

    FileStream& operator=(FileStream&& other) noexcept;


    const char* getCFileMode() const {
        switch (_flags & 0x3) {
            default: return "b";
            case 0x1: return "wb";
            case 0x2: return "rb";
            case 0x3: return "wrb";
        }
    }
    void setFilepath(const char* filePath, size_t size) const;
    void setFilepath(const wchar_t* filePath, size_t size) const;

    void setFilepath(const char* filePath) const;
    void setFilepath(const wchar_t* filePath) const;

    void setFilepath(const std::string& filePath) const;
    void setFilepath(const std::wstring& filePath) const;
    const std::string& getFilePath() const;

    bool isOpen() const override;
    bool open(const char* filepath, const char* mode, const int shared = _SH_DENYWR, bool keepPosition = false) const;
    bool open(const wchar_t* filepath, const char* mode, const int shared = _SH_DENYWR, bool keepPosition = false) const;

    bool open(const char* mode, const int shared = _SH_DENYWR, bool keepPosition = false) const;
    bool flush() const override;
    bool close() const override;

    bool canWrite() const override { return _file != nullptr; }
    bool canRead()  const override { return _file != nullptr; }

    size_t read(void* buffer, size_t elementSize, size_t count, const bool bigEndian = false) const override;
    size_t write(const void* buffer, const size_t elementSize, const size_t count, const bool bigEndian = false) const override;

    size_t seek(int64_t offset, int origin) const override;

private:
    mutable FILE* _file;
    mutable std::string _filepath;
};