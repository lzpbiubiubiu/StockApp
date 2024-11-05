#include "LoginPage.h"
#include "Controller/LoginPageController.h"

namespace UI
{
    IMPLEMENT_PAGE("loginPage", LoginPage)

    LoginPage::LoginPage(QObject* parent) : Page(parent)
    {
        SetController(new LoginPageController(this));
        SetContentItem("qrc:/Qml/Page/LoginPage.qml");
    }

    LoginPage::~LoginPage() { }

    bool LoginPage::GetLoginEnabled() const
    {
        return m_loginEnabled;
    }
    void LoginPage::SetLoginEnabled(bool loginEnabled)
    {
        if(m_loginEnabled != loginEnabled)
        {
            m_loginEnabled = loginEnabled;
            emit signalLoginEnabled();
        }
    }

    QString LoginPage::GetLastUserAccount() const
    {
        return m_lastUserAccount;
    }

    void LoginPage::SetLastUserAccount(const QString& account)
    {
        if(m_lastUserAccount != account)
        {
            m_lastUserAccount = account;
            emit signalLastUserAccountChanged();
        }
    }

    void LoginPage::ShowSettingPanel(const QString& ip, int port)
    {
        QMetaObject::invokeMethod(m_item, "showSettingPanel", Q_ARG(QVariant, ip), Q_ARG(QVariant, port));
    }

}
