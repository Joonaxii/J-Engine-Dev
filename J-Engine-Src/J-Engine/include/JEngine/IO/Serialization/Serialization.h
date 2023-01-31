#pragma once
#include <type_traits>
#include <iostream>

namespace JEngine::Serialization {
    const static int UNKNOWN_FILE = -1;
    const static int BINARY_FILE = 0;
    const static int JSON_FILE = 1;

    const static int BINARY_VERSION = 0;

    const int checkFileType(std::istream& stream, const char* header);
}