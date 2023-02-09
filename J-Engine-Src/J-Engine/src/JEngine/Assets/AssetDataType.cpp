#include <JEngine/Assets/AssetDataType.h>

namespace JEngine {

    struct HeaderPair {
        AssetDataFormat fmt;
        const char* name;
        size_t len;

        HeaderPair() : fmt(), name(nullptr), len(0) {}
        HeaderPair(const AssetDataFormat fmt, const char* name, const size_t len) : 
            fmt(fmt), name(name), len(len) {}

        bool operator==(const HeaderPair& other) const {
            return len == other.len;
        }

        bool operator!=(const HeaderPair& other) const {
            return len != other.len;
        }

        bool operator<(const HeaderPair& other) const {
            return len > other.len;
        }

        bool operator>(const HeaderPair& other) const {
            return len < other.len;
        }
    };

    int32_t sortHeaderPairs(const HeaderPair& a, const HeaderPair& b) {
        return a.len < b.len ? -1 : a.len > b.len ? 1 : 0;
    }

    AssetDataFormat headerToAssetFormat(const ConstSpan<char> header) {
        static std::vector<HeaderPair> HEADERS{};
        if (HEADERS.empty()) {
            for (size_t i = 0; i < (int)(AssetDataFormat::MAX); i++) {
                const char* c = assetDataFormatToHeader((AssetDataFormat)i);
                HEADERS.push_back({ AssetDataFormat(i), c, strlen(c) });
            }
            std::sort(HEADERS.begin(), HEADERS.end());
        }

        for (size_t i = 0; i < HEADERS.size(); i++) {

            const auto& hdr = HEADERS[i];
            ConstSpan<char> hdrSp(hdr.name, hdr.len);
            if (hdr.fmt == AssetDataFormat::Unknown) { continue; }
            if (hdr.len > header.length()) { continue; }

            switch (hdr.fmt)
            {
                case JEngine::AssetDataFormat::WAV:
                    if (header.length() == 12) {
                        ConstSpan<char> main(hdr.name, hdr.len);
                        if (main.slice(0, 4).equals(header.slice(0, 4))) {
                            if (main.slice(8, 4).equals(header.slice(8, 4))) { return AssetDataFormat::WAV; }
                        }
                    }
                    break;
                default:
                    if (hdrSp.equals(header)) { return hdr.fmt; }
                    break;
            }
        }
    }

    AssetDataFormat headerToAssetFormat(const char* header) {
        return headerToAssetFormat(ConstSpan<char>(header, strlen(header)));
    }
}