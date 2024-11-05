#include "TerminalUserLoginResponse.h"

namespace Net
{
    TerminalUserLoginResponse::TerminalUserLoginResponse(QObject *parent)
        : StockAppResponse(parent)
    {
    }

    TerminalUserLoginResponse::~TerminalUserLoginResponse()
    {}

    void TerminalUserLoginResponse::ParseJsonParam(const QJsonObject& val)
    {
        QJsonObject userInfoObj = val["userInfo"].toObject();
        ASSIGN_TO_STRING(userInfoObj, "name", m_result.userInfo.name);
        ASSIGN_TO_STRING(userInfoObj, "account", m_result.userInfo.account);
        ASSIGN_TO_STRING(userInfoObj, "password", m_result.userInfo.password);
        ASSIGN_TO_STRING(userInfoObj, "tel", m_result.userInfo.tel);
        ASSIGN_TO_INT32(userInfoObj, "status", m_result.userInfo.status);
    }

    const TerminalUserLoginResponse::Result& TerminalUserLoginResponse::GetResult() const
    {
        return m_result;
    }

}
