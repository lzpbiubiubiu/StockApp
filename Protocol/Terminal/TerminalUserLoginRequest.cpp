#include "TerminalUserLoginRequest.h"

namespace Net
{
    TerminalUserLoginRequest::TerminalUserLoginRequest(QObject *parent)
        : StockAppRequest(parent)
    {
    }

    TerminalUserLoginRequest::~TerminalUserLoginRequest()
    {}


    void TerminalUserLoginRequest::SetAccount(const QString& account)
    {
        m_account = account;
    }
    void TerminalUserLoginRequest::SetPassword(const QString& password)
    {
        m_password = password;
    }

    void TerminalUserLoginRequest::MakeJsonParam(QJsonObject& val)
    {
        ASSIGN_FROM_STRING(val, "account", m_account);
        ASSIGN_FROM_STRING(val, "password", m_password);
    }
}
