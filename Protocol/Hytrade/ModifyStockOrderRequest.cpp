#include "ModifyStockOrderRequest.h"
#include "Core/GlobalData.h"

namespace Net
{
    ModifyStockOrderRequest::ModifyStockOrderRequest(QObject *parent)
        : StockAppRequest(parent)
    {
    }

    ModifyStockOrderRequest::~ModifyStockOrderRequest()
    {}

    void ModifyStockOrderRequest::SetOperate(int operate)
    {
        m_operate = operate;
    }

    void ModifyStockOrderRequest::SetOrderNo(const QString& orderNo)
    {
        m_orderNo = orderNo;
    }

    void ModifyStockOrderRequest::SetOrderUuid(const QString& orderUuid)
    {
        m_orderUuid = orderUuid;
    }

    void ModifyStockOrderRequest::SetStockOrderDetail(const QJsonObject& stockOrderDetail)
    {
        m_stockOrderDetail = stockOrderDetail;
    }

    void ModifyStockOrderRequest::MakeJsonParam(QJsonObject& val)
    {
        ASSIGN_FROM_INT32(val, "operate", m_operate);
        ASSIGN_FROM_STRING(val, "orderNo", m_orderNo);
        ASSIGN_FROM_STRING(val, "orderUuid", m_orderUuid);
        ASSIGN_FROM_OBJECT(val, "stockOrderDetail", m_stockOrderDetail);
    }
}
