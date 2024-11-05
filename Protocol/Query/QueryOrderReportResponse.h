#pragma once

#include "Protocol/StockAppResponse.h"
#include "Core/StackStruct.h"

namespace Net
{
    /** 查询订单报表返回 */
    class QueryOrderReportResponse : public Net::StockAppResponse
    {
        Q_OBJECT
    public:

        struct OrderReportDetail
        {
            /** 日期*/
            QString date;

            /** 总金额*/
            qint64 totalAmount = 0;
        };

        struct Result
        {
            // 请求唯一id
            QString uniqueId;

            // 订单报表列表数据
            QList<OrderReportDetail> orderReports;
        };

        QueryOrderReportResponse(QObject* parent = nullptr);
        ~QueryOrderReportResponse();

        const Result& GetResult() const;

    protected:
        
        virtual void ParseJsonParam(const QJsonObject& val) override;

    private:
        Result m_result;
    };
}
