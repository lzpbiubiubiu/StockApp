#include "Base/Util/JsonUtil.h"
#include "ConvertUtil.h"

namespace Core
{
    Sql::OrderEntityPtr ConvertUtil::ToEntity(Core::OrderPtr order)
    {
        auto entity = Sql::OrderEntityPtr::create();
        entity->SetUuid(order->uuid);
        entity->SetReceiptNo(order->receiptNo);
        entity->SetOrderAmount(order->orderAmount);
        entity->SetPromotionAmount(order->promotionAmount);
        entity->SetWholeOrderPromoAmount(order->wholeOrderPromoAmount);
        entity->SetTime(order->time);
        QMetaEnum metaEnum = QMetaEnum::fromType<Core::Order::OrderType>();
        QString type = metaEnum.valueToKey(order->type);
        Q_ASSERT(!type.isEmpty());
        entity->SetType(type);

        metaEnum = QMetaEnum::fromType<Core::Order::OrderStatus>();
        QString status = metaEnum.valueToKey(order->status);
        Q_ASSERT(!status.isEmpty());
        entity->SetStatus(status);
        entity->SetExtension(QString(QJsonDocument(order->extension).toJson(QJsonDocument::Compact)));
        return entity;
    }

    Sql::OrderEntityPtrList ConvertUtil::ToEntityList(const Core::OrderPtrList& orders)
    {
        Sql::OrderEntityPtrList list;
        for(auto i : orders)
            list << ToEntity(i);
        return list;
    }

    Core::OrderPtr ConvertUtil::FromEntity(Sql::OrderEntityPtr entity)
    {
        auto order = Core::OrderPtr::create();
        order->uuid = entity->GetUuid();
        order->receiptNo = entity->GetReceiptNo();
        order->orderAmount = entity->GetOrderAmount();
        order->promotionAmount = entity->GetPromotionAmount();
        order->wholeOrderPromoAmount = entity->GetWholeOrderPromoAmount();
        order->time = entity->GetTime();

        QMetaEnum metaEnum = QMetaEnum::fromType<Core::Order::OrderType>();
        int type = metaEnum.keyToValue(entity->GetType().toUtf8());
        Q_ASSERT(type != -1);
        order->type = static_cast<Core::Order::OrderType>(type);

        metaEnum = QMetaEnum::fromType<Core::Order::OrderStatus>();
        int value = metaEnum.keyToValue(entity->GetStatus().toUtf8());
        Q_ASSERT(value != -1);
        order->status = static_cast<Core::Order::OrderStatus>(value);

        QJsonParseError error;
        QJsonDocument extension = QJsonDocument::fromJson(entity->GetExtension().toUtf8(), &error);
        if(error.error == QJsonParseError::NoError)
        {
            if(extension.isObject())
            {
                order->extension = extension.object();
            }
        }
        return order;
    }

    Core::OrderPtrList ConvertUtil::FromEntityList(const Sql::OrderEntityPtrList& entities)
    {
        Core::OrderPtrList list;
        for(auto i : entities)
            list << FromEntity(i);
        return list;
    }

    Sql::WareItemEntityPtr ConvertUtil::ToEntity(Core::WareItemPtr item)
    {
        auto entity = Sql::WareItemEntityPtr::create();
        entity->SetUuid(item->uuid);
        entity->SetOrderUuid(item->orderUuid);
        entity->SetCode(item->code);
        entity->SetName(item->name);
        entity->SetCount(item->count);
        entity->SetAmount(item->amount);
        entity->SetPrice(item->price);
        entity->SetDiscountPrice(item->discountPrice);
        entity->SetPromotionAmount(item->promotionAmount);
        entity->SetStock(item->stock);
        QMetaEnum saleModeMetaEnum = QMetaEnum::fromType<Core::WareItem::WareType>();
        item->extension["wareType"] = saleModeMetaEnum.valueToKey(item->wareType);
        item->extension["retailPrice"] = item->retailPrice;
        item->extension["wholesalePrice"] = item->wholesalePrice;
        entity->SetExtension(QString(QJsonDocument(item->extension).toJson(QJsonDocument::Compact)));
        return entity;
    }

    Core::WareItemPtr ConvertUtil::FromEntity(Sql::WareItemEntityPtr entity)
    {
        auto item = Core::WareItemPtr::create();
        item->uuid = entity->GetUuid();
        item->orderUuid = entity->GetOrderUuid();
        item->code = entity->GetCode();
        item->name = entity->GetName();
        item->count = entity->GetCount();
        item->amount = entity->GetAmount();
        item->price = entity->GetPrice();
        item->discountPrice = entity->GetDiscountPrice();
        item->promotionAmount = entity->GetPromotionAmount();
        item->stock = entity->GetStock();
        item->extension = Base::JsonUtil::ToJsonObject(entity->GetExtension());
        QMetaEnum saleModeMetaEnum = QMetaEnum::fromType<Core::WareItem::WareType>();
        item->wareType = static_cast<Core::WareItem::WareType>(saleModeMetaEnum.keyToValue(item->extension["wareType"].toString().toUtf8()));
        item->retailPrice = item->extension["retailPrice"].toVariant().toLongLong();
        item->wholesalePrice = item->extension["wholesalePrice"].toVariant().toLongLong();
        return item;
    }

    Sql::WareItemEntityPtrList ConvertUtil::ToEntityList(const Core::WareItemPtrList& items)
    {
        Sql::WareItemEntityPtrList list;
        for(auto i : items)
            list << ToEntity(i);
        return list;
    }

    Core::WareItemPtrList ConvertUtil::FromEntityList(const Sql::WareItemEntityPtrList& entities)
    {
        Core::WareItemPtrList list;
        for(auto i : entities)
            list << FromEntity(i);
        return list;
    }

    QString ConvertUtil::FormatDatetime(qint64 time)
    {

        return QDateTime::fromMSecsSinceEpoch(time).toString("yyyy-MM-dd HH:mm:ss");
    }

    QString ConvertUtil::FormatDatetime(const QDateTime& time)
    {
        QDateTime dateTime = time;
        if(dateTime.isNull())
        {
            dateTime = QDateTime::currentDateTime();
        }

        return dateTime.toString("yyyy-MM-dd HH:mm:ss");
    }

    QString ConvertUtil::FormatDate(qint64 dateTime)
    {
        return QDateTime::fromMSecsSinceEpoch(dateTime).toString(GetFormatDate());
    }

    QString ConvertUtil::FormatDate(const QDateTime& dateTime)
    {
        return dateTime.toString(GetFormatDate());
    }

    QString ConvertUtil::GetFormatDate()
    {
        QString format = "yyyy-MM-dd HH:mm:ss";
        QString dateFormat;
        if (format.indexOf("y") < format.indexOf("d"))
            dateFormat = format.mid(format.indexOf("y"), format.lastIndexOf("d") - format.indexOf("y") + 1);
        else
            dateFormat = format.mid(format.indexOf("d"), format.lastIndexOf("y") - format.indexOf("d") + 1);
        return dateFormat;
    }
}
