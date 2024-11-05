#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include "HttpTask.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpModule.h"
#include "HttpTaskExecutor.h"

namespace Net
{
    HttpTaskExecutor::HttpTaskExecutor(CallbackThread cbt, QObject* parent)
        : QObject(parent)
    {
        m_networkManager = new QNetworkAccessManager;
        if(cbt == CALLBACK_IN_WORK_THREAD)
        {
            m_thread = QSharedPointer<QThread>::create();
            m_thread->start();
            m_networkManager->moveToThread(m_thread.data());
            moveToThread(m_thread.data());
        }
        else
        {
            m_networkManager->moveToThread(QCoreApplication::instance()->thread());
            moveToThread(QCoreApplication::instance()->thread());
        }
    }

    HttpTaskExecutor::~HttpTaskExecutor()
    {
        m_networkManager->deleteLater();
        if(m_thread != nullptr)
        {
            m_thread->quit();
            m_thread->wait();
        }
    }

    void HttpTaskExecutor::ExecuteTask(HttpTask* task)
    {
        Q_ASSERT(task);
        task->moveToThread(thread());
        QMetaObject::invokeMethod(this, "onExecuteTask", Qt::QueuedConnection, Q_ARG(Net::HttpTask*, task));
    }

    void HttpTaskExecutor::onExecuteTask(Net::HttpTask* task)
    {
        connect(task, &HttpTask::signalTaskFinished, this, &HttpTaskExecutor::onTaskFinished);
        task->SetNetworkAccessManager(m_networkManager);
        task->Run();
    }

    void HttpTaskExecutor::onTaskFinished(Net::HttpResultPtr result)
    {
        sender()->deleteLater();
        emit Net::HttpModule::Get()->signalRequestFinished(result);
    }
}