#pragma once

namespace JEngine {
    template<typename T>
    struct EnumNames {
        static constexpr int32_t Count{0};

        static bool isNoName(const char* name) {
            return strlen(name) == 0;
        }

        static const char** getEnumNames() {
            return nullptr;
        }

        static const char* getEnumName(const T value) {
            int32_t index = int32_t(value);
            auto names = getEnumNames();
            if (index < 0 || index >= Count || !names) { return ""; }
            return names[index];
        }

        static bool noDraw(int32_t index);
        static bool getNextValidIndex(int32_t& index);
    };

    template<typename T>
    bool EnumNames<T>::noDraw(int32_t index) {
        auto names = getEnumNames();
        if (index < 0 || index >= Count || !names) { return true; }
        return isNoName(names[index]);
    }

    template<typename T>
    inline bool EnumNames<T>::getNextValidIndex(int32_t& index) {
        auto names = getEnumNames();
        if (!names) { return true; }

        int32_t original = index;
        if (index < 0 || index >= Count || noDraw(index)) {
            while (true) {
                index++;
                if (index >= Count) {
                    index = 0;
                }
                if (index == original) { break; }

                if (!noDraw(index)) { break; }
            }
        }
        return index != original;
    }
}