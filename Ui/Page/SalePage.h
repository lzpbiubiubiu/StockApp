#pragma once

#include "Page.h"
#include "Ui/Component/SaleWarePanel.h"

namespace UI
{
    class SalePage : public Page
    {
        Q_OBJECT
        Q_PROPERTY(QObject* saleWarePanel MEMBER m_saleWarePanel CONSTANT)
        Q_PROPERTY(qint64 orderAmount READ GetOrderAmount NOTIFY signalOrderAmountChanged)
        Q_PROPERTY(qint64 promotionAmount READ GetPromotionAmount NOTIFY signalPromotionAmountChanged)
        Q_PROPERTY(bool wholeSaleOrder READ GetWholeSaleOrder NOTIFY signalWholeSaleOrderChanged)
        Q_PROPERTY(bool hasTradeWare READ GetHasTradeWare NOTIFY signalHasTradeWareChanged)
        DECLARE_PAGE()

    public:
        Q_INVOKABLE SalePage(QObject* parent = nullptr);

        virtual ~SalePage();

        /** 获取商品加购面板 */
        SaleWarePanel* GetSaleWarePanel() const;

        /** 更新订单信息 */
        void UpdateToolBar();

        /** 显示商品单价修改面板 */
        void ShowPriceEditPanel(int index, Core::WareItemPtr item);

        /** 单品赠送弹出层 */
        void ShowWarePresentationPanel(int index, Core::WareItemPtr item);

        /** 设置|获取出库订单金额 */
        void SetOrderAmount(qint64 amount);
        qint64 GetOrderAmount() const;

        /** 设置|获取出库订单促销金额 */
        void SetPromotionAmount(qint64 amount);
        qint64 GetPromotionAmount() const;

        /** 设置|获取出库订单是否是批发订单 */
        void SetWholeSaleOrder(bool wholeSaleOrder);
        bool GetWholeSaleOrder() const;

        /** 设置|获取数据模型 */
        void SetHasTradeWare(bool hasTradeWare);
        bool GetHasTradeWare();

    Q_SIGNALS:
        void signalOrderAmountChanged();
        void signalHasTradeWareChanged();
        void signalPromotionAmountChanged();
        void signalWholeSaleOrderChanged();

    private:

        // 出库商品列表面板
        SaleWarePanel* m_saleWarePanel = nullptr;

        // 出库订单金额
        qint64 m_orderAmount = 0;

        // 出库订单促销金额
        qint64 m_promotionAmount = 0;

        // 是否有异常商品
        bool m_hasTradeWare = false;

        // 是否批发订单
        bool m_wholeSaleOrder = false;
    };
} // namespace UI
