#include <JEngine/IO/Serialization/Serialization.h>

namespace JEngine::Serialization {
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