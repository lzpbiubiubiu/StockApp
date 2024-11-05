#include <QTimer>
#include <QJsonObject>
#include "LoginPageController.h"
#include "Base/Log/Log.h"
#include "Core/GlobalData.h"
#include "Base/Common/ServiceManager.h"

// 检查页面是否不可见
#define CHECK_PAGE_NON_VISIBLE()   \
    do                             \
    {                              \
        if(GetPage()->IsVisible()) \
            return;                \
    } while(0)

namespace UI
{
    LoginPageController::LoginPageController(Page* parent) : PageController(parent)
    {
        m_page = GetPage<LoginPage>();

        m_bussinessMag = Base::GetService<Core::BusinessManager>();
        connect(m_bussinessMag, &Core::BusinessManager::signalLoginSuccess, this, &LoginPageController::onLoginSuccess);
        connect(m_bussinessMag, &Core::BusinessManager::signalLoginError, this, &LoginPageController::onLoginError);
        connect(m_bussinessMag, &Core::BusinessManager::signalRestoreUserAccount, this, &LoginPageController::onRestoreUserAccount);
    }

    LoginPageController::~LoginPageController() { }

    void LoginPageController::login(const QString& account, const QString& password)
    {
        m_page->SetLoginEnabled(false);
        m_bussinessMag->Login(account, password);
    }

    void LoginPageController::editAppIp()
    {
        const auto& app = Core::GlobalData::Get()->GetConfig().app;
        m_page->ShowSettingPanel(app.ip, app.port);
    }

    void LoginPageController::setAppIp(const QString& ip, int port)
    {
        auto businessMannager = Base::GetService<Core::BusinessManager>();
        businessMannager->UpdateAppCache(ip, port);
    }

    void LoginPageController::OnPageLoad()
    {
        // 加载开机启动服务
        Base::ServiceManager::Get()->Load(Base::ServiceManager::StartupOption::BOOT);
    }

    void LoginPageController::OnPageShow()
    {}

    void LoginPageController::OnPageHide() { }

    void LoginPageController::OnPageUnload() { }

    void LoginPageController::onLoginError(const QString& message)
    {
        m_page->SetLoginEnabled(true);
        UI::Application::GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
    }

    void LoginPageController::onLoginSuccess()
    {
        m_page->SetLoginEnabled(true);

        Core::GlobalData::Get()->SetValue("userAccount", Core::GlobalData::Get()->GetUser().account);

        // 跳转主页面
        UI::Application::GetMainWindow()->LoadPage("mainPage");
    }

    void LoginPageController::onRestoreUserAccount(const QString& account)
    {
        m_page->SetLastUserAccount(account);
    }
}
