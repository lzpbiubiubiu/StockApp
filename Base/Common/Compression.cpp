#include "Compression.h"
#include <QtZlib/zlib.h>

#define MOD_GZIP_ZLIB_WINDOWSIZE 15
#define MOD_GZIP_ZLIB_CFACTOR    9
#define MOD_GZIP_ZLIB_BSIZE      8096

namespace Base
{
    QByteArray Compression::Compress(const QByteArray& data)
    {
        return qCompress(data);
    }

    QByteArray Compression::GzipCompress(const QByteArray& data)
    {
        QByteArray outBuf;
        z_stream zstream;
        int err = 0;
        int GZIP_ENCODING = 16;
        if(!data.isEmpty())
        {
            zstream.zalloc = (alloc_func)0;
            zstream.zfree = (free_func)0;
            zstream.opaque = (voidpf)0;
            zstream.next_in = (Bytef *)data.data();
            zstream.avail_in = data.size();

            if(deflateInit2(&zstream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                MAX_WBITS + GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK)
            {
                return QByteArray();
            }

            for(;;) 
            {
                char destBuf[8192] = { 0 };
                zstream.next_out = (Bytef*)destBuf;
                zstream.avail_out = sizeof(destBuf);
                int err = deflate(&zstream, Z_FINISH);
                outBuf.append(destBuf, sizeof(destBuf) - zstream.avail_out);
                if(err == Z_STREAM_END || err != Z_OK)
                    break;
            }
            deflateEnd(&zstream);
        }
        return outBuf;
    }

    QByteArray Compression::Uncompress(const QByteArray& data)
    {
        return qUncompress(data);
    }

    QByteArray Compression::GzipUncompress(const QByteArray& data)
    {
        QByteArray outBuffer;
        z_stream zstream;
        zstream.zalloc = NULL;
        zstream.zfree = NULL;
        zstream.opaque = NULL;
        zstream.avail_in = data.size();
        zstream.next_in = (Bytef*)data.data();

        int err = -1;
        int ret = -1;
        err = inflateInit2(&zstream, MAX_WBITS + 16);
        if(err == Z_OK)
        {
            for(;;)
            {
                char buffer[8192] = { 0 };
                zstream.avail_out = sizeof(buffer);
                zstream.next_out = (Bytef*)buffer;
                int err = inflate(&zstream, Z_FINISH);
                outBuffer.append(buffer, sizeof(buffer) - zstream.avail_out);
                if(err == Z_STREAM_END || err != Z_OK)
                    break;
            }
        }
        inflateEnd(&zstream);
        return outBuffer;
    }
}
