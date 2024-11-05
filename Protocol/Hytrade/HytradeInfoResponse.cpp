#include "HytradeInfoResponse.h"


namespace Net
{
    HytradeInfoResponse::HytradeInfoResponse(QObject* parent)
        : StockAppResponse(parent)
    {}

    HytradeInfoResponse::~HytradeInfoResponse()
    {}

    const HytradeInfoResponse::Result& HytradeInfoResponse::GetResult() const
    {
        return m_result;
    }

    void HytradeInfoResponse::ParseJsonParam(const QJsonObject& val)
    {
        ASSIGN_TO_STRING(val, "uniqueId", m_result.uniqueId);
    }
}
