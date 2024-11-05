#pragma once

#include <QByteArray>

namespace Base
{
    /** 数据压缩 */
    class Compression
    {
    public:
        /**
         * deflate压缩数据
         * @param data: 未压缩数据
         * @return 成功：压缩后的数据，失败：空数组
         */
        static QByteArray Compress(const QByteArray& data);


        /**
         * gzip压缩数据
         * @param data: 未压缩数据
         * @return 成功：压缩后的数据，失败：空数组
         */
        static QByteArray GzipCompress(const QByteArray& data);

        /**
         * deflate 解压数据
         * @param data: 压缩数据
         * @return 成功：解压后的数据，失败：空数组
         */
        static QByteArray Uncompress(const QByteArray& data);

        /**
         * gzip 解压数据
         * @param data: 压缩数据
         * @return 成功：解压后的数据，失败：空数组
         */
        static QByteArray GzipUncompress(const QByteArray& data);
    };
}
