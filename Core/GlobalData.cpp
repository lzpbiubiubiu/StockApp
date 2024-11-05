#include <QReadLocker>
#include <QWriteLocker>
#include <QSettings>
#include "Entity/KvEntity.h"
#include "Base/Common/ServiceManager.h"
#include "Base/Log/Log.h"
#include "DatabaseManager.h"
#include "GlobalData.h"

namespace Core
{
    GlobalData::GlobalData()
    {
    }

    GlobalData::~GlobalData()
    {
    }

    CommonConfig& GlobalData::GetConfig()
    {
        return m_config;
    }

    User& GlobalData::GetUser()
    {
        return m_user;
    }

    bool GlobalData::SetValue(const QString& key, const QString& value)
    {
        QWriteLocker locker(&m_kvLock);
        auto dbm = Base::GetService<Core::DatabaseManager>();
        Q_ASSERT(dbm && dbm->GetState() == Base::ServiceBase::RUNNING);

        Sql::DmlBase<Sql::KvEntity> kvDml;
        kvDml.SetDatabase(dbm->GetStockDatabase());
        auto entity = Sql::KvEntityPtr::create();
        entity->SetKey(key);
        entity->SetValue(value);
        QString sqlError;
        if(!kvDml.Save(entity, sqlError))
        {
            LOG_ERROR(QString("sql error: %1").arg(sqlError));
            return false;
        }
        m_kvCache[key] = value;
        return true;
    }

    QString GlobalData::GetValue(const QString& key, const QString& defaultValue)
    {
        QReadLocker locker(&m_kvLock);
        if(m_kvCache.contains(key))
            return m_kvCache[key];

        auto dbm = Base::GetService<Core::DatabaseManager>();
        Q_ASSERT(dbm && dbm->GetState() == Base::ServiceBase::RUNNING);
        Sql::DmlBase<Sql::KvEntity> kvDml;
        kvDml.SetDatabase(dbm->GetStockDatabase());

        auto entity = Sql::KvEntityPtr::create();
        entity->SetKey(key);
        QString sqlError;
        if(!kvDml.FindById(entity, sqlError))
        {
            LOG_INFO(QStringLiteral("指定KEY: %1 不存在，返回默认值：%2").arg(key).arg(defaultValue));
            return defaultValue;
        }
        QString value = entity->GetValue();
        m_kvCache[key] = value;
        return value;
    }
}
