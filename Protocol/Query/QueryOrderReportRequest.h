#pragma once

#include "Protocol/StockAppRequest.h"
#include "Core/StackStruct.h"
namespace Net
{
    /** 查询订单报表请求，${domain}/stockServer/sale/queryOrderReport */
    class QueryOrderReportRequest : public Net::StockAppRequest
    {
        Q_OBJECT
    public:

        QueryOrderReportRequest(QObject* parent = nullptr);
        ~QueryOrderReportRequest();

        /** 设置查询日期 */
        void SetQueryDate(qint64 dateTime);

        /** 设置查询范围 */
        void SetQueryRange(int range);
    protected:
        virtual void MakeJsonParam(QJsonObject& val) override;

    private:

        /** 查询日期时间戳*/
        qint64 m_dateTime = 0;

        /** 查询范围， 1: 一个月， 2：一周， 3：所有 */
        int m_range = 1;
    };
}
