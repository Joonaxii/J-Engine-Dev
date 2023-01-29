#include <JEngine/IO/Serialization/Serialization.h>
#include <fstream>
#include <iostream>

namespace JEngine::Serialization {

    const void serializeStringBinary(std::ostream& stream, const std::string& str) {
        uint64_t len = str.length();
        stream.write(reinterpret_cast<char*>(&len), sizeof(int32_t));
        if (len < 1) { return; }
        stream.write(str.c_str(), len);
    }

    const std::string deserializeStringBinary(std::istream& stream) {
        uint64_t len = 0;
        stream.read(reinterpret_cast<char*>(&len), sizeof(int32_t));
        if (len < 1) { return ""; }

        char* buf = reinterpret_cast<char*>(_malloca(len));
      
        std::string str("");
        if (buf) {
            stream.read(buf, len);
            str = std::string(buf, len);
        }
        _freea(buf);
        return str;
    }

    const void serializeWStringBinary(std::ostream & stream, const std::wstring & str) {
        const uint64_t len = str.length();
        stream.write(reinterpret_cast<const char*>(&len), sizeof(int32_t));
        stream.write(reinterpret_cast<const char*>(str.c_str()), len << 1);
    }

    const std::wstring deserializeWStringBinary(std::istream & stream) {
        int len = 0;
        stream.read(reinterpret_cast<char*>(&len), sizeof(int));
        if (len < 1) { return L""; }

        char* buf = reinterpret_cast<char*>(_malloca(len << 1));
        stream.read(buf, len << 1);
        std::wstring str = std::wstring(reinterpret_cast<wchar_t*>(buf), len);
        _freea(buf);
        return str;
    }

    const int checkFileType(std::istream& stream, const char* header) {    
        //Check if binary
        char hdr[4] { 0, 0, 0, 0 };
        stream.read(hdr, 4);
        stream.seekg(0, std::ios::beg);
        auto val = strncmp(header, hdr, 4);
        if (val == 0) { return BINARY_FILE; }

        //Check for JSON
        {
            char c = '\0';
            while (!stream.eof()) {
                stream.read(&c, 1);
                if (c == '{') { break; }
                if (!std::isspace(c)) { break; }
            }

            if (c == '{') {
                int off = 1;
                stream.seekg(-1, std::ios::end);
                auto len = stream.tellg();
                while (!stream.eof()) {
                    stream.read(&c, 1);

                    if (c == '}') { break; }
                    if (!std::isspace(c)) { break; }

                    off++;
                    stream.seekg(-off, std::ios::end);
                }

                stream.seekg(0, std::ios::beg);
                if (c == '}') { return JSON_FILE; }
            }
        }
        return UNKNOWN_FILE;
    }
}