#pragma once

#include <QString>

namespace Base
{
    /** 加解密工具类，提供常用加解密封装 */
    class CryptoUtil
    {
    public:
        /** Base64加密字符串 */
        static QString ToBase64Encode(const QString& plain);
        static QByteArray ToBase64Encode(const QByteArray& plain);

        /** Base64解密字符串 */
        static QString ToBase64Decode(const QString& cipher);
        static QByteArray ToBase64Decode(const QByteArray& cipher);

        /** 掩码加密字符串 前{front}位 后{behind}位 中间全部使用{mask}替换 */
        static QString ToMaskEncode(const QString& plain, int front, int behind, const QString& mask = "*");
    };
}
