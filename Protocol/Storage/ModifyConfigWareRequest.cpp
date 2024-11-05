#include "ModifyConfigWareRequest.h"
#include "Core/GlobalData.h"

namespace Net
{
    ModifyConfigWareRequest::ModifyConfigWareRequest(QObject *parent)
        : StockAppRequest(parent)
    {
    }

    ModifyConfigWareRequest::~ModifyConfigWareRequest()
    {}


    void ModifyConfigWareRequest::SetCode(const QString& code)
    {
        m_wareCode = code;
    }

    void ModifyConfigWareRequest::SetName(const QString& name)
    {
        m_wareName = name;
    }

    void ModifyConfigWareRequest::SetStock(qint64 stock)
    {
        m_stock = stock;
    }

    void ModifyConfigWareRequest::SetRetailPrice(qint64 price)
    {
        m_retailPrice = price;
    }

    void ModifyConfigWareRequest::SetWholeSalePrice(qint64 price)
    {
        m_wholeSalePrice = price;
    }

    void ModifyConfigWareRequest::SetOperate(int operate)
    {
        m_operate = operate;
    }

    void ModifyConfigWareRequest::MakeJsonParam(QJsonObject& val)
    {
        ASSIGN_FROM_STRING(val, "userAccount", Core::GlobalData::Get()->GetUser().account);
        ASSIGN_FROM_STRING(val, "userName", Core::GlobalData::Get()->GetUser().name);
        ASSIGN_FROM_STRING(val, "wareCode", m_wareCode);
        ASSIGN_FROM_STRING(val, "wareName", m_wareName);
        ASSIGN_FROM_INT64(val, "retailPrice", m_retailPrice);
        ASSIGN_FROM_INT64(val, "wholeSalePrice", m_wholeSalePrice);
        ASSIGN_FROM_INT32(val, "operate", m_operate);
        if(1 == m_operate)
            ASSIGN_FROM_INT64(val, "stock", m_stock);
    }
}
