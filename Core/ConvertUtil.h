#pragma once

#include "Core/StackStruct.h"
#include "Entity/OrderEntity.h"
#include "Entity/WareItemEntity.h"
namespace Core
{
    /** 数据类型转换工具类 */
    class ConvertUtil
    {

    public:

        // Core::Order <=> Sql::OrderEntity
        static Sql::OrderEntityPtr ToEntity(Core::OrderPtr order);
        static Sql::OrderEntityPtrList ToEntityList(const Core::OrderPtrList& orders);
        static Core::OrderPtr FromEntity(Sql::OrderEntityPtr entity);
        static Core::OrderPtrList FromEntityList(const Sql::OrderEntityPtrList& entities);

        // Sql::WareEntity <=> Core::WareItem
        static Sql::WareItemEntityPtr ToEntity(Core::WareItemPtr item);
        static Sql::WareItemEntityPtrList ToEntityList(const Core::WareItemPtrList& items);
        static Core::WareItemPtr FromEntity(Sql::WareItemEntityPtr entity);
        static Core::WareItemPtrList FromEntityList(const Sql::WareItemEntityPtrList& entities);

        /** 根据配置转换时间格式 */
        static QString FormatDatetime(qint64 dateTime);
        static QString FormatDatetime(const QDateTime& dateTime = QDateTime());

        static QString FormatDate(qint64 dateTime);
        static QString FormatDate(const QDateTime& dateTime = QDateTime());

        /** 获取年月日格式 */
        static QString GetFormatDate();
    };
}
