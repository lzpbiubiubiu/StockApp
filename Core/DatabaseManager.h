#pragma once

#include "Base/Util/PathUtil.h"
#include "Base/Util/MathUtil.h"
#include "Database/StockDB.h"
#include "Database/BusinessDB.h"
#include "Base/Common/ServiceBase.h"
#include "Entity/VersionEntity.h"
#include "Base/Database/Base/DBBase.h"
#include "Base/Database/Base/DmlBase.h"

/** 配置数据库名称 */
#define STOCK_DATABASE_NAME ("Stock")

/** 业务数据库名称 */
#define BUSINESS_DATABASE_NAME ("Business")

/** 临时数据库名称 */
#define TEMP_DATABASE_NAME ("Temp")

namespace Core
{
    /** 数据库管理器服务，实现数据库文件的管理 */
    class DatabaseManager : public Base::ServiceBase
    {
        Q_OBJECT
    public:
        DatabaseManager(QObject* parent = nullptr);
        ~DatabaseManager();

        /** 获取配置数据库 */
        QSharedPointer<Sql::DBBase> GetStockDatabase();

        /** 获取业务数据库 */
        QSharedPointer<Sql::DBBase> GetBusinessDatabase(const QDate& date);
        QSharedPointer<Sql::DBBase> GetBusinessDatabase(const QString& name);

        /** 获取临时数据库 */
        QSharedPointer<Sql::DBBase> GetTempDatabase();

    protected:
        /**
         * 创建SQLite数据库
         * @param name: 数据库名称
         * @return 成功返回非空值
         */
        template<typename T> QSharedPointer<T> CreateDatabase(const QString& name);

        /** 查询表版本 */
        VersionEntityPtr QueryTableVersion(const Orm::QdOrmClass* ormClass);

        /** 登记表版本 */
        bool RegisterTableVersion(const Orm::QdOrmClass* ormClass);

        /** 更新表版本 */
        bool UpdateTableVersion(const Orm::QdOrmClass* ormClass);

        /** 创建表索引 */
        bool CreateTableIndex(const Orm::QdOrmClass* ormClass, const QSqlDatabase& database);

        /** 升级表 */
        bool UpgradeTable(const Orm::QdOrmClass* ormClass, VersionEntityPtr version, const QSqlDatabase& database);

        /** 升级库 */
        bool UpgradeDatabase(QSharedPointer<Sql::DBBase> database);

        /** 重新生成新库 */
        template<typename T> QSharedPointer<T> RecreateDatabase(const QString& baseName, const QString& fileName);

        /** 备份数据库 */
        void BackupDatabase(const QString& baseName, const QString& fileName);

        virtual bool OnStart() override;

        virtual void OnStop() override;

    private:
    private:
        // 数据库创建锁
        QMutex m_mtx;

        // 数据库缓存
        QHash<QString, QSharedPointer<Sql::DBBase>> m_databasePool;

        // version表操作对象
        Sql::DmlBase<Sql::VersionEntity> m_versionDML;
    };
}
