#pragma once

#include "PageController.h"
#include "Ui/Page/SalePage.h"
#include "Ui/Component/SaleWarePanel.h"
#include "Core/BusinessManager.h"
namespace UI
{
    class SalePageController : public PageController
    {
        Q_OBJECT

    public:
        SalePageController(Page* parent);

        virtual ~SalePageController();

        /** 商品数量+1 */
        Q_INVOKABLE void addWareCount(int index);

        /** 商品数量-1 */
        Q_INVOKABLE void reduceWareCount(int index);

        /** 编辑数量 */
        Q_INVOKABLE void editWareCount(int index, int count);

        /** 删除出库商品 */
        Q_INVOKABLE void deleteWareItem(int index);

        /** 出库订单结算 */
        Q_INVOKABLE void submitOrder(qint64 wholeOrderPromoAmt = 0, qint64 deliveryFeesAmt = 0, const QString& remarkText = "");

        /** 删除错误商品 */
        Q_INVOKABLE void deleteErrorWares();

        /** 编辑价格 */
        Q_INVOKABLE void editWarePrice(int index);

        /** 修改商品价格 */
        Q_INVOKABLE void changeWarePrice(int index, qint64 price);

        /** 单品赠送编辑 */
        Q_INVOKABLE void editWarePresentation(int index);

        /** 单品赠送 */
        Q_INVOKABLE void warePresentation(int index, qint64 maxPresentationNum = 0, qint64 presentationNum = 0);
    protected:
        /** 页面加载时回调 */
        virtual void OnPageLoad() override;

        /** 页面显示前回调 */
        virtual void OnPageShow() override;

        /** 页面隐藏前回调 */
        virtual void OnPageHide() override;

        /** 页面卸载前回调 */
        virtual void OnPageUnload() override;

        /** 更新商品数量 */
        void UpdateWareCount(int index, Core::BusinessManager::ItemAction action, int value = 1);

        /** 初始化page页面 */
        void InitPage();
    protected Q_SLOTS:
        /** 添加商品到出库购物车 */
        void onAddSaleWaresSuccess(Core::WareItemPtr item);

        /** 添加商品到出库购物车 */
        void onAddSaleMergeItemSuccess(Core::WareItemPtr item);

        /** 修改商品信号处理 */
        void onSaleUpdateItemSuccess(int index, Core::WareItemPtr item);
        void onSaleUpdateItemError(const QString& message);

        /** 赠送商品信号处理 */
        void onPresentationWareSuccess(int index, Core::WareItemPtr item);
        void onPresentationWareError(const QString& message);

        /** 处理失败信号 */
        void onSignalError(const QString& message);

        /** 处理删除商品信号 */
        void onDeletedItem(int index);

        /** 处理出库订单结算信号 */
        void onSubmitSaleOrderSuccess();
        void onSubmitSaleOrderError(const QString& message);


    private:
        // 页面
        SalePage* m_page = nullptr;

        // 首页商品列表面板
        SaleWarePanel* m_saleWarePanel = nullptr;

        // 业务管理器
        Core::BusinessManager* m_businessManager = nullptr;

    };
}
