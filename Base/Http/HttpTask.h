// Copyright (c) 2021 Dmall Pos Team

#pragma once

#include <QSharedPointer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QThread>
#include <QTimer>
#include "HttpConsts.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
namespace Net
{
    class HttpRequest;
    class HttpResponse;

    /** HTTP代理任务，负责请求发起和响应接收 */
    class HttpTask : public QObject
    {
        Q_OBJECT
    public:
        friend class HttpTaskExecutor;
        HttpTask(QSharedPointer<HttpRequest> request, QSharedPointer<HttpResponse> response, QObject* parent = nullptr);
        ~HttpTask();

        /** 任务是否正在执行 */
        bool IsRunning() const;

    protected:
        /** 设置网络管理器 */
        void SetNetworkAccessManager(QNetworkAccessManager* instance);

        /** 设置请求头部 */
        void SetRawHeader(QNetworkRequest& request);

        /** 开启SSL */
        void EnabledSSL(QNetworkRequest& request);

        /** 任务实现，由HttpTaskExecutor负责调度 */
        void Run();

        /** 检查任务是否超时 */
        bool IsExpired(qint64 timestamp) const;

    Q_SIGNALS:
        /** HTTP任务完成 */
        void signalTaskFinished(Net::HttpResultPtr result);

    protected Q_SLOTS:
        void onReplyReadReady();
        void onReplyFinished();
        void onReplyError(QNetworkReply::NetworkError code);
        void onReplySslErrors(const QList<QSslError>& errors);
        void onReplyDownloadProgress(qint64 recv, qint64 total);
        void onReplyUploadProgress(qint64 sent, qint64 total);
        void onTimeout();

    private:
        QSharedPointer<HttpRequest> m_request;
        QSharedPointer<HttpResponse> m_response;
        QNetworkAccessManager* m_networkAccessManager = nullptr;
        QNetworkReply* m_networkReply = nullptr;
        QTimer* m_timer = nullptr;
        qint64 m_timestamp = 0;
        int m_elapsed = 0;
        bool m_isRunning = false;
        bool m_neverTimeout = false;
    };
}

Q_DECLARE_METATYPE(Net::HttpTask*)
