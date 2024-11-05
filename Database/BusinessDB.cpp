#include "BusinessDB.h"
#include "Entity/VersionEntity.h"
#include "Entity/OrderEntity.h"
#include "Entity/WareItemEntity.h"
#include "Base/Database/Orm/QdOrmDao.h"

namespace Sql
{
    bool BusinessDB::CreateTable()
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

        // 创建tb_order表
        if(!tables.contains(OrderEntity::GetTableName()))
        {
            QSqlError error = Orm::create_table<OrderEntity>(&sqlDatabase);
            QString s = error.databaseText();
            QString s1 = error.driverText();
            if(error.isValid())
            {
                //LOG_ERROR("sql error: {}", error.databaseText().toUtf8());
                return false;
            }
        }

        // 创建tb_ware_item表
        if(!tables.contains(WareItemEntity::GetTableName()))
        {
            QSqlError error = Orm::create_table<WareItemEntity>(&sqlDatabase);
            if(error.isValid())
            {
                //LOG_ERROR("sql error: {}", error.databaseText().toUtf8());
                return false;
            }
        }

        return true;
    }
}
