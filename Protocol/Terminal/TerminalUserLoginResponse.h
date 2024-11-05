#pragma once

#include "Protocol/StockAppResponse.h"

namespace Net
{
    /** 心跳请求，${domain}/stockServer/terminal/userLogin */
    class TerminalUserLoginResponse : public StockAppResponse
    {
        struct UserInfo
        {
            /** 名称 */
            QString name;

            /** 账号 */
            QString account;

            /** 密码 */
            QString password;

            /** 电话 */
            QString tel;

            /** 用户状态1: 正常用户， 2：超级用户 */
            int status = 1;

        };
        struct Result
        {
            /** 用户信息 */
            UserInfo userInfo;
        };

    public:
        TerminalUserLoginResponse(QObject* parent = Q_NULLPTR);
        ~TerminalUserLoginResponse();

        const Result& GetResult() const;
    protected:
        virtual void ParseJsonParam(const QJsonObject& val) override;

    private:
        Result m_result;
    };
}
