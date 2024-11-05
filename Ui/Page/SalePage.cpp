#include "Controller/SalePageController.h"
#include "Base/Common/ServiceManager.h"
#include "Core/BusinessManager.h"
#include "SalePage.h"

namespace UI
{
    IMPLEMENT_PAGE("salePage", SalePage)

    SalePage::SalePage(QObject* parent/* = nullptr*/)
        : Page(parent)
    {
        m_saleWarePanel = new SaleWarePanel(this);
        SetContentItem("qrc:/Qml/Page/SalePage.qml");
        SetController(new SalePageController(this));
    }

    SalePage::~SalePage()
    {
    }

    SaleWarePanel* SalePage::GetSaleWarePanel() const
    {
        return m_saleWarePanel;
    }

    void SalePage::SetOrderAmount(qint64 amount)
    {
        if(m_orderAmount != amount)
        {
            m_orderAmount = amount;
            emit signalOrderAmountChanged();
        }
    }

    qint64 SalePage::GetOrderAmount() const
    {
        return m_orderAmount;
    }

    void SalePage::SetPromotionAmount(qint64 amount)
    {
        if(m_promotionAmount != amount)
        {
            m_promotionAmount = amount;
            emit signalPromotionAmountChanged();
        }
    }

    qint64 SalePage::GetPromotionAmount() const
    {
        return m_promotionAmount;
    }

    void SalePage::SetHasTradeWare(bool hasTradeWare)
    {
        if(m_hasTradeWare != hasTradeWare)
        {
            m_hasTradeWare = hasTradeWare;
            emit signalHasTradeWareChanged();
        }
    }

    bool SalePage::GetHasTradeWare()
    {
        return m_hasTradeWare;
    }

    void SalePage::UpdateToolBar()
    {
        auto businessManager = Base::GetService<Core::BusinessManager>();
        auto saleOrder = businessManager->GetCurrentSaleOrder();
        if(saleOrder)
        {
            SetOrderAmount(saleOrder->orderAmount);
            SetPromotionAmount(saleOrder->promotionAmount + saleOrder->wholeOrderPromoAmount);
        }
        else
        {
            SetOrderAmount(0);
            SetPromotionAmount(0);
        }

        SetHasTradeWare(false);
    }

    void SalePage::ShowPriceEditPanel(int index, Core::WareItemPtr item)
    {
        QJsonObject data;
        data["index"] = index;
        data["name"] = item->name;
        data["code"] = item->code;
        data["price"] = item->price;
        data["discountPrice"] = item->discountPrice;

        QMetaObject::invokeMethod(m_item, "showPriceEditPanel", Q_ARG(QVariant, QVariant::fromValue(data)));
    }

    void SalePage::ShowWarePresentationPanel(int index, Core::WareItemPtr item)
    {
        QJsonObject data;
        data["index"] = index;
        data["maxPresentationNum"] = item->count;
        data["name"] = item->name;
        data["code"] = item->code;

        // 数量为1的标品 和非标品 不需要输入数量
        if(1 == item->count)
        {
            data["showEditPresentationNum"] = false;
            data["presentationNum"] = item->count;
        }
        else
        {
            data["showEditPresentationNum"] = true;
            data["presentationNum"] = 0;
        }
        QMetaObject::invokeMethod(m_item, "showWarePresentationPanel", Q_ARG(QVariant, QVariant::fromValue(data)));
    }
} // namespace UI
