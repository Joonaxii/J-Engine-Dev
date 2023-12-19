#include <JEngine/IO/Audio.h>
#include <JEngine/IO/FileStream.h>
#include <JEngine/Core/Log.h>

namespace JEngine {

    namespace Wav {
        struct Header {
            char riff[4]{ 'R', 'I', 'F', 'F' };
            uint32_t size{ 0 };
            char wave[4]{ 'W', 'A', 'V', 'E' };
        };

        struct FormatChunk {
            char fmt[4]{0};
            uint32_t size{ 0 };
            uint16_t type{ 0 };
            uint16_t channels{ 0 };
            uint32_t sampleRate{ 0 };
            uint32_t byteRate{ 0 };
            uint16_t blockAlign{ 0 };
            uint16_t bitsPerSample{ 0 };
        };

        bool decode(const char* path, AudioData& audio) {
            FileStream fs(path, "rb");
            if (fs.isOpen()) {
                return decode(fs, audio);
            }
            JENGINE_ERROR("[Audio-IO] (WAV) Decode Error: Failed to open file '{0}' for reading!", path);
            return false;
        }

        bool decode(const Stream& stream, AudioData& audio) {
            size_t pos = stream.tell();

            Header header{};
            size_t read = stream.readValue(header, false);

            if (read < 1 || 
                strncmp(header.riff, "RIFF", 4) != 0 ||
                strncmp(header.wave, "WAVE", 4) != 0 ||
                header.size <= 36) {
                JENGINE_ERROR("[Audio-IO] (WAV) Decode Error: Failed to decode wav, not a WAVE file!");
                return false;
            }
            FormatChunk fmtCh{};
            stream.readValue(fmtCh, false);

            if (strncmp(fmtCh.fmt, "fmt ", 4) != 0) {
                JENGINE_ERROR("[Audio-IO] (WAV) Decode Error: Failed to decode wav, SubChunk 1 is not where it should be!");
                return false;
            }

            if (fmtCh.type != 1) {
                JENGINE_ERROR("[Audio-IO] (WAV) Decode Error: Failed to decode wav, PCM is only supported but found type '{0}'!", fmtCh.type);
                return false;
            }

            //We seek to the start of SubChunk 2, just in case there is data we don't need in SubChunk1
            stream.seek(pos + fmtCh.size + 8 + sizeof(Header), SEEK_SET);
            char temp[5]{ 0 };
            stream.read(temp, 4, false);

            if (strncmp(temp, "data", 4) != 0) {
                JENGINE_ERROR("[Audio-IO] (WAV) Decode Error: Failed to decode wav, SubChunk 2 is not where it should be!");
                return false;
            }

            uint32_t size{ 0 };
            stream.readValue(size, false);

            audio.clear(false);
            audio.format = AudioFormat::PCM;
            audio.sampleRate = fmtCh.sampleRate;
            audio.depth = fmtCh.bitsPerSample;
            audio.channels = uint8_t(fmtCh.channels);
            audio.blockAlign = fmtCh.blockAlign;
            audio.sampleType = audio.depth == 8 ? AudioSampleType::Unsigned : AudioSampleType::Signed;
            audio.sampleCount = fmtCh.byteRate < 1 ? 0 : (size / fmtCh.byteRate);

            if (!audio.doAllocate(size)) {
                JENGINE_ERROR("[Audio-IO] (WAV) Decode Error: Failed to allocate audio data buffer!");
                return false;
            }
            stream.read(audio.data, size, false);
            return false;
        }
    }
}