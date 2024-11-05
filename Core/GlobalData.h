#pragma once

#include <QHash>
#include <QReadWriteLock>
#include "Base/Common/Singleton.h"
#include "Core/StackStruct.h"
#include "Config.h"

namespace Core
{
    /** 全局共享数据对象 */
    class GlobalData
    {
        TO_BE_SINGLETON(GlobalData)

    public:
        /** 获取配置信息 */
        CommonConfig& GetConfig();

        /** 获取当前用户 */
        User& GetUser();

        /** 写KV数据 */
        bool SetValue(const QString& key, const QString& value);

        /** 读KV数据 */
        QString GetValue(const QString& key, const QString& defaultValue = QString());

    private:
        GlobalData();
        ~GlobalData();

        // 配置数据
        CommonConfig m_config;

        // 用户数据
        User m_user;

        // KV读写锁
        QReadWriteLock m_kvLock;

        // KV数据缓存
        QHash<QString, QString> m_kvCache;
    };
}
