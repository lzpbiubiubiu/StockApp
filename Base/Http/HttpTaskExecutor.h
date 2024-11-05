// Copyright (c) 2021 Dmall Pos Team

#pragma once

#include <QList>
#include <QSharedPointer>
#include <QObject>
#include <QNetworkReply>
#include <QThread>
#include <QTimer>
#include "HttpConsts.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpTask.h"

namespace Net
{
    /** HTTP回调线程 */
    enum CallbackThread
    {
        /** 主线程回调 */
        CALLBACK_IN_MAIN_THREAD = 0,

        /** 工作线程回调 */
        CALLBACK_IN_WORK_THREAD
    };

    /** HTTP任务调度 */
    class HttpTaskExecutor : public QObject
    {
        Q_OBJECT
    public:
        HttpTaskExecutor(CallbackThread cbt = CallbackThread::CALLBACK_IN_MAIN_THREAD, QObject* parent = nullptr);
        ~HttpTaskExecutor();

        void ExecuteTask(HttpTask* task);

    protected:
        Q_INVOKABLE void onExecuteTask(Net::HttpTask* task);

    protected Q_SLOTS:
        void onTaskFinished(Net::HttpResultPtr result);

    private:
        QNetworkAccessManager* m_networkManager = nullptr;
        QSharedPointer<QThread> m_thread;
    };
}
