#include <QJsonObject>
#include "Core/GlobalData.h"
#include "QueryOrderReportRequest.h"

namespace Net
{
    QueryOrderReportRequest::QueryOrderReportRequest(QObject* parent)
        : StockAppRequest(parent)
    {
    }

    QueryOrderReportRequest::~QueryOrderReportRequest()
    {}

    void QueryOrderReportRequest::SetQueryDate(qint64 dateTime)
    {
        m_dateTime = dateTime;
    }

    void QueryOrderReportRequest::SetQueryRange(int range)
    {
        m_range = range;
    }

    void QueryOrderReportRequest::MakeJsonParam(QJsonObject& val)
    {
        ASSIGN_FROM_INT32(val, "queryRange", m_range);
        ASSIGN_FROM_INT64(val, "date", m_dateTime);
        ASSIGN_FROM_STRING(val, "uniqueId", GetTraceNumber());
    }
}
