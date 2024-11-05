#pragma once

#include <QObject>
#include <QString>
#include <QList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QSharedPointer>

namespace Core
{
    /** 库存商品 */
    struct WareItem
    {
        enum WareType
        {
            /** 普通商品 */
            WARE_TYPE_NORMAL = 1,

            /** 赠品 */
            WARE_TYPE_GIFT = 2
        };
        Q_ENUM(WareType)

        /** UUID */
        QString uuid;

        /** 名称 */
        QString name;

        /** 商品编码 */
        QString code;

        /** 订单UUID */
        QString orderUuid;

        /** 数量、份数 */
        int count = 0;

        /** 库存 */
        qint64 stock = 0;

        /** 小计金额 */
        qint64 amount = 0;

        /** 单品价格 */
        qint64 price = 0;

        /** 单品零售价格 */
        qint64 retailPrice = 0;

        /** 单品批发价格 */
        qint64 wholesalePrice = 0;

        /** 执行价 */
        qint64 discountPrice = 0;

        /** 单品促销金额 */
        qint64 promotionAmount = 0;

        /** 商品类型 */
        WareType wareType = WareType::WARE_TYPE_NORMAL;

        /** 扩展字段 */
        QJsonObject extension;

        Q_GADGET
    };
    using WareItemPtr = QSharedPointer<WareItem>;
    using WareItemPtrList = QList<QSharedPointer<WareItem>>;

    /** 订单*/
    struct Order
    {
        enum OrderType
        {
            /** 零售 */
            RETAIL = 1,

            /** 批发 */
            WHOLESALE = 2
        };
        Q_ENUM(OrderType)

        /** 订单状态 */
        enum OrderStatus
        {
            /** 订单创建 */
            CREATE = 1,

            /** 订单完成 */
            FINISH = 2
        };
        Q_ENUM(OrderStatus)

        /** UUID */
        QString uuid;

        /** 商品列表 */
        WareItemPtrList items;

        /** 商品品数 */
        int kind = 0;

        /** 商品件数 */
        int piece = 0;

        /** 订单金额*/
        qint64 orderAmount = 0;

        /** 单品促销金额*/
        qint64 promotionAmount = 0;

        /** 整单促销金额 */
        qint64 wholeOrderPromoAmount = 0;

        /** 小票号 */
        QString receiptNo;

        /** 订单类型 */
        OrderType type = RETAIL;

        /** 订单状态 */
        OrderStatus status = CREATE;

        /** 订单创建 */
        QDateTime time;

        /** 扩展字段 */
        QJsonObject extension;

        Q_GADGET
    };
    using OrderPtr = QSharedPointer<Order>;
    using OrderPtrList = QList<QSharedPointer<Order>>;

    /** 用户 */
    struct User
    {
        /** 用户状态 */
        enum Status
        {
            /** 正常 */
            NORMAL = 1,

            /** 超级 */
            SUPER = 2
        };

        /** 账号 */
        QString account;

        /** 密码（仅在内存中,不保存至数据库）*/
        QString pwd;

        /** 姓名 */
        QString name;

        /** 电话 */
        QString tel;

        /** 用户状态 */
        Status status = NORMAL;

        Q_GADGET
    };

    using UserPtr = QSharedPointer<User>;
    using UserPtrList = QList<QSharedPointer<User>>;


    /** 订单报表*/
    struct OrderReport
    {
        /** 报表日期 */
        QString date;

        /** 扩展字段 */
        qint64 amount = 0;

        Q_GADGET
    };
    using OrderReportPtr = QSharedPointer<OrderReport>;
    using OrderReportPtrList = QList<QSharedPointer<OrderReport>>;

}
Q_DECLARE_METATYPE(Core::WareItem)
Q_DECLARE_METATYPE(Core::WareItemPtr)
Q_DECLARE_METATYPE(Core::WareItemPtrList)
Q_DECLARE_METATYPE(Core::Order)
Q_DECLARE_METATYPE(Core::OrderPtr)
Q_DECLARE_METATYPE(Core::OrderPtrList)
Q_DECLARE_METATYPE(Core::User)
Q_DECLARE_METATYPE(Core::UserPtr)
Q_DECLARE_METATYPE(Core::UserPtrList)
Q_DECLARE_METATYPE(Core::OrderReport)
Q_DECLARE_METATYPE(Core::OrderReportPtr)
Q_DECLARE_METATYPE(Core::OrderReportPtrList)
