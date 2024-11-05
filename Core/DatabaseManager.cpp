#include "DatabaseManager.h"

namespace Core
{

    DatabaseManager::DatabaseManager(QObject* parent)
        : Base::ServiceBase(parent)
    {}

    DatabaseManager::~DatabaseManager()
    {}

    QSharedPointer<Sql::DBBase> DatabaseManager::GetStockDatabase()
    {
        // Pvt.db
        QString fileName = Base::PathUtil::GetDataDir() + STOCK_DATABASE_NAME + ".db";
        if(m_databasePool.contains(fileName))
            return m_databasePool[fileName];
        else
        {
            QSharedPointer<Sql::StockDB> stockDb =  CreateDatabase<Sql::StockDB>(fileName);
            //数据库有问题
            if(stockDb.isNull())
            {
                stockDb = RecreateDatabase<Sql::StockDB>(STOCK_DATABASE_NAME, fileName);
            }
            else //备份数据库
            {
                BackupDatabase(STOCK_DATABASE_NAME, fileName);
            }
            return stockDb;
        }
    }

    QSharedPointer<Sql::DBBase> DatabaseManager::GetBusinessDatabase(const QDate& date)
    {
        // Businessyyyy-MM-dd.db
        QString fileName = Base::PathUtil::GetDataDir() + BUSINESS_DATABASE_NAME + date.toString("yyyy-MM-dd") + ".db";
        if(m_databasePool.contains(fileName))
            return m_databasePool[fileName];
        else
        {
            QSharedPointer<Sql::BusinessDB> businessDb = CreateDatabase<Sql::BusinessDB>(fileName);
            //数据库有问题
            if(businessDb.isNull())
            {
                businessDb = RecreateDatabase<Sql::BusinessDB>(BUSINESS_DATABASE_NAME, fileName);
            }
            else //备份数据库
            {
                BackupDatabase(BUSINESS_DATABASE_NAME, fileName);
            }
            return businessDb;
        }
    }

    QSharedPointer<Sql::DBBase> DatabaseManager::GetBusinessDatabase(const QString& name)
    {
        QString fileName = Base::PathUtil::GetDataDir() + name;
        if(m_databasePool.contains(fileName))
            return m_databasePool[fileName];
        else
            return CreateDatabase<Sql::BusinessDB>(fileName);
    }

    QSharedPointer<Sql::DBBase> DatabaseManager::GetTempDatabase()
    {
        QString fileName = Base::PathUtil::GetDataDir() + TEMP_DATABASE_NAME + ".db";
        if(m_databasePool.contains(fileName))
            return m_databasePool[fileName];
        else
        {
            QSharedPointer<Sql::BusinessDB> saleDb = CreateDatabase<Sql::BusinessDB>(fileName);
            //数据库有问题
            if(saleDb.isNull())
            {
                //直接删除数据库
                QFile::remove(fileName);

                //再创建新的数据库一次
                saleDb = CreateDatabase<Sql::BusinessDB>(fileName);
            }
            return saleDb;
        }
    }


    template<typename T> QSharedPointer<T> DatabaseManager::CreateDatabase(const QString& name)
    {
        QMutexLocker locker(&m_mtx);
        if(m_databasePool.contains(name))
            return qSharedPointerCast<T>(m_databasePool[name]);

        auto database = QSharedPointer<T>::create();
        Sql::DatabaseInfo info;
        info.databaseName = name;
        //info.supportCipher = QSqlDatabase::isDriverAvailable("SQLITECIPHER");
        info.databaseType = info.supportCipher ? "SQLITECIPHER" : "QSQLITE";
        //info.databasePwd = "test";

        database->SetDatabaseInfo(info);
        if(!database->Connect())
        {
            Q_ASSERT(0);
            return QSharedPointer<T>();
        }

        if(!database->CreateTable())
        {
            Q_ASSERT(0);
            return QSharedPointer<T>();
        }

        if(!UpgradeDatabase(database))
        {
            Q_ASSERT(0);
            return QSharedPointer<T>();
        }

        m_databasePool[name] = database;
        return database;
    }

    template<typename T>
    QSharedPointer<T> DatabaseManager::RecreateDatabase(const QString& baseName, const QString& fileName)
    {
        //LOG_ERROR(QStringLiteral("数据库有问题，文件：%1").arg(fileName).toUtf8());
        //先重命名现有数据库
        QString errorName = Base::PathUtil::GetDataDir() + baseName + "_error.db";
        QFile::remove(errorName);
        if(!QFile::rename(fileName, errorName))
        {
            //LOG_ERROR(QStringLiteral("数据库重命名失败，文件：%1").arg(fileName).toUtf8());
        }
        
        //埋点
        
        //再创建新的数据库一次
        return CreateDatabase<T>(fileName);
    }

