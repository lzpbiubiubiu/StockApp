#include "OrderStockListPanel.h"
#include "Base/Common/ServiceManager.h"
#include "Core/BusinessManager.h"
#include "Base/Util/PriceUtil.h"
#include "Base/Util/JsonUtil.h"
#include "Application.h"

namespace UI
{
    OrderStockListPanel::OrderStockListPanel(QObject* parent)
        : QObject(parent)
    {
        connect(Base::GetService<Core::BusinessManager>(), &Core::BusinessManager::signalQueryOrderStockListSuccess, this, &OrderStockListPanel::onQueryOrderStockListSuccess);
        connect(Base::GetService<Core::BusinessManager>(), &Core::BusinessManager::signalQueryOrderStockListError, this, &OrderStockListPanel::onQueryOrderStockListError);
        connect(Base::GetService<Core::BusinessManager>(), &Core::BusinessManager::signalModifyStockOrderSuccess, this, &OrderStockListPanel::onModifyStockOrderSuccess);
        connect(Base::GetService<Core::BusinessManager>(), &Core::BusinessManager::signalModifyStockOrderError, this, &OrderStockListPanel::onModifyStockOrderError);
    }

    OrderStockListPanel::~OrderStockListPanel()
    {

    }

    int OrderStockListPanel::GetPageIndex() const
    {
        return m_pageIndex;
    }

    void OrderStockListPanel::SetPageIndex(int index)
    {
        if(m_pageIndex != index)
        {
            m_pageIndex = index;
            emit signalPageIndexChanged();
        }
    }

    void OrderStockListPanel::SetOrderModel()
    {
        QJsonArray orderModel;
        for(auto item : m_stockOrders)
        {
            orderModel.append(MakeStockOrderObj(item));
        }
        m_orderModel = orderModel;
        emit signalOrderModelChanged();
    }

    void OrderStockListPanel::SetCurrentOrder(Core::StockOrderDetailPtr stockOrder)
    {
        if(!stockOrder)
            return;

        m_currentOrder = MakeStockOrderObj(stockOrder);
        emit signalCurrentOrderChanged();
    }

    QJsonObject OrderStockListPanel::GetCurrentOrder() const
    {
        return m_currentOrder;
    }

    void OrderStockListPanel::SetQuerying(bool querying)
    {
        if(m_querying != querying)
        {
            m_querying = querying;
            emit signalQueryingChanged();
        }
    }

    bool OrderStockListPanel::GetQuerying() const
    {
        return m_querying;
    }

    void OrderStockListPanel::SetQuerySuccess(bool querySuccess)
    {
        if(m_querySuccess != querySuccess)
        {
            m_querySuccess = querySuccess;
            emit signalQuerySuccessChanged();
        }
    }

    bool OrderStockListPanel::GetQuerySuccess() const
    {
        return m_querySuccess;
    }

    void OrderStockListPanel::SetRefreshButtonEnable(bool refreshButtonEnable)
    {
        if(m_refreshButtonEnable != refreshButtonEnable)
        {
            m_refreshButtonEnable = refreshButtonEnable;
            emit signalRefreshButtonEnableChanged();
        }
    }

    bool OrderStockListPanel::GetRefreshButtonEnable() const
    {
        return m_refreshButtonEnable;
    }

    QJsonArray OrderStockListPanel::GetOrderModel() const
    {
        return m_orderModel;
    }

    void OrderStockListPanel::resetProperty()
    {
        m_pageIndex = PageIndex::PAGE_ORDER_STOCK_LIST;
        SetQuerying(false);
        SetQuerySuccess(false);
        SetRefreshButtonEnable(true);
    }

    void OrderStockListPanel::queryOrderStockList()
    {
        SetQuerying(true);
        Base::GetService<Core::BusinessManager>()->QueryOrderStockList();
    }

    void OrderStockListPanel::updateStockOrder(int orderIndex, int wareIndex, int wareCount)
    {
        if(orderIndex < 0 || orderIndex >= m_stockOrders.size())
            return;

        auto orderItem = m_stockOrders[orderIndex];
        if(wareIndex < 0 || wareIndex >= orderItem->wares.size())
            return;
        auto& wareItem = orderItem->wares[wareIndex];
        int count = wareItem.count;
        wareItem.count = wareCount;
        qint64 amount1 = wareItem.amount;
        qint64 promotionAmount1 = (count * wareItem.promotionAmount);
        wareItem.amount = wareItem.discountPrice * wareItem.count;
        orderItem->orderAmount += (wareItem.amount - amount1);
        qint64 promotionAmount2 = (wareItem.count * wareItem.promotionAmount);
        orderItem->totalPromotionAmount += promotionAmount2 - promotionAmount1;
        SetOrderModel();
        SetCurrentOrder(orderItem);
    }

