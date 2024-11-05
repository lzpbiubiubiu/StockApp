#include <QMutex>
#include <QTime>
#include <QSemaphore>
#include <QMetaEnum>
#include "ServiceManager.h"
#include "Base/Log/Log.h"
namespace Base
{
    ServiceManager::ServiceManager(QObject* parent)
        : QObject(parent)
    {
        moveToThread(&m_thread);
        m_thread.start();
    }

    ServiceManager::~ServiceManager()
    {
        m_thread.quit();
        m_thread.wait();
    }

    void ServiceManager::Load(StartupOption option)
    {
        // 以ServiceName枚举定义顺序同步加载
        QMetaObject::invokeMethod(this, "onLoad", Q_ARG(StartupOption, option));
    }

    void ServiceManager::Unload()
    {
        // 以ServiceName枚举定义反向顺序同步卸载
        QMetaObject::invokeMethod(this, "onUnload");
    }

    void ServiceManager::onLoad(StartupOption option)
    {
        LOG_INFO(QStringLiteral("正在启动服务..."));

        // 按枚举定义顺序启动服务线程
        bool exit = false;
        QTime tick;
        QSemaphore semp;
        QStringList::const_iterator itr = m_entryNames.cbegin();
        for(; itr != m_entryNames.cend(); ++itr)
        {
            QString serviceName = (*itr);
            if(!m_entries.contains(serviceName))
                continue;

            if(m_entries[serviceName].instance->GetState() == IService::RUNNING)
                continue;

            // 启动方式不匹配
            if(m_entries[serviceName].option != option)
                continue;

            // 断开之前的连接，防止重复连接响应槽函数引起崩溃
            QObject::disconnect(m_entries[serviceName].instance, &ServiceBase::signalStarted, this, 0);
            QObject::disconnect(m_entries[serviceName].instance, &ServiceBase::signalStartError, this, 0);

            QObject::connect(m_entries[serviceName].instance, &ServiceBase::signalStarted, this, [&]()
            {
                semp.release();
            }, Qt::DirectConnection);

            QObject::connect(m_entries[serviceName].instance, &ServiceBase::signalStartError, this, [&](const QString& message)
            {
                m_entries[serviceName].instance->m_thread.quit();

                emit signalLoadError(message);
                QString errmsg = QStringLiteral("[%1]...FAILED，错误：%2").arg(serviceName).arg(message);
                //LOG_ERROR(errmsg.toUtf8());
                exit = true;
                semp.release();
            }, Qt::DirectConnection);

            // 启动服务实例
            tick.restart();
            m_entries[serviceName].instance->m_thread.start();

            // 等待启动完成
            semp.acquire();

            LOG_INFO(QStringLiteral("[%1]...STARTED %2ms").arg(serviceName).arg(tick.elapsed()));

            // 错误中断
            if(exit) break;
        }

        if(!exit)
        {
            LOG_INFO(QStringLiteral("服务启动完成"));
            emit signalLoaded(option);
        }
    }

    void ServiceManager::onUnload()
    {
        LOG_INFO(QStringLiteral("正在停止服务..."));

        // 按枚举定义倒序终止服务线程
        QTime tick;
        QStringList::const_iterator itr = m_entryNames.cbegin();
        for(; itr != m_entryNames.cend(); ++itr)
        {
            QString serviceName = (*itr);
            if(!m_entries.contains(serviceName))
                continue;

            if(m_entries[serviceName].instance->GetState() == IService::STOPPED)
                continue;

            tick.restart();
            m_entries[serviceName].instance->m_thread.quit();
            m_entries[serviceName].instance->m_thread.wait(20000);
            LOG_INFO(QStringLiteral("[%1]...STOPPED %2ms").arg(serviceName).arg(tick.elapsed()));
        }
        LOG_INFO(QStringLiteral("服务停止完成"));
        emit signalUnloaded();
    }
}
