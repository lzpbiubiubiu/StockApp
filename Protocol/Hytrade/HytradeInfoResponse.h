#pragma once

#include "Protocol/StockAppResponse.h"
#include "Core/StackStruct.h"

namespace Net
{
    /** 结算算价返回 */
    class HytradeInfoResponse : public Net::StockAppResponse
    {
        Q_OBJECT
    public:

        struct Result
        {
            // 请求唯一id，订单维度相同
            QString uniqueId;
        };

        HytradeInfoResponse(QObject* parent = nullptr);
        ~HytradeInfoResponse();

        const Result& GetResult() const;

    protected:
        
        virtual void ParseJsonParam(const QJsonObject& val) override;


    private:
        Result m_result;
    };
}
