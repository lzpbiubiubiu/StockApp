#pragma once

#include "PageController.h"
#include "Ui/Page/LoginPage.h"
#include "Core/BusinessManager.h"
namespace UI
{
    /** 登录页控制器 */
    class LoginPageController : public PageController
    {
        Q_OBJECT

    public:
        LoginPageController(Page* parent);
        //edit by luyong
        virtual ~LoginPageController();

        /** 收银员登录 */
        Q_INVOKABLE void login(const QString& account, const QString& password);

        /** 编辑ip地址 */
        Q_INVOKABLE void editAppIp();

        /** 导出全局JS函数设置ip */
        Q_INVOKABLE void setAppIp(const QString& ip, int port);

    protected:
        /** 页面加载时回调 */
        virtual void OnPageLoad() override;

        /** 页面显示前回调 */
        virtual void OnPageShow() override;

        /** 页面隐藏前回调 */
        virtual void OnPageHide() override;

        /** 页面卸载前回调 */
        virtual void OnPageUnload() override;

    private Q_SLOTS:
        /** 登录信号返回处理 */
        void onLoginSuccess();
        void onLoginError(const QString& message);

        /** 处理恢复上次登录用户账号信号返回 */
        void onRestoreUserAccount(const QString& account);

    private:
        // page页面
        LoginPage* m_page = nullptr;

        // 业务管理器
        Core::BusinessManager* m_bussinessMag = nullptr;
    };
}
