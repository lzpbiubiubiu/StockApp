#include "StockAppHeartbeatRequest.h"

namespace Net
{
    StockAppHeartbeatRequest::StockAppHeartbeatRequest(QObject *parent)
        : StockAppRequest(parent)
    {
    }

    StockAppHeartbeatRequest::~StockAppHeartbeatRequest()
    {}

    void StockAppHeartbeatRequest::SetTimestamp(qint64 timestamp)
    {
        m_timestamp = timestamp;
    }

    void StockAppHeartbeatRequest::MakeJsonParam(QJsonObject& val)
    {
        ASSIGN_FROM_INT64(val, "timestamp", m_timestamp);
    }
}
