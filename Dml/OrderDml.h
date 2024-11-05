#pragma once

#include "Base/Database/Base/DmlBase.h"
#include "Core/StackStruct.h"
#include "Entity/OrderEntity.h"
#include "Entity/WareItemEntity.h"

namespace Sql
{
    /** 订单数据持久化辅助类 */
    class OrderDml : public QObject
    {
        Q_OBJECT
    public:
        OrderDml(QObject* parent = nullptr);
        ~OrderDml();

        /** 设置目标数据库 */
        void SetDatabase(QSharedPointer<DBBase> database);

        /** 新增订单 */
        bool AddOrder(Core::OrderPtr order, QString& sqlError);

        /** 更新订单 */
        bool UpdateOrder(Core::OrderPtr order, QString& sqlError);

        /** 删除订单 */
        bool DeleteOrder(const QString& uuid, QString& sqlError);

        /** 删除所有订单 */
        bool ClearOrders(QString& sqlError);

        /** 获取所有订单 */
        bool ListOrders(Core::OrderPtrList& orders, QString& sqlError);

        /** 查找订单（UUID匹配） */
        bool FindOrderByUuid(Core::OrderPtr& order, const QString& uuid, QString& sqlError);

        /** 查找订单 (receiptNo匹配) */
        bool FindOrderByReceiptNo(Core::OrderPtr& order, const QString& receiptNo, QString& sqlError);

        /** 新增商品 */
        bool AddItem(Core::WareItemPtr item, QString& sqlError);
        
        /** 新增商品 */
        bool AddItemList(const Core::WareItemPtrList& items, QString& sqlError);
        
        /** 更新商品 */
        bool UpdateItem(Core::WareItemPtr item, QString& sqlError);
        
        /** 更新商品 */
        bool UpdateItemList(const Core::WareItemPtrList& items, QString& sqlError);
 
        /** 删除商品 */
        bool DeleteItem(const QString& uuid, QString& sqlError);
        
        /** 清空所有商品 */
        bool ClearItems(QString& sqlError);
        
        /** 查询商品（订单UUID匹配） */
        bool FindItemByOrderUuid(Core::WareItemPtrList& items, const QString& orderUuid, bool asc, QString& sqlError);
   
        /** 删除商品（订单UUID匹配） */
        bool DeleteItemByOrderUuid(const QString& orderUuid, QString& sqlError);

    private:
        Sql::DmlBase<Sql::OrderEntity> m_dmlOrder;
        Sql::DmlBase<Sql::WareItemEntity> m_dmlWareItem;
    };
}
