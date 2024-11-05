#include <QReadLocker>
#include <QWriteLocker>
#include "ServiceBase.h"

namespace Base
{
    ServiceBase::ServiceBase(QObject* parent)
        : QObject(parent)
    {
        moveToThread(&m_thread);
        connect(&m_thread, &QThread::started, [this]()
        {
            QWriteLocker locker(&m_rwLock);
            if(OnStart())
            {
                m_state = State::RUNNING;
                emit signalStarted();
            }
            else
            {
                emit signalStartError("Failed to start services");
            }
        });

        connect(&m_thread, &QThread::finished, [this]()
        {
            QWriteLocker locker(&m_rwLock);
            if(m_state == State::RUNNING)
            {
                OnStop();
                m_state = State::STOPPED;
            }
            emit signalStopped();
        });
    }

    ServiceBase::~ServiceBase()
    {}

    bool ServiceBase::OnStart()
    {
        return true;
    }

    void ServiceBase::OnStop()
    {}

    ServiceBase::State ServiceBase::GetState()
    {
        QReadLocker locker(&m_rwLock);
        return m_state;
    }
}
