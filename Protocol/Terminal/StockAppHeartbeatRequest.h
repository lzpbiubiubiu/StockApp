#pragma once

#include "Protocol/StockAppRequest.h"

namespace Net
{
    /** 心跳请求，${domain}/stockApp/terminal/heartbeat */
    class StockAppHeartbeatRequest : public StockAppRequest
    {
    public:
        StockAppHeartbeatRequest(QObject* parent = Q_NULLPTR);
        ~StockAppHeartbeatRequest();

        void SetTimestamp(qint64 timestamp);
    protected:
        virtual void MakeJsonParam(QJsonObject& val) override;

    private:

        // 心跳时间戳
        qint64 m_timestamp = 0;
    };
}
