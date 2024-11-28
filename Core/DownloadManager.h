#pragma once

#include <QFile>
#include <QReadWriteLock>
#include <QNetworkAccessManager>
#include <QTimer>
#include "Base/Common/ServiceBase.h"
#include "Base/Http/HttpRequest.h"
#include "Base/Http/HttpResponse.h"

namespace Core
{
    /** 下载器服务，实现文件异步下载 因为是全局服务，使用的时候要判断url是不是使用者要下载的url*/
    class DownloadManager : public Base::ServiceBase
    {
        Q_OBJECT
    public:
        DownloadManager(QObject* parent = nullptr);
        ~DownloadManager();

        /** 下载 默认3分超时 */
        void Download(const QString& url, const QString& fileName, int timeout = 180, int downLoadTimes = 1, const QString& md5 = "");

    Q_SIGNALS:
        void signalDownloadSuccess(const QString& url, const QString& fileName);
        void signalDownloadError(const QString& url, const QString& message);
        void signalDownProgress(const QString& url, qint64 bytesReceived, qint64 bytesTotal);

    protected:
        Q_INVOKABLE void onDownload(const QString& url, const QString& fileName, int timeout, int downLoadTimes, const QString& md5);

        virtual bool OnStart() override;

        virtual void OnStop() override;
        

    private Q_SLOTS:
        void onReplyReadyRead();
        void onReplyFinished();
        void onReplyError(QNetworkReply::NetworkError code);
        void onReplySslErrors(const QList<QSslError>& errors);
        void onDownProgress(qint64 bytesReceived, qint64 bytesTotal);
        void onTimeout();

    private:
        QNetworkAccessManager* m_networkManager = nullptr;
        QStringList m_urls;
    };
}
