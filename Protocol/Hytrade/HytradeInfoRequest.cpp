#include <QJsonObject>
#include "Core/GlobalData.h"
#include "HytradeInfoRequest.h"

namespace Net
{
    HytradeInfoRequest::HytradeInfoRequest(QObject* parent)
        : StockAppRequest(parent)
    {
    }

    HytradeInfoRequest::~HytradeInfoRequest()
    {}

    void HytradeInfoRequest::SetWareList(const Core::WareItemPtrList& wares)
    {
        m_wares = wares;
    }

    void HytradeInfoRequest::SetReceiptNo(const QString& receiptNo)
    {
        m_receiptNo = receiptNo;
    }

    void HytradeInfoRequest::SetOrderAmount(qint64 amount)
    {
        m_orderAmount = amount;
    }

    void HytradeInfoRequest::SetTotalPromotionAmount(qint64 amount)
    {
        m_totalPromotionAmount = amount;
    }

    void HytradeInfoRequest::SetWarePromotionAmount(qint64 amount)
    {
        m_warePromotionAmount = amount;
    }


    void HytradeInfoRequest::SetWholeOrderPromotionAmount(qint64 amount)
    {
        m_wholeOrderPromotionAmount = amount;
    }

    void HytradeInfoRequest::SetDeliveryFeesAmount(qint64 amount)
    {
        m_deliveryFeesAmount = amount;
    }

    void HytradeInfoRequest::SetOrderRemark(const QString& text)
    {
        m_orderRemark = text;
    }

    void HytradeInfoRequest::SetOrderExtension(const QJsonObject& extension)
    {
        m_extension = extension;
    }

    void HytradeInfoRequest::SetOrderType(int type)
    {
        m_type = type;
    }

    QJsonArray HytradeInfoRequest::MakeWareJsonParam(const Core::WareItemPtrList& wares)
    {
        QJsonArray ret_wares;
        for (auto& i : wares)
        {
            QJsonObject obj;
            obj["code"] = i->code;
            obj["count"] = i->count;
            obj["uuid"] = i->uuid;
            obj["price"] = i->price;
            obj["discountPrice"] = i->discountPrice;
            obj["amount"] = i->amount;
            obj["promotionAmount"] = i->promotionAmount;
            obj["name"] = i->name;
            QJsonObject extObj;
            extObj["retailPrice"] = i->retailPrice;
            extObj["wholesalePrice"] = i->wholesalePrice;
            obj["extension"] = extObj;
            ret_wares << obj;
        }

        return ret_wares;
    }

    void HytradeInfoRequest::MakeJsonParam(QJsonObject& val)
    {
        QJsonArray wares = MakeWareJsonParam(m_wares);
        ASSIGN_FROM_ARRAY(val, "wares", wares);
        ASSIGN_FROM_STRING(val, "receiptNo", m_receiptNo);
        ASSIGN_FROM_STRING(val, "userAccount", Core::GlobalData::Get()->GetUser().account);
        ASSIGN_FROM_STRING(val, "userName", Core::GlobalData::Get()->GetUser().name);
        ASSIGN_FROM_INT64(val, "orderAmount", m_orderAmount);
        ASSIGN_FROM_INT64(val, "totalPromotionAmount", m_totalPromotionAmount);
        ASSIGN_FROM_INT32(val, "orderType", m_type);
        QJsonArray promoArr;
        if(0 != m_warePromotionAmount)
        {
            QJsonObject wareProObj;
            wareProObj["promotionName"] = QStringLiteral("单品促销");
            wareProObj["promotionAmount"] = m_warePromotionAmount;
            promoArr.append(wareProObj);
        }

        if(0 != m_wholeOrderPromotionAmount)
        {
            QJsonObject wholeOrderProObj;
            wholeOrderProObj["promotionName"] = QStringLiteral("整单促销");
            wholeOrderProObj["promotionAmount"] = m_wholeOrderPromotionAmount;
            promoArr.append(wholeOrderProObj);
        }
        ASSIGN_FROM_INT64(val, "deliveryFeesAmount", m_deliveryFeesAmount);
        ASSIGN_FROM_ARRAY(val, "promotions", promoArr);
        m_extension["orderRemark"] = m_orderRemark;
        ASSIGN_FROM_OBJECT(val, "extension", m_extension);
        ASSIGN_FROM_STRING(val, "uniqueId", GetTraceNumber());
    }
}
