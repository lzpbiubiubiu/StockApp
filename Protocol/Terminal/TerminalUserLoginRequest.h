#pragma once

#include "Protocol/StockAppRequest.h"

namespace Net
{
    /** 登录请求，${domain}/stockApp/terminal/userLogin */
    class TerminalUserLoginRequest : public StockAppRequest
    {
    public:
        TerminalUserLoginRequest(QObject* parent = Q_NULLPTR);
        ~TerminalUserLoginRequest();

        void SetAccount(const QString& account);
        void SetPassword(const QString& password);
    protected:
        virtual void MakeJsonParam(QJsonObject& val) override;

    private:

        // 账号
        QString m_account;

        // 密码
        QString m_password;
    };
}
