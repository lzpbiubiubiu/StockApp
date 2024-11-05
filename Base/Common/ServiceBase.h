#pragma once

#include <QThread>
#include <QReadWriteLock>
#include <QMetaObject>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>
#include "IService.h"

// 冻结标识，提供单一操作互斥宏
#define SUPPORT_FROZEN_FLAG()\
    protected:\
        QMutex m_frozenLock;\
        bool m_isFrozen = false;

#define CHECK_FROZEN_FLAG()         do { QMutexLocker locker(&m_frozenLock); if(m_isFrozen) return; } while(0)
#define RESET_FROZEN_FLAG()         do { QMutexLocker locker(&m_frozenLock); m_isFrozen = false; } while(0)
#define CHECK_AND_SET_FROZEN_FLAG() do { QMutexLocker locker(&m_frozenLock); if(m_isFrozen) return; m_isFrozen = true; } while(0)

namespace Base
{
    /** 服务基类实现 */
    class ServiceBase : public QObject, public IService
    {
        Q_OBJECT
        friend class ServiceManager;
    public:

        ServiceBase(QObject* parent = nullptr);
        ~ServiceBase();

        virtual State GetState() override;

    protected:
        virtual bool OnStart() override;

        virtual void OnStop() override;

    Q_SIGNALS:
        /** 服务启动信号 */
        void signalStarted();

        /** 服务出错信号 */
        void signalStartError(const QString& message);

        /** 服务停止信号 */
        void signalStopped();

    protected:
        QThread m_thread;
        QReadWriteLock m_rwLock;
        State m_state = State::STOPPED;
    };
}
