#include "CryptoUtil.h"

namespace Base
{
    QByteArray CryptoUtil::ToBase64Encode(const QByteArray& plain)
    {
#ifdef _DEBUG
        return plain;
#else
        return plain.toBase64();
#endif
    }

    QString CryptoUtil::ToBase64Encode(const QString& plain)
    {
#ifdef _DEBUG
        return plain;
#else
        return QString(ToBase64Encode(plain.toUtf8()));
#endif
    }

    QByteArray CryptoUtil::ToBase64Decode(const QByteArray& cipher)
    {
#ifdef _DEBUG
        return cipher;
#else
        return QByteArray::fromBase64(cipher);
#endif
    }

    QString CryptoUtil::ToBase64Decode(const QString& cipher)
    {
#ifdef _DEBUG
        return cipher;
#else
        return QString::fromUtf8(ToBase64Decode(cipher.toUtf8()));
#endif
    }

    QString CryptoUtil::ToMaskEncode(const QString& plain, int front, int behind, const QString& mask)
    {
        if(front < 0 || behind < 0)
            return plain;

        int length = plain.length();
        if(length <= front + behind)
            return plain;

        QString replaceElement = mask.repeated(length - behind - front);
        QString cipher = plain;
        cipher = cipher.replace(front, length - behind - front, replaceElement);
        return cipher;
    }
}