    VersionEntityPtr DatabaseManager::QueryTableVersion(const Orm::QdOrmClass* ormClass)
    {
        Orm::QdOrmWhere query;
        query.where(VersionEntity::ColumnName()).isEqualTo(ormClass->tableName()).limit(1);
        QString error;
        VersionEntityPtr version;
        VersionEntityPtrList records;
        if(m_versionDML.FindByQuery(records, query, error) && !records.isEmpty())
            version = records.first();
        return version;
    }

    bool DatabaseManager::RegisterTableVersion(const Orm::QdOrmClass* ormClass)
    {
        // 表版本已注册，直接返回
        if(QueryTableVersion(ormClass) != nullptr)
            return true;

        auto versionEntity = QSharedPointer<VersionEntity>::create();
        versionEntity->SetName(ormClass->tableName());
        versionEntity->SetVersion(ormClass->version());
        QString error;
        return m_versionDML.Add(versionEntity, error);
    }

    bool DatabaseManager::UpdateTableVersion(const Orm::QdOrmClass* ormClass)
    {
        // 表版本不存在，更新失败
        auto versionEntity = QueryTableVersion(ormClass);
        if(versionEntity == nullptr)
            return false;

        // 表版本一致不需要更新
        if(ormClass->version() <= versionEntity->GetVersion())
            return true;

        versionEntity->SetVersion(ormClass->version());
        QString error;
        return m_versionDML.UpdateById(versionEntity, QStringList(), error);
    }

    bool DatabaseManager::CreateTableIndex(const Orm::QdOrmClass* ormClass, const QSqlDatabase& database)
    {
        QSqlQuery query(database);

        const auto& dataMembers = ormClass->ormMetaMembers();
        for(const auto& ormMember : dataMembers)
        {
            if(ormMember->isIndex())
            {
                QString sql = "CREATE INDEX " + ormClass->tableName() + "_" + ormMember->fieldName() + "_idx" + " ON " + ormClass->tableName() + " (" + ormMember->fieldName() + ");";
                if(!query.exec(sql))
                    return false;
            }
        }
        return true;
    }

    bool DatabaseManager::UpgradeTable(const Orm::QdOrmClass* ormClass, VersionEntityPtr version, const QSqlDatabase& database)
    {
        QSqlQuery query(database);
        const auto& dataMembers = ormClass->ormMetaMembers();
       
        for(const auto& ormMember : dataMembers)
        {
            if(ormMember.isNull() || (ormMember->fieldVersion() <= version->GetVersion()))
                continue;

            // 新增列
            QString sql = "ALTER TABLE " + ormClass->tableName() + " ADD COLUMN " + ormMember->fieldName() + " " + ormMember->fieldSqlForm() + ";";
            if(!query.exec(sql))
            {
                QString s = query.lastError().text();
                return false;
            }

            // 新增索引
            if(ormMember->isIndex())
            {
                sql = "CREATE INDEX " + ormClass->tableName() + "_" + ormMember->fieldName() + "_idx" + " ON " + ormClass->tableName() + " (" + ormMember->fieldName() + ");";
                if(!query.exec(sql))
                    return false;
            }
        }
        return true;
    }

    bool DatabaseManager::UpgradeDatabase(QSharedPointer<Sql::DBBase> database)
    {
        if(database.isNull())
            return false;

        m_versionDML.SetDatabase(database);
        QStringList tables = database->GetSqlDatabase().tables();
        for( const auto& table : tables)
        {
            const Orm::QdOrmClass* ormClass = Orm::QdOrmFactory::Get()->getAppointClass(table);
            if(!ormClass)
                continue;

            // 根据类名获取version表中记录的版本
            VersionEntityPtr version = QueryTableVersion(ormClass);
            if(version != nullptr)
            {
                // 过滤已更新的类
                if(ormClass->version() > version->GetVersion())
                {
                    if(!UpgradeTable(ormClass, version, database->GetSqlDatabase()))
                        return false;

                    if(!UpdateTableVersion(ormClass))
                        return false;
                }
            }
            else
            {
                // 注册新表的版本号
                if(!RegisterTableVersion(ormClass))
                    return false;

                // 创建表索引
                if(!CreateTableIndex(ormClass, database->GetSqlDatabase()))
                    return false;
            }
        }
        return true;
    }

    void DatabaseManager::BackupDatabase(const QString& baseName, const QString& fileName)
    {
        QString backName = Base::PathUtil::GetDataDir() + baseName + "_backup.db";
        QFile::remove(backName);
        if(!QFile::copy(fileName, backName))
        {
            //LOG_ERROR(QStringLiteral("数据库备份COPY失败，文件：%1").arg(fileName).toUtf8());
        }
    }

    bool DatabaseManager::OnStart()
    {    
        // 预创建数据库
        if(GetStockDatabase().isNull())
            return false;

        return true;
    }

    void DatabaseManager::OnStop()
    {
        for(auto& i : m_databasePool)
            i->Disconnect();
    }
}
