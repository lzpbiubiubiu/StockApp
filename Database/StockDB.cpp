#include "StockDB.h"
#include "Entity/KvEntity.h"
#include "Entity/VersionEntity.h"
#include "Entity/SequenceEntity.h"
#include "Base/Database/Orm/QdOrmDao.h"

namespace Sql
{
    bool StockDB::CreateTable()
    {
        QWriteLocker locker(&GetLock());
        auto sqlDatabase = GetSqlDatabase();
        if(!sqlDatabase.isOpen())
            return false;

        auto tables = sqlDatabase.tables();

        // 创建tb_version表
        if(!tables.contains(VersionEntity::GetTableName()))
        {
            QSqlError error = Orm::create_table<VersionEntity>(&sqlDatabase);
            if(error.isValid())
            {
                //LOG_ERROR("sql error: {}", error.databaseText().toUtf8());
                return false;
            }
        }

        // 创建tb_sequence表
        if(!tables.contains(SequenceEntity::GetTableName()))
        {
            QSqlError error = Orm::create_table<SequenceEntity>(&sqlDatabase);
            if(error.isValid())
            {
                //LOG_ERROR("sql error: {}", error.databaseText().toUtf8());
                return false;
            }
        }

        // 创建tb_kv表
        if(!tables.contains(KvEntity::GetTableName()))
        {
            QSqlError error = Orm::create_table<KvEntity>(&sqlDatabase);
            if(error.isValid())
            {
                //LOG_ERROR("sql error: {}", error.databaseText().toUtf8());
                return false;
            }
        }

        return true;
    }
}
