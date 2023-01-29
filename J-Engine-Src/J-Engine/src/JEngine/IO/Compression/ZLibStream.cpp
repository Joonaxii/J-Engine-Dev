#include <JEngine/IO/Compression/ZLibStream.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>

namespace JEngine::ZLib {
    const int deflateData(std::istream& streamIn, std::ostream& target, const int level) {
        int ret, flush;
        unsigned have;
        z_stream strm;
        unsigned char in[CHUNK];
        unsigned char out[CHUNK];

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        ret = deflateInit(&strm, level);
        if (ret != Z_OK) { return ret; }

        auto start = streamIn.tellg();
        streamIn.seekg(0, std::ios::end);
        auto end = streamIn.tellg();
        streamIn.seekg(start, std::ios::beg);

        do {
            strm.avail_in = uInt(streamIn.readsome(reinterpret_cast<char*>(in), CHUNK));
            if (streamIn.bad()) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
            flush = (streamIn.tellg() == end) ? Z_FINISH : Z_NO_FLUSH;
            strm.next_in = in;

            do {
                strm.avail_out = CHUNK;
                strm.next_out = out;
                ret = deflate(&strm, flush);
                assert(ret != Z_STREAM_ERROR);
                have = CHUNK - strm.avail_out;
                target.write(reinterpret_cast<char*>(out), have);

            } while (strm.avail_out == 0);
            assert(strm.avail_in == 0);
        } while (flush != Z_FINISH);

        assert(ret == Z_STREAM_END);
        (void)deflateEnd(&strm);
        return Z_OK;
    }

    const int inflateData(std::istream& streamIn, std::ostream& target) {
        int ret;
        unsigned have;
        z_stream strm;
        unsigned char in[CHUNK];
        unsigned char out[CHUNK];

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        ret = inflateInit(&strm);
        if (ret != Z_OK) { return ret; }

        do {
            strm.avail_in = uInt(streamIn.readsome(reinterpret_cast<char*>(in), CHUNK));
            if (streamIn.bad() || streamIn.fail()) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
            if (strm.avail_in == 0) { break; }

            strm.next_in = in;
            do {
                strm.avail_out = CHUNK;
                strm.next_out = out;

                ret = inflate(&strm, Z_NO_FLUSH);
                assert(ret != Z_STREAM_ERROR);

                switch (ret)
                {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                    __fallthrough;
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return ret;
                }

                have = CHUNK - strm.avail_out;
                auto before = target.tellp();
                target.write(reinterpret_cast<char*>(out), have);

                if (target.fail() || target.bad()) {
                    (void)inflateEnd(&strm);
                    return Z_ERRNO;
                }
            } while (strm.avail_out == 0);
        } while (ret != Z_STREAM_END);

        (void)inflateEnd(&strm);
        return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
    }

    const void zerr(const int ret) {
        switch (ret) {
            case Z_ERRNO:
                std::cout << "ZLib error!\n";
                break;
            case Z_STREAM_ERROR:
                std::cout << "ZStream error! (Possibly invalid compression level)\n";
                break;
            case Z_DATA_ERROR:
                std::cout << "Invalid or incomplete deflate data!\n";
                break;
            case Z_MEM_ERROR:
                std::cout << "ZLib out of memory!\n";
                break;
            case Z_VERSION_ERROR:
                std::cout << "ZLib version mismatch!\n";
                break;
        }
    }
}