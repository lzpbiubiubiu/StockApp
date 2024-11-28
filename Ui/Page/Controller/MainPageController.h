#pragma once

#include "PageController.h"
#include "Core/StackStruct.h"
#include "Base/Common/ServiceManager.h"
#include "Ui/Page/MainPage.h"
#include "Core/BusinessManager.h"

namespace UI
{
    class MainPageController : public PageController
    {
        Q_OBJECT

    public:
        MainPageController(Page* parent);

        virtual ~MainPageController();

        /** 拉取配置商品列表 */
        void FetchConfigWares();

        /** 首页出库零售商品 */
        Q_INVOKABLE void retailWare(int index);

        /** 首页出库批发商品 */
        Q_INVOKABLE void wholesaleWare(int index);

        /** 刷新首页商品列表 */
        Q_INVOKABLE void refreshWares();
    protected:
        /** 页面加载时回调 */
        virtual void OnPageLoad() override;

        /** 页面显示前回调 */
        virtual void OnPageShow() override;

        /** 页面隐藏前回调 */
        virtual void OnPageHide() override;

        /** 页面卸载前回调 */
        virtual void OnPageUnload() override;

        /** 初始化page页面 */
        void InitPage();
    private Q_SLOTS:

        /** 服务加载结果 */
        void onLoaded(Base::ServiceManager::StartupOption option);

        /** 首页商品拉取配置结果 */
        void onLoadConfigWaresSuccess();
        void onLoadConfigWaresError(const QString& message);

        /** 添加商品到出库购物车 */
        void onAddSaleWaresSuccess(Core::WareItemPtr item);

        /** 处理失败信号 */
        void onSignalError(const QString& message);

        /** 删除出库商品 */
        void onDeletedSaleWareItem(int index);

        /** 商品赠送成功 */
        void onPresentationWareSuccess(int index, Core::WareItemPtr item);

        /** 处理异常订单恢复 */
        void onRestoreOrder();

         /** 处理出库订单结算结果 */
        void onSubmitSaleOrderSuccess();
    private:
        // 页面
        MainPage* m_page = nullptr;

        // 首页商品列表面板
        StockWarePanel* m_stockWarePanel = nullptr;

        // 业务管理器
        Core::BusinessManager* m_businessManager = nullptr;
    };
}
