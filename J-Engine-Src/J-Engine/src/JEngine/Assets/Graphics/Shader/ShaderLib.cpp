#include <JEngine/Assets/Graphics/Shader/ShaderLib.h>
#include <JEngine/Helpers/StringConsts.h>
#include <sstream>
#include <JEngine/Utility/Span.h>

namespace JEngine {
    void ShaderLib::parse(const Stream& stream) {
        clear();

        std::stringstream strBuffer;
        const size_t length = stream.size();

        char* libStr = reinterpret_cast<char*>(_malloca(length));
        if (!libStr) {
            JENGINE_CORE_ERROR("Failed to allocate shader buffer!");
            return;
        }

        stream.read(libStr, length, false);

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
                        int32_t endOfComment = indexOf(temp, "*/");

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

        _freea(libStr);
    }
}
