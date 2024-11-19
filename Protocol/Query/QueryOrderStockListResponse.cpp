#include "QueryOrderStockListResponse.h"
#include "Base/Util/JsonUtil.h"
namespace Net
{
    QueryOrderStockListResponse::QueryOrderStockListResponse(QObject* parent)
        : StockAppResponse(parent)
    {}

    QueryOrderStockListResponse::~QueryOrderStockListResponse()
    {}

    const QueryOrderStockListResponse::Result& QueryOrderStockListResponse::GetResult() const
    {
        return m_result;
    }

    void QueryOrderStockListResponse::ParseJsonParam(const QJsonObject& val)
    {
        ASSIGN_TO_STRING(val, "uniqueId", m_result.uniqueId);
        auto stockingOrderArr = val["stockingOrders"].toArray();
        for(auto item : stockingOrderArr)
        {
            QJsonObject obj = item.toObject();
            StockOrderDetail stockOrderDetail;
            ASSIGN_TO_STRING(obj, "orderNo", stockOrderDetail.orderNo);
            ASSIGN_TO_STRING(obj, "orderUuid", stockOrderDetail.orderUuid);
            ASSIGN_TO_STRING(obj, "time", stockOrderDetail.time);
            ASSIGN_TO_INT64(obj, "orderAmount", stockOrderDetail.orderAmount);
            ASSIGN_TO_INT64(obj, "totalPromotionAmount", stockOrderDetail.totalPromotionAmount);
            ASSIGN_TO_INT64(obj, "deliveryFreeAmount", stockOrderDetail.deliveryFreeAmount);
            ASSIGN_TO_STRING(obj, "extension", stockOrderDetail.extension);
            for(auto wareItem : obj["wares"].toArray())
            {
                QJsonObject wareObj = wareItem.toObject();
                StockOrderDetail::StockOrderWare ware;
                ASSIGN_TO_STRING(wareObj, "name", ware.name);
                ASSIGN_TO_STRING(wareObj, "code", ware.code);
                ASSIGN_TO_INT64(wareObj, "amount", ware.amount);
                ASSIGN_TO_INT64(wareObj, "discountPrice", ware.discountPrice);
                ASSIGN_TO_INT64(wareObj, "count", ware.count);
                ASSIGN_TO_INT64(wareObj, "price", ware.price);
                ASSIGN_TO_INT64(wareObj, "promotionAmount", ware.promotionAmount);
                ASSIGN_TO_BOOL(wareObj, "gifts", ware.gifts);
                ware.extension = Base::JsonUtil::ToString(wareObj["extension"].toObject());
                stockOrderDetail.wares.append(ware);
            }
            m_result.stockingOrders.append(stockOrderDetail);
        }
    }
}
