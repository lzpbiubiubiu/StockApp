#pragma once

#include <QJsonArray>
#include "Core/BusinessManager.h"
#include "Core/StackStruct.h"
namespace UI
{
    /** 备货清单面板 OrderStockListPanel */
    class OrderStockListPanel : public QObject
    {
        // 页面序号
        enum PageIndex {

            /** 订单备货清单页面 */
            PAGE_ORDER_STOCK_LIST = 0,

            /** 订单详情页面 */
            PAGE_ORDER_DETAIL,

            /** 订单备注 */
            PAGE_ORDER_REMARK
        };

        Q_OBJECT
        Q_PROPERTY(int pageIndex READ GetPageIndex WRITE SetPageIndex NOTIFY signalPageIndexChanged)
        Q_PROPERTY(QJsonArray orderModel READ GetOrderModel NOTIFY signalOrderModelChanged)
        Q_PROPERTY(QJsonObject currentOrder READ GetCurrentOrder NOTIFY signalCurrentOrderChanged)
        Q_PROPERTY(bool querying READ GetQuerying NOTIFY signalQueryingChanged)
        Q_PROPERTY(bool querySuccess READ GetQuerySuccess NOTIFY signalQuerySuccessChanged)
        Q_PROPERTY(bool refreshButtonEnable READ GetRefreshButtonEnable NOTIFY signalRefreshButtonEnableChanged)

    public:
        OrderStockListPanel(QObject* parent = nullptr);
        ~OrderStockListPanel();

        /** 获取|设置页面索引 */
        int GetPageIndex() const;
        void SetPageIndex(int index);

        /** 设置|获取备货清单model数据 */
        void SetOrderModel();
        QJsonArray GetOrderModel() const;

        /** 设置|获取当前选中订单的数据 */
        void SetCurrentOrder(Core::StockOrderDetailPtr stockOrder);
        QJsonObject GetCurrentOrder() const;

        /** 设置|获取查询中状态 */
        void SetQuerying(bool querying);
        bool GetQuerying() const;

        /** 设置|获取查询成功状态 */
        void SetQuerySuccess(bool querySuccess);
        bool GetQuerySuccess() const;

        /** 设置|获取重试按钮的使能状态 */
        void SetRefreshButtonEnable(bool refreshButtonEnable);
        bool GetRefreshButtonEnable() const;

        /** 重置属性值*/
        Q_INVOKABLE void resetProperty();

        /** 查询 */
        Q_INVOKABLE void queryOrderStockList();

        /** 重试查询 */
        Q_INVOKABLE void refreshQueryOrderStockList();

        /** 更新备货订单 */
        Q_INVOKABLE void updateStockOrder(int orderIndex, int wareIndex, int wareCount);

        /** 更新备货订单 */
        Q_INVOKABLE void updateStockOrder(int orderIndex, const QString& orderRemark);

        /** 选择当前的订单明细 */
        Q_INVOKABLE void clickOrderDetail(int orderIndex);

        /** 订单出库 */
        Q_INVOKABLE void stockOrderOutbound(int orderIndex);

        /** 订单修改 */
        Q_INVOKABLE void stockOrderModify(int orderIndex);

        /** 订单作废 */
        Q_INVOKABLE void stockOrderVoid(int orderIndex);
    Q_SIGNALS:
        void signalPageIndexChanged();
        void signalOrderModelChanged();
        void signalCurrentOrderChanged();
        void signalQueryingChanged();
        void signalQuerySuccessChanged();
        void signalRefreshButtonEnableChanged();

    private Q_SLOTS:
        /** 查询备货清单列表信号返回 */
        void onQueryOrderStockListSuccess(const Core::StockOrderDetailPtrList& stockOrders);
        void onQueryOrderStockListError(const QString& message);

        /** 备货订单修改信号返回 */
        void onModifyStockOrderSuccess();
        void onModifyStockOrderError(const QString& message, Core::BusinessManager::StockOrderOperate operateType);

    private:
        /** 组装界面显示的订单数据 */
        QJsonObject MakeStockOrderObj(Core::StockOrderDetailPtr stockOrder);

    private:

        // 页面索引
        int m_pageIndex = PageIndex::PAGE_ORDER_STOCK_LIST;

        // 查询中
        bool m_querying = false;

        // 查询成功
        bool m_querySuccess = false;

        // 重试按钮的使能
        bool m_refreshButtonEnable = true;

        // 订单数据model
        QJsonArray m_orderModel;

        // 当前选择的订单数据
        QJsonObject m_currentOrder;

        // 备货清单列表
        Core::StockOrderDetailPtrList m_stockOrders;
    };
}
