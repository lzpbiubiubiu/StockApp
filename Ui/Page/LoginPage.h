#pragma once

#include "Page.h"

namespace UI
{
    class LoginPage : public Page
    {
        Q_OBJECT
        Q_PROPERTY(bool loginEnabled READ GetLoginEnabled WRITE SetLoginEnabled NOTIFY signalLoginEnabled)
        Q_PROPERTY(QString lastUserAccount READ GetLastUserAccount WRITE SetLastUserAccount NOTIFY signalLastUserAccountChanged)
        DECLARE_PAGE()
    public:
        Q_INVOKABLE LoginPage(QObject* parent = nullptr);

        ~LoginPage();

        /** 获取|设置登录按钮使能 */
        bool GetLoginEnabled() const;
        void SetLoginEnabled(bool loginEnabled);

        /** 设置|获取上一次登录的用户账号 */
        QString GetLastUserAccount() const;
        void SetLastUserAccount(const QString& account);

        /** 设置IP地址和端口弹出层 */
        void ShowSettingPanel(const QString& ip, int port);
    Q_SIGNALS:
        void signalLoginEnabled();
        void signalLastUserAccountChanged();

    private:

        // 登录使能
        bool m_loginEnabled = true;

        // 上一次登录的用户账号
        QString m_lastUserAccount;
    };
}
