#include <JEngine/Assets/Graphics/Shader/ShaderLib.h>
#include <sstream>
#include <JEngine/Utility/Span.h>
#include <JEngine/Utility/StringHelpers.h>

namespace JEngine {
    void ShaderLib::parse(const Stream& stream) {
        clear();

        std::stringstream strBuffer;
        const size_t length = stream.size();

        char* libStr = reinterpret_cast<char*>(_malloca(length));
        if (!libStr) {
            JE_CORE_ERROR("Failed to allocate shader buffer!");
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
                        size_t endOfComment = Helpers::indexOf(temp, "*/");

                        assert(endOfComment != SIZE_MAX && "Invalid comment found!");
                        i += endOfComment - 1;
                        scope = i + 1;
                        continue;
                    }
                    break;
            }


            if (std::isspace(c)) {



                scope = i;
            }
        }

        _freea(libStr);
    }
}
