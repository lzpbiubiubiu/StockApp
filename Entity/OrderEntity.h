#pragma once

#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
 #undef GetUserName
#endif

#include "Base/Database/Base/EntityBase.h"

#define ORDER_TABLE_VERSION_V1 (1)
//#define ORDER_TABLE_VERSION_V2 (2)
//#define ORDER_TABLE_VERSION_V3 (3)

namespace Sql
{
    /** [tb_order] 订单表  */
    class OrderEntity : public Sql::EntityBase
    {
        Q_OBJECT

        ORM_REGISTER_FRIEND_CLASS(OrderEntity)

        ORM_REGISTER_TABLE("tb_order")

        // 订单UUID
        ORM_REGISTER_COLUMN(QString, Uuid, "uuid")

        // 订单小票号
        ORM_REGISTER_COLUMN(QString, ReceiptNo, "receipt_no")

        // 订单金额
        ORM_REGISTER_COLUMN(qint64, OrderAmount, "order_amount")

        // 订单促销金额
        ORM_REGISTER_COLUMN(qint64, PromotionAmount, "promotion_amount")

        // 订单整单促销金额
        ORM_REGISTER_COLUMN(qint64, WholeOrderPromoAmount, "whole_order_promo_amount")

        // 订单类型
        ORM_REGISTER_COLUMN(QString, Type, "type")

        // 订单状态
        ORM_REGISTER_COLUMN(QString, Status, "Status")

        // 扩展字段
        ORM_REGISTER_COLUMN(QString, Extension, "extension")

        // 下单时间（本地时间）(加购第一件商品的时间)
        ORM_REGISTER_COLUMN(QDateTime, Time, "time")
    };

    using OrderEntityPtr = QSharedPointer<OrderEntity>;
    using OrderEntityPtrList = QList<QSharedPointer<OrderEntity>>;
}

using namespace Sql;
