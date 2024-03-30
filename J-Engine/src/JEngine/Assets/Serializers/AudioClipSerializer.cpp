#include <JEngine/Assets/Serializers/AudioClipSerializer.h>
#include <JEngine/Utility/StringHelpers.h>

namespace JEngine {

    uint8_t AudioClipSerializer::parseLabelFile(const Stream& stream, uint32_t sampleRate, AudioSection sections[AudioClip::MAX_SECTION_COUNT], size_t size) {
        if (!stream.isOpen()) { return 0; }
        uint8_t sectC = 0;
        size = size == SIZE_MAX ? stream.size() : size;

        char temp[96]{ 0 };

        char* buffer = reinterpret_cast<char*>(_malloca(size));
        if (buffer) {
            ConstSpan<char> span(buffer, size);
            span = span.trim();
            while (span.length() > 0) {
                int32_t next = span.indexOfAny("\n\r", 2);
                auto line = span.slice(0, next < 0 ? span.length() : next).trim();

                int32_t st = line.indexOfAny(" \t", 2);
                if (st > -1) {
                    memcpy(temp, line.get(), st);
                    temp[st] = 0;
                    double startT = atof(temp);

                    line = line.slice(st).trim();
                    st = line.indexOfAny(" \t", 2);
                    if (st > -1) {
                        memcpy(temp, line.get(), st);
                        temp[st] = 0;
                        double endT = atof(temp);

                        line = line.slice(st).trim();         
                        auto& section = sections[sectC];
                        section.targetSection = AudioSection::AUD_NULL_SECTION;

                        section.startSample = uint32_t(Audio::secondsToSample(startT, sampleRate));
                        section.endSample = uint32_t(Audio::secondsToSample(endT, sampleRate));

                        if (Helpers::endsWith(line, "_intro", false)) {
                            section.targetSection = uint8_t(sectC + 1);
                        }
                        else if (Helpers::endsWith(line, "_loop", false)) {
                            section.targetSection = uint8_t(sectC);
                        }

                        sectC++;
                        if (sectC >= AudioClip::MAX_SECTION_COUNT) { break; }
                    }
                }

                if (next < -1) {
                    break;
                }
                span = span.slice(next + 1).trim();
            }
            _freea(buffer);
        }
        return sectC;
    }
}