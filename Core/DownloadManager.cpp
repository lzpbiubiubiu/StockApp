#include <QFileInfo>
#include <QUrl>
#include <QMutexLocker>
#include "Base/Log/Log.h"
#include "Base/Http/HttpModule.h"
#include "DownloadManager.h"
#include "Base/Util/PathUtil.h"
#include "Base/Util/MathUtil.h"

namespace Core
{
    DownloadManager::DownloadManager(QObject* parent)
        : ServiceBase(parent)
    {}

    DownloadManager::~DownloadManager()
    {}

    bool DownloadManager::OnStart()
    {
        m_networkManager = new QNetworkAccessManager(this);
        return true;
    }

    void DownloadManager::OnStop()
    {
    }

    void DownloadManager::Download(const QString& url, const QString& fileName, int timeout, int downLoadTimes, const QString& md5)
    {
        QMetaObject::invokeMethod(this, "onDownload", Q_ARG(QString, url), Q_ARG(QString, fileName), Q_ARG(int, timeout), Q_ARG(int, downLoadTimes), Q_ARG(QString, md5));
    }

    void DownloadManager::onDownload(const QString& url, const QString& fileName, int timeout, int downLoadTimes, const QString& md5)
    {
        if(!QUrl(url).isValid())
        {
            QString message = QString("url %1 invalid").arg(url);
            emit signalDownloadError(url, message);
            return;
        }

        if(m_urls.contains(url))
            return;

        QString tmpFileName = fileName;
        tmpFileName.append(".tmp");
        QFile* tmpFile = new QFile(tmpFileName);
        if(!tmpFile->open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            tmpFile->deleteLater();
            QString message = QString("create tmp file %1 failed").arg(tmpFileName);
            emit signalDownloadError(url, message);
            return;
        }

        auto reply = m_networkManager->get(QNetworkRequest(url));
        tmpFile->setParent(reply);
        tmpFile->setObjectName("file"); 

        QTimer* timer = new QTimer(reply);
        timer->setProperty("reply", QVariant::fromValue<QNetworkReply*>(reply));
        timer->setInterval(timeout * 1000);
        timer->start();

        reply->setProperty("url", url);
        reply->setProperty("file", QVariant::fromValue<QFile*>(tmpFile));
        reply->setProperty("timer", QVariant::fromValue<QTimer*>(timer));
        reply->setProperty("md5", md5);
        reply->setProperty("downloadTimes", downLoadTimes);

        connect(timer, &QTimer::timeout, this, &DownloadManager::onTimeout);
        connect(reply, &QNetworkReply::readyRead, this, &DownloadManager::onReplyReadyRead);
        connect(reply, &QNetworkReply::finished, this, &DownloadManager::onReplyFinished);
        connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
            this, &DownloadManager::onReplyError);
        connect(reply, &QNetworkReply::sslErrors, this, &DownloadManager::onReplySslErrors);

