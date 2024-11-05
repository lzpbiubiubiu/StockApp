#include "QueryOrderReportResponse.h"

namespace Net
{
    QueryOrderReportResponse::QueryOrderReportResponse(QObject* parent)
        : StockAppResponse(parent)
    {}

    QueryOrderReportResponse::~QueryOrderReportResponse()
    {}

    const QueryOrderReportResponse::Result& QueryOrderReportResponse::GetResult() const
    {
        return m_result;
    }

    void QueryOrderReportResponse::ParseJsonParam(const QJsonObject& val)
    {
        ASSIGN_TO_STRING(val, "uniqueId", m_result.uniqueId);
        auto orderReportArr = val["orderReports"].toArray();
        for(auto item : orderReportArr)
        {
            QJsonObject obj = item.toObject();
            OrderReportDetail orderReportDetail;
            ASSIGN_TO_STRING(obj, "date", orderReportDetail.date);
            ASSIGN_TO_INT64(obj, "totalAmount", orderReportDetail.totalAmount);
            m_result.orderReports.append(orderReportDetail);
        }
    }
}
