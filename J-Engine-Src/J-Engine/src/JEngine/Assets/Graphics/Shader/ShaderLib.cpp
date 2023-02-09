#include <JEngine/Assets/Graphics/Shader/ShaderLib.h>
#include <JEngine/Helpers/StringConsts.h>
#include <sstream>
#include <JEngine/Utility/Span.h>

namespace JEngine {
    void ShaderLib::parse(std::istream& stream) {
        clear();

        std::stringstream strBuffer;

        const size_t start = stream.tellg();
        stream.seekg(0, std::ios::end);

        const size_t end = stream.tellg();
        const size_t length = end - start;
        stream.seekg(start, std::ios::beg);

        char* libStr = reinterpret_cast<char*>(malloc(length));
        stream.read(libStr, length);

        Span<char> libDataSpan(libStr, length);
        libDataSpan = libDataSpan.trim();

        size_t scope = 0;
        Span<char> temp;
        for (size_t i = 0; i < length; i++) {
            const char c = libDataSpan[i];
            const char nxtC = i < (length - 1) ?  libDataSpan[i + 1] : '\0';

            switch (c)
            {
                case '/': //Most likely a comment
                    if (nxtC == '*') {
                        temp = libDataSpan.slice(i);
                        int32_t endOfComment = temp.indexOf("*/");

                        assert(endOfComment > -1 && "Invalid comment found!");
                        i += size_t(endOfComment) - 1;
                        scope = i + 1;
                        continue;
                    }
                    break;

                default:
                    
                    break;
            }


            if (isWhiteSpace(c)) {



                scope = i;
            }
        }

        free(libStr);
    }
}