        connect(reply, &QNetworkReply::downloadProgress, this, &DownloadManager::onDownProgress);
        m_urls << url;
    }

    void DownloadManager::onReplyReadyRead()
    {
        auto reply = qobject_cast<QNetworkReply*>(sender());
        QString url = reply->property("url").toString();
        QFile* file = reply->property("file").value<QFile*>();
        QString md5 = reply->property("md5").toString();
        int downloadTimes = reply->property("downloadTimes").toInt();
        QByteArray data = reply->readAll();
        if(file->write(data) < data.size())
        {
            QString message = QString("write tmp file %1 failed").arg(file->fileName());
            LOG_ERROR(message.toUtf8());
            QTimer* timer = reply->property("timer").value<QTimer*>();
            reply->abort();
            timer->stop();
            file->flush();
            file->close();
            reply->deleteLater();
            m_urls.removeAll(url);
            QString tmpFileName = file->fileName();
            QFile::remove(tmpFileName);
            file->deleteLater();
            if(downloadTimes < 3)
                Download(url, file->fileName(), 180, (downloadTimes + 1), md5);
        }
    }

    void DownloadManager::onReplyFinished()
    {
        auto reply = qobject_cast<QNetworkReply*>(sender());
        if(reply->error() == QNetworkReply::NoError)
        {
            QString url = reply->property("url").toString();
            QFile* file = reply->property("file").value<QFile*>();
            QTimer* timer = reply->property("timer").value<QTimer*>();
            QString md5 = reply->property("md5").toString();
            int downloadTimes = reply->property("downloadTimes").toInt();
            timer->stop();
            file->flush();
            if(!md5.isEmpty()) 
            {
                QString mm = Base::MathUtil::GetFileMD5(file->fileName());
                if(mm != md5)
                {
                    LOG_ERROR(QStringLiteral("图片MD5不一致，图片地址：%1, 原始MD5:%2, MD5:%3").arg(url).arg(md5).arg(mm).toUtf8());
                    file->close();
                    reply->deleteLater();
                    QFile::remove(file->fileName());
                    m_urls.removeAll(url);
                    if(downloadTimes < 3)
                        Download(url, file->fileName(), 180, (downloadTimes + 1), md5);
                    return;
                }
            }
            file->close();
            reply->deleteLater();
            m_urls.removeAll(url);
            QString tmpFileName = file->fileName();
            QString newFileName = file->fileName().mid(0, file->fileName().size() - QString(".tmp").size());
            QFile::remove(newFileName);
            QFile::rename(tmpFileName, newFileName);
            emit signalDownloadSuccess(url, newFileName);
        }
    }

    void DownloadManager::onReplyError(QNetworkReply::NetworkError code)
    {
        auto reply = qobject_cast<QNetworkReply*>(sender());
        QString url = reply->property("url").toString();
        QFile* file = reply->property("file").value<QFile*>();
        QTimer* timer = reply->property("timer").value<QTimer*>();
        QString md5 = reply->property("md5").toString();
        int downloadTimes = reply->property("downloadTimes").toInt();

        LOG_ERROR(QString("download file error url:%1 msg:%2 code:%3").arg(url).arg(reply->errorString()).arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>()));

        timer->stop();
        file->flush();
        file->close();
        m_urls.removeAll(url);
        QString tmpFileName = file->fileName();
        QFile::remove(tmpFileName);
        file->deleteLater();
        if(downloadTimes < 3)
            Download(url, file->fileName(), 180, (downloadTimes + 1), md5);
        emit signalDownloadError(url, reply->errorString());
        reply->deleteLater();
    }

    void DownloadManager::onReplySslErrors(const QList<QSslError>& errors)
    {
        auto reply = qobject_cast<QNetworkReply*>(sender());
        QString url = reply->property("url").toString();
        QFile* file = reply->property("file").value<QFile*>();
        QTimer* timer = reply->property("timer").value<QTimer*>();
        QString md5 = reply->property("md5").toString();
        int downloadTimes = reply->property("downloadTimes").toInt();

        LOG_ERROR(QString("download file ssl error url:%1 msg:%2 code:%3").arg(url).arg(reply->errorString()).arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>()));

        timer->stop();
        file->flush();
        file->close();
        m_urls.removeAll(url);
        QString tmpFileName = file->fileName();
        QFile::remove(tmpFileName);
        file->deleteLater();
        if(downloadTimes < 3)
            Download(url, file->fileName(),180, (downloadTimes + 1), md5);
        emit signalDownloadError(url, reply->errorString());
        reply->deleteLater();
    }

    void DownloadManager::onDownProgress(qint64 bytesReceived, qint64 bytesTotal)
    {
        if(bytesTotal <= 0)
        {
            return;
        }

        auto reply = qobject_cast<QNetworkReply*>(sender());
        if (reply->error() == QNetworkReply::NoError)
        {
            QString url = reply->property("url").toString();
            emit signalDownProgress(url, bytesReceived, bytesTotal);
        }
    }

    void DownloadManager::onTimeout()
    {
        auto timer = qobject_cast<QTimer*>(sender());
        timer->stop();
        auto reply = timer->property("reply").value<QNetworkReply*>();
        if(reply) {
            QFile* file = reply->property("file").value<QFile*>();
            QString md5 = reply->property("md5").toString();
            QString url = reply->property("url").toString();
            int downloadTimes = reply->property("downloadTimes").toInt();
            reply->abort();
            reply->deleteLater();
            if(file)
            {
                file->flush();
                file->close();
                file->deleteLater();
            }
            if(downloadTimes < 3)
                Download(url, file->fileName(), 180, (downloadTimes + 1), md5);
        }
    }
}

