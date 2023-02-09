#include <JEngine/IO/Compression/ZLibStream.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>

namespace JEngine::ZLib {
    int32_t deflateData(char* dataIn, const size_t lenIn, char* dataOut, const size_t lenOut, const int32_t level) {
        return 0;
    }

    int32_t inflateData(char* dataIn, const size_t lenIn, char* dataOut, const size_t lenOut) {
        z_stream zInfo = { 0 };
        zInfo.total_in = zInfo.avail_in = lenIn;
        zInfo.total_out = zInfo.avail_out = lenOut;
        zInfo.next_in = reinterpret_cast<uint8_t*>(dataIn);
        zInfo.next_out = reinterpret_cast<uint8_t*>(dataOut);

        int32_t nErr, nRet = -1;
        nErr = inflateInit(&zInfo);               
        if (nErr == Z_OK) {
            nErr = inflate(&zInfo, Z_FINISH);     
            if (nErr == Z_STREAM_END) {
                nRet = zInfo.total_out;
            }
        }
        inflateEnd(&zInfo);
        return(nRet);
    }

    int32_t inflateBegin(ZLibContext& context, char* dataOut, const size_t lenOut) {
        context.init(nullptr, 0, dataOut, lenOut);

        int32_t nErr;
        nErr = inflateInit(&context.stream);

        if (nErr == Z_OK) {
            return nErr;
        }
        inflateEnd(&context.stream);
        return nErr;
    }

    int32_t inflateSegment(ZLibContext& context, char* dataIn, const size_t lenIn, int32_t& dataOut, const bool isLast) {
        int32_t nErr, nRet = -1;

        context.refreshNext(dataIn, lenIn);
        nErr = inflate(&context.stream, Z_NO_FLUSH);
        if (nErr == Z_STREAM_END) {
            dataOut = context.stream.total_out;
            return inflateEnd(context, dataOut);
        } else if (nErr == Z_OK) {
            dataOut = context.stream.total_out;
        }
        return nErr;
    }

    int32_t inflateEnd(ZLibContext& context, int32_t& dataOut) {
        if (!context.initialized) { return Z_OK; }
        dataOut = context.stream.total_out;

        inflate(&context.stream, Z_FINISH);
        int32_t ret = inflateEnd(&context.stream);
        context.initialized = false;
        return ret;
    }

    int32_t deflateData(std::istream& streamIn, std::ostream& target, const int32_t level) {
        int ret, flush{};
        unsigned have;
        z_stream strm{};
        uint8_t in[CHUNK]{};
        uint8_t out[CHUNK]{};

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        ret = deflateInit(&strm, level);
        if (ret != Z_OK) { return ret; }

        do {
            strm.avail_in = uInt(streamIn.readsome(reinterpret_cast<char*>(in), CHUNK));
            if (streamIn.bad()) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
            flush = streamIn.eof() ? Z_FINISH : Z_NO_FLUSH;
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

    int32_t inflateData(std::istream& streamIn, std::ostream& target) {
        int ret;
        unsigned have;
        z_stream strm{};
        uint8_t in[CHUNK]{};
        uint8_t out[CHUNK]{};

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;

        ret = inflateInit(&strm);
        if (ret != Z_OK) { return ret; }

        do {
            strm.avail_in = uInt(streamIn.readsome(reinterpret_cast<char*>(in), CHUNK));
            if (streamIn.bad()) {
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

                if (target.bad()) {
                    (void)inflateEnd(&strm);
                    return Z_ERRNO;
                }
            } while (strm.avail_out == 0);
        } while (ret != Z_STREAM_END);

        (void)inflateEnd(&strm);
        return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
    }

}