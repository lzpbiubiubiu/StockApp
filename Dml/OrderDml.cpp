#include "Base/Common/System.h"
#include "Base/Common/ServiceManager.h"
#include "Core/DatabaseManager.h"
#include "Core/ConvertUtil.h"
#include "OrderDml.h"

namespace Sql
{
    OrderDml::OrderDml(QObject* parent) : QObject(parent) { }

    OrderDml::~OrderDml() { }

    void OrderDml::SetDatabase(QSharedPointer<DBBase> database)
    {
        m_dmlOrder.SetDatabase(database);
        m_dmlWareItem.SetDatabase(database);
    }

    bool OrderDml::AddOrder(Core::OrderPtr order, QString& sqlError)
    {
        auto entity = Core::ConvertUtil::ToEntity(order);
        return m_dmlOrder.Add(entity, sqlError);
    }

    bool OrderDml::UpdateOrder(Core::OrderPtr order, QString& sqlError)
    {
        auto entity = Core::ConvertUtil::ToEntity(order);
        return m_dmlOrder.UpdateById(entity, QStringList(), sqlError);
    }

    bool OrderDml::DeleteOrder(const QString& uuid, QString& sqlError)
    {
        auto query = Orm::QdOrmWhere();
        query.where(OrderEntity::ColumnUuid()).isEqualTo(uuid);
        return m_dmlOrder.DeleteByQuery(query, sqlError);
    }

    bool OrderDml::ClearOrders(QString& sqlError)
    {
        return m_dmlOrder.DeleteAll(sqlError);
    }

    bool OrderDml::ListOrders(Core::OrderPtrList& orders, QString& sqlError)
    {
        Sql::OrderEntityPtrList entities;
        bool result = m_dmlOrder.ListAll(entities, sqlError);
        if(result && !entities.isEmpty())
            orders = Core::ConvertUtil::FromEntityList(entities);
        return result;
    }

    bool OrderDml::FindOrderByUuid(Core::OrderPtr& order, const QString& uuid, QString& sqlError)
    {
        Sql::OrderEntityPtrList entities;
        auto query = Orm::QdOrmWhere();
        query.where(OrderEntity::ColumnUuid()).isEqualTo(uuid);
        bool result = m_dmlOrder.FindByQuery(entities, query, sqlError);
        if(result && !entities.isEmpty())
            order = Core::ConvertUtil::FromEntity(entities[0]);
        return result;
    }

    bool OrderDml::FindOrderByReceiptNo(Core::OrderPtr& order, const QString& receiptNo, QString& sqlError)
    {
        Sql::OrderEntityPtrList entities;
        auto query = Orm::QdOrmWhere();
        query.where(OrderEntity::ColumnReceiptNo()).isEqualTo(receiptNo);
        bool result = m_dmlOrder.FindByQuery(entities, query, sqlError);
        if(result && !entities.isEmpty())
            order = Core::ConvertUtil::FromEntity(entities[0]);
        return result;
    }

    bool OrderDml::AddItem(Core::WareItemPtr item, QString& sqlError)
    {
        auto entity = Core::ConvertUtil::ToEntity(item);
        return m_dmlWareItem.Add(entity, sqlError);
    }

    bool OrderDml::AddItemList(const Core::WareItemPtrList& items, QString& sqlError)
    {
        auto entities = Core::ConvertUtil::ToEntityList(items);
        return m_dmlWareItem.AddList(entities, sqlError);
    }


    bool OrderDml::UpdateItem(Core::WareItemPtr item, QString& sqlError)
    {
        auto entity = Core::ConvertUtil::ToEntity(item);
        return m_dmlWareItem.UpdateById(entity, QStringList(), sqlError);
    }

    bool OrderDml::UpdateItemList(const Core::WareItemPtrList& items, QString& sqlError)
    {
        auto entities = Core::ConvertUtil::ToEntityList(items);
        return m_dmlWareItem.UpdateList(entities, sqlError);
    }

    bool OrderDml::DeleteItem(const QString& uuid, QString& sqlError)
    {
        auto query = Orm::QdOrmWhere();
        query.where(WareItemEntity::ColumnUuid()).isEqualTo(uuid);
        return m_dmlWareItem.DeleteByQuery(query, sqlError);
    }

    bool OrderDml::ClearItems(QString& sqlError)
    {
        return m_dmlWareItem.DeleteAll(sqlError);
    }

    bool OrderDml::FindItemByOrderUuid(Core::WareItemPtrList& items, const QString& orderUuid, bool asc, QString& sqlError)
    {
        Sql::WareItemEntityPtrList entities;
        auto query = Orm::QdOrmWhere();

        if(asc)
            query.where(WareItemEntity::ColumnOrderUuid()).isEqualTo(orderUuid).orderAsc("rowid");
        else
            query.where(WareItemEntity::ColumnOrderUuid()).isEqualTo(orderUuid).orderDesc("rowid");

        bool result = m_dmlWareItem.FindByQuery(entities, query, sqlError);
        if(result && !entities.isEmpty())
            items = Core::ConvertUtil::FromEntityList(entities);
        return result;
    }

    bool OrderDml::DeleteItemByOrderUuid(const QString& orderUuid, QString& sqlError)
    {
        auto query = Orm::QdOrmWhere();
        query.where(WareItemEntity::ColumnOrderUuid()).isEqualTo(orderUuid);
        return m_dmlWareItem.DeleteByQuery(query, sqlError);
    }
}