    void OrderStockListPanel::updateStockOrder(int orderIndex, const QString& orderRemark)
    {
        if(orderIndex < 0 || orderIndex >= m_stockOrders.size())
            return;

        auto orderItem = m_stockOrders[orderIndex];
        auto extensionObj = Base::JsonUtil::ToJsonObject(orderItem->extension);
        extensionObj["orderRemark"] = orderRemark;
        orderItem->extension = Base::JsonUtil::ToString(extensionObj);
        SetOrderModel();
        SetCurrentOrder(orderItem);
    }

    void OrderStockListPanel::clickOrderDetail(int orderIndex)
    {
        if(orderIndex < 0 || orderIndex >= m_stockOrders.size())
            return;
        auto stockOrder = m_stockOrders[orderIndex];
        SetCurrentOrder(stockOrder);
    }

    void OrderStockListPanel::refreshQueryOrderStockList()
    {
        SetRefreshButtonEnable(false);
        queryOrderStockList();
    }

    void OrderStockListPanel::onQueryOrderStockListSuccess(const Core::StockOrderDetailPtrList& stockOrders)
    {
        SetQuerying(false);
        SetQuerySuccess(true);
        SetRefreshButtonEnable(true);
        m_stockOrders = stockOrders;
        SetOrderModel();
    }

    void OrderStockListPanel::onQueryOrderStockListError(const QString& message)
    {
        UI::Application::GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
        SetQuerying(false);
        SetQuerySuccess(false);
        SetRefreshButtonEnable(true);
    }

    QJsonObject OrderStockListPanel::MakeStockOrderObj(Core::StockOrderDetailPtr stockOrder)
    {
        QJsonObject obj;
        if(!stockOrder)
            return obj;

        obj["receipt"] = stockOrder->orderNo;
        obj["dateTime"] = stockOrder->time;
        obj["orderAmt"] = Base::PriceUtil::FenToString(stockOrder->orderAmount);
        obj["totalPromotionAmt"] = Base::PriceUtil::FenToString(stockOrder->totalPromotionAmount);
        obj["deliveryFreeAmt"] = Base::PriceUtil::FenToString(stockOrder->deliveryFreeAmount);
        obj["orderRemark"] = Base::JsonUtil::ToJsonObject(stockOrder->extension)["orderRemark"].toString();
        QJsonArray wareArray;
        int piece = 0;
        int kind = 0;
        for(const auto& wareItem : stockOrder->wares)
        {
            QJsonObject wareObj;
            wareObj["wareName"] = wareItem.name;
            wareObj["wareCode"] = wareItem.code;
            wareObj["wareDiscountPrice"] = Base::PriceUtil::FenToString(wareItem.discountPrice);
            wareObj["warePromotionAmount"] = Base::PriceUtil::FenToString(wareItem.promotionAmount);
            wareObj["wareAmount"] = Base::PriceUtil::FenToString(wareItem.amount);
            wareObj["wareCount"] = QString::number(wareItem.count);
            wareArray.append(wareObj);
            kind++;
            piece += wareItem.count;
        }
        obj["wareTotalQty"] = QStringLiteral("%1品/%2件").arg(kind).arg(piece);
        obj["wareList"] = wareArray;
        return obj;
    }

    void OrderStockListPanel::onModifyStockOrderSuccess()
    {
        UI::Application::GetMainWindow()->CloseLoading();
        SetPageIndex(PageIndex::PAGE_ORDER_STOCK_LIST);
        queryOrderStockList();
    }

    void OrderStockListPanel::onModifyStockOrderError(const QString& message, Core::BusinessManager::StockOrderOperate operateType)
    {
        UI::Application::GetMainWindow()->CloseLoading();
        UI::Application::GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
        if(Core::BusinessManager::StockOrderOperate::ORDER_OUTBOUND == operateType)
        {
            SetPageIndex(PageIndex::PAGE_ORDER_STOCK_LIST);
        }
    }

    void OrderStockListPanel::stockOrderOutbound(int orderIndex)
    {
        if(orderIndex < 0 || orderIndex >= m_stockOrders.size())
            return;
        auto order = m_stockOrders[orderIndex];
        UI::Application::GetMainWindow()->ShowLoading();
        Base::GetService<Core::BusinessManager>()->ModifyStockOrder(order, Core::BusinessManager::StockOrderOperate::ORDER_OUTBOUND);
    }

    void OrderStockListPanel::stockOrderModify(int orderIndex)
    {
        if(orderIndex < 0 || orderIndex >= m_stockOrders.size())
            return;
        auto order = m_stockOrders[orderIndex];
        UI::Application::GetMainWindow()->ShowLoading();
        Base::GetService<Core::BusinessManager>()->ModifyStockOrder(order, Core::BusinessManager::StockOrderOperate::ORDER_MODIFY);
    }

    void OrderStockListPanel::stockOrderVoid(int orderIndex)
    {
        if(orderIndex < 0 || orderIndex >= m_stockOrders.size())
            return;
        auto order = m_stockOrders[orderIndex];
        UI::Application::GetMainWindow()->ShowLoading();
        Base::GetService<Core::BusinessManager>()->ModifyStockOrder(order, Core::BusinessManager::StockOrderOperate::ORDER_VOID);
    }
}
