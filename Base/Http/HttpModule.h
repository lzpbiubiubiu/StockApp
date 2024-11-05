// Copyright (c) 2021 Dmall Pos Team

#pragma once

#include <QList>
#include <QSharedPointer>
#include "HttpConsts.h"
#include "HttpTaskExecutor.h"
#include "../Common/Singleton.h"

class QNetworkReply;
class QNetworkRequest;
class QNetworkAccessManager;

namespace Net
{
    class HttpRequest;
    class HttpResponse;

    /** HTTP请求处理模块 */
    class HttpModule : public QObject
    {
        Q_OBJECT
        TO_BE_SINGLETON(HttpModule)
    public:
        ~HttpModule();

        /**
         * 发起HTTP请求
         * @param request 请求对象
         * @param response 响应对象
         * @param cbt 回调函数执行线程
         * @param waitForFinished 是否阻塞等待请求完成
         */
        void ProcessRequest(
            QSharedPointer<HttpRequest> request,
            QSharedPointer<HttpResponse> response,
            bool waitForFinished = false,
            CallbackThread cbt = CALLBACK_IN_WORK_THREAD);

        /** 设置代理服务器 */
        void SetProxy(const QString& ip, int port);

    Q_SIGNALS:
        /** HTTP请求完成 */
        void signalRequestFinished(Net::HttpResultPtr result);

    private:
        HttpModule(QObject* parent = nullptr);

    private:
        QSharedPointer<HttpTaskExecutor> m_mainThreadExecutor;
        QList<QSharedPointer<HttpTaskExecutor>> m_workThreadExecutors;
    };
}
