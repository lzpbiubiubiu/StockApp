#include <QUuid>
#include <QTime>
#include <QtMath>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QFile>
#include <QImage>
#include <QBuffer>
#include <QFileInfo>
#include "MathUtil.h"

namespace Base
{
    QString MathUtil::GetUUID()
    {
        QUuid id = QUuid::createUuid();
        QString uuid = id.toString();
        uuid.remove("{").remove("}").remove("-");
        return uuid.toLower();
    }

    QString MathUtil::GetNonce()
    {
        return GetUUID().mid(0, 16);
    }

    QString MathUtil::GetMD5(const QString& content)
    {
        QByteArray md5 = QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex();
        return md5.toLower();
    }

    QString MathUtil::GetFileMD5(const QString& fileName)
    {
        if(!QFile::exists(fileName))
            return "";

        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly))
            return "";

        QCryptographicHash hash(QCryptographicHash::Md5);
        while(!file.atEnd())
        {
            // 64K
            QByteArray data = file.read(64 * 1024);
            hash.addData(data);
        }
        return QString(hash.result().toHex()).toLower();
    }

    QString MathUtil::GetSHA256(const QString& content)
    {
        QByteArray sha256 = QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Sha256).toHex();
        return sha256.toLower();
    }

    int MathUtil::GetRandomInt(int min, int max)
    {
        qsrand(QDateTime::currentMSecsSinceEpoch());
        return (qrand() % (max - min + 1)) + min;
    }

    bool MathUtil::IsFloatEqual(double a, double b, double epsilon)
    {
        return qFabs(a - b) < epsilon;
    }

    QString MathUtil::GetCipherNumber(const QString& plain)
    {
        static const char cipherTable[] = { '1', '9', '4', '6', '2', '5', '3', '0', '7', '8' };
        QString cipher;
        bool ok = false;
        for(auto& ch : plain)
        {
            quint8 num = QString(ch).toInt(&ok);
            Q_ASSERT(ok);
            cipher.append(cipherTable[num]);
        }
        return cipher;
    }

    QString MathUtil::GetPlainNumber(const QString& cipher)
    {
        static const char plainTable[] = { '7', '0', '4', '6', '2', '5', '3', '8', '9', '1' };
        QString plain;
        bool ok = false;
        for(auto& ch : cipher)
        {
            quint8 num = QString(ch).toInt(&ok);
            Q_ASSERT(ok);
            plain.append(plainTable[num]);
        }
        return plain;
    }

    QString MathUtil::GetImageFileBase64(const QString& imagePath)
    {
        QFile file(imagePath);
        if(!file.open(QIODevice::ReadOnly))
            return QString();

        QByteArray ba = file.readAll();
        file.close();
        return ba.toBase64();
    }

    bool MathUtil::IsNumber(const QString& text)
    {
        if(text.isEmpty())
            return false;

        QRegularExpression re("^[0-9]+$");
        return re.match(text).hasMatch();
    }

    bool MathUtil::IsEnglish(const QString& text)
    {
        if(text.isEmpty())
            return false;

        QRegularExpression re("^[a-zA-Z]+$");
        return re.match(text).hasMatch();
    }
}
