#include <QThread>
#include <QSemaphore>
#include <QNetworkProxy>
#include <thread>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpTask.h"
#include "HttpModule.h"
#include "../Common/System.h"
#include "../Log/Log.h"

// 最小任务调度线程数
static const int MINIMAL_THREAD_POOL_SIZE = 1;

namespace Net
{
    HttpModule::HttpModule(QObject* parent)
        : QObject(parent)
    {
        m_mainThreadExecutor = QSharedPointer<HttpTaskExecutor>::create(CALLBACK_IN_MAIN_THREAD);
        int size = std::thread::hardware_concurrency();
        for(int i = 0; i < qMax(size, MINIMAL_THREAD_POOL_SIZE); ++i)
            m_workThreadExecutors << QSharedPointer<HttpTaskExecutor>::create(CALLBACK_IN_WORK_THREAD);
    }

    HttpModule::~HttpModule()
    {}

    void HttpModule::ProcessRequest(QSharedPointer<HttpRequest> request, QSharedPointer<HttpResponse> response,
        bool waitForFinished, CallbackThread cbt)
    {
        HttpTask* task = new HttpTask(request, response);
        QSemaphore semp;
        if(waitForFinished)
        {
            connect(task, &Net::HttpTask::signalTaskFinished, [&](QSharedPointer<Net::HttpResult> result) {
                semp.release();
            });
        }

        if(cbt == CALLBACK_IN_MAIN_THREAD)
        {
            m_mainThreadExecutor->ExecuteTask(task);
        }
        else
        {
            static int index = 0;
            int i = index++ % m_workThreadExecutors.size();
            m_workThreadExecutors[i]->ExecuteTask(task);
        }

        if(waitForFinished)
            semp.acquire();
    }

    void HttpModule::SetProxy(const QString& ip, int port)
    {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(ip);
        proxy.setPort(port);
        QNetworkProxy::setApplicationProxy(proxy);
    }
}
