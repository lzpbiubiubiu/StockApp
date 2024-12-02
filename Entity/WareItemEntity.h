#pragma once
#include "Base/Database/Base/EntityBase.h"

#define WARE_ITEM_TABLE_VERSION_V1 (1)
//#define WARE_ITEM_TABLE_VERSION_V2 (2)
//#define WARE_ITEM_TABLE_VERSION_V3 (3)

namespace Sql
{
    /** [tb_ware_item] 商品表  */
    class WareItemEntity : public Sql::EntityBase
    {
        Q_OBJECT

        ORM_REGISTER_FRIEND_CLASS(WareItemEntity)

        ORM_REGISTER_TABLE("tb_ware_item")

        // UUID
        ORM_REGISTER_COLUMN(QString, Uuid, "uuid")

        // 订单UUID
        ORM_REGISTER_COLUMN(QString, OrderUuid, "order_uuid")

        // 商品编码
        ORM_REGISTER_COLUMN(QString, Code, "code")

        // 名称
        ORM_REGISTER_COLUMN(QString, Name, "name")

        // 数量、份数
        ORM_REGISTER_COLUMN(int, Count, "count")

        // 商品剩余库存
        ORM_REGISTER_COLUMN(qint64, Stock, "stock")

        // 小计金额
        ORM_REGISTER_COLUMN(qint64, Amount, "amount")

        // 单品原价
        ORM_REGISTER_COLUMN(qint64, Price, "price")

        // 单品促销价
        ORM_REGISTER_COLUMN(qint64, DiscountPrice, "discount_price")

        // 促销金额
        ORM_REGISTER_COLUMN(qint64, PromotionAmount, "promotion_amount")

        // 商品图片url
        ORM_REGISTER_COLUMN(QString, ImageUrl, "image_url")

        // 促销图片md5
        ORM_REGISTER_COLUMN(QString, ImageMd5, "image_md5")

        // 扩展字段（通常是JSON格式字符串）
        ORM_REGISTER_COLUMN(QString, Extension, "extension")
    };

    using WareItemEntityPtr = QSharedPointer<WareItemEntity>;
    using WareItemEntityPtrList = QList<QSharedPointer<WareItemEntity>>;
}

using namespace Sql;
