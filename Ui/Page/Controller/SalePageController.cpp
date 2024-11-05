#include "SalePageController.h"
#include "Base/Common/ServiceManager.h"
#include "Base/Util/MathUtil.h"
#include "Base/Log/Log.h"
#include "Application.h"
namespace UI
{
    SalePageController::SalePageController(Page* parent)
        : PageController(parent)
    {
        m_page = GetPage<SalePage>();
        m_saleWarePanel = m_page->GetSaleWarePanel();
        m_businessManager = Base::GetService<Core::BusinessManager>();
        connect(m_businessManager, &Core::BusinessManager::signalAddSaleWaresSuccess, this, &SalePageController::onAddSaleWaresSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalSaleMergeItemSuccess, this, &SalePageController::onAddSaleMergeItemSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalUpdateSaleItemSuccess, this, &SalePageController::onSaleUpdateItemSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalUpdateSaleItemError, this, &SalePageController::onSignalError);
        connect(m_businessManager, &Core::BusinessManager::signalPresentationWareSuccess, this, &SalePageController::onPresentationWareSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalPresentationWareError, this, &SalePageController::onPresentationWareError);
        connect(m_businessManager, &Core::BusinessManager::signalDeletedSaleItem, this, &SalePageController::onDeletedItem);
        connect(m_businessManager, &Core::BusinessManager::signalSubmitSaleOrderSuccess, this, &SalePageController::onSubmitSaleOrderSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalSubmitSaleOrderError, this, &SalePageController::onSubmitSaleOrderError);

    }

    SalePageController::~SalePageController()
    {
    }

    void SalePageController::OnPageLoad()
    {
        LOG_INFO(QString("SalePage => OnPageLoad()"));
    }

    void SalePageController::OnPageShow()
    {
        LOG_INFO(QString("SalePage => OnPageShow()"));
        InitPage();
    }

    void SalePageController::OnPageHide()
    {
        LOG_INFO(QString("SalePage => OnPageHide()"));
    }

    void SalePageController::OnPageUnload()
    {
        LOG_INFO(QString("SalePage => OnPageUnload()"));
    }

    void SalePageController::InitPage()
    {
        m_saleWarePanel->InitWares();
        m_page->UpdateToolBar();
    }

    void SalePageController::onAddSaleWaresSuccess(Core::WareItemPtr item)
    {
        m_saleWarePanel->OnAddItem(item);
        m_page->UpdateToolBar();
    }

    void SalePageController::onAddSaleMergeItemSuccess(Core::WareItemPtr item)
    {
        m_saleWarePanel->OnMergeItem(item);
        m_page->UpdateToolBar();
    }

    void SalePageController::addWareCount(int index)
    {
        UpdateWareCount(index, Core::BusinessManager::ItemAction::ADD_COUNT, 1);
    }

    void SalePageController::reduceWareCount(int index)
    {
        UpdateWareCount(index, Core::BusinessManager::ItemAction::REDUCE_COUNT, 1);
    }

    void SalePageController::editWareCount(int index, int count)
    {
        UpdateWareCount(index, Core::BusinessManager::ItemAction::UPDATE_COUNT, count);
    }

    void SalePageController::deleteWareItem(int index)
    {
        m_businessManager->DeleteSaleWareItem(index);
    }

    void SalePageController::UpdateWareCount(int index, Core::BusinessManager::ItemAction action, int value)
    {
        auto order = m_businessManager->GetCurrentSaleOrder();
        if(order.isNull())
            return;

        if(index < 0 || index >= order->items.size())
            return;

        auto item = order->items[index];
        if(item.isNull())
            return;
        QVariantList values;
        m_businessManager->UpdateSaleWareItem(index, action, values << value);
    }

    void SalePageController::onSaleUpdateItemSuccess(int index, Core::WareItemPtr item)
    {
        m_saleWarePanel->OnUpdateItem(index, item);
        m_page->UpdateToolBar();
    }

    void SalePageController::onSaleUpdateItemError(const QString& message)
    {
        UI::Application::Get()->GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
    }

    void SalePageController::onPresentationWareSuccess(int index, Core::WareItemPtr item)
    {
        m_saleWarePanel->OnDeleteItem(index);
        m_saleWarePanel->OnMergeItem(item);
        m_page->UpdateToolBar();
    }

    void SalePageController::onPresentationWareError(const QString& message)
    {
       UI::Application::Get()->GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
    }

    void SalePageController::onSignalError(const QString& message)
    {
        UI::Application::Get()->GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
    }

    void SalePageController::onDeletedItem(int index)
    {
        m_saleWarePanel->OnDeleteItem(index);
        m_page->UpdateToolBar();
        UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("删除商品成功"), TOAST_SHORT_DELAY);
    }

    void SalePageController::submitOrder(qint64 wholeOrderPromoAmt, qint64 deliveryFeesAmt, const QString& remarkText)
    {
        UI::Application::Get()->GetMainWindow()->ShowLoading();
        m_businessManager->SubmitSaleOrder(wholeOrderPromoAmt, deliveryFeesAmt, remarkText);
    }

    void SalePageController::onSubmitSaleOrderSuccess()
    {
        UI::Application::Get()->GetMainWindow()->CloseLoading();
        UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("订单出库成功"), TOAST_SHORT_DELAY);
        m_saleWarePanel->InitWares();
        m_page->UpdateToolBar();
    }

    void SalePageController::onSubmitSaleOrderError(const QString& message)
    {
        UI::Application::Get()->GetMainWindow()->CloseLoading();
        // 判断是否需要截留商品
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            UI::Application::Get()->GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
            return;
        }

        // 无错误商品数据
        QJsonObject root = doc.object();
        QString msg = root["displayMsg"].toString();
        QJsonArray wares = root["data"].toObject()["tradeErrorWares"].toArray();
        if(wares.isEmpty())
        {
            UI::Application::Get()->GetMainWindow()->ShowToast(msg, TOAST_SHORT_DELAY);
            return;
        }
        m_page->SetHasTradeWare(true);

        for(auto item : wares)
        {
            QJsonObject obj = item.toObject();
            Core::WareItemPtr wareItem = Core::WareItemPtr::create();
            wareItem->code = obj["code"].toString();
            wareItem->uuid = obj["uuid"].toString();
            wareItem->stock = obj["stock"].toInt();
            wareItem->count = obj["count"].toInt();
            m_saleWarePanel->OnUpdateItemDisable(wareItem);
        }
    }

    void SalePageController::deleteErrorWares()
    {
        for(const auto& item : m_saleWarePanel->GetTradeErrorWares())
        {
            // 库存不为0的时候，删除部分数量商品
            auto model = m_saleWarePanel->GetModel();
            auto index = model->getIndex(item.uuid);
            auto stock = item.stock.toInt();
            auto count = item.count.toInt();
            if(0 == stock)
            {
                m_businessManager->DeleteSaleWareItem(index);
            }
            else
            {
                UpdateWareCount(index, Core::BusinessManager::ItemAction::REDUCE_COUNT, count - stock);
            }
        }
    }

    void SalePageController::editWarePrice(int index)
    {
        auto order = m_businessManager->GetCurrentSaleOrder();
        if(order)
        {
            if(index >= 0 && index < order->items.size())
            {
                GetPage<SalePage>()->ShowPriceEditPanel(index, order->items[index]);
            }
        }
    }

    void SalePageController::changeWarePrice(int index, qint64 price)
    {
        auto order = m_businessManager->GetCurrentSaleOrder();
        if(order.isNull())
            return;

        if(index < 0 || index >= order->items.size())
            return;

        QVariantList values;
        values << price;
        m_businessManager->UpdateSaleWareItem(index, Core::BusinessManager::ItemAction::UPDATE_DISCOUNT_PRICE, values);
    }

    void SalePageController::editWarePresentation(int index)
    {
        auto order = m_businessManager->GetCurrentSaleOrder();
        if(order.isNull())
            return;

        if(index < 0 || index >= order->items.size())
            return;

        auto item = order->items[index];
        if(item.isNull())
            return;

        // 拦截已删除商品
        if(item->count == 0)
            return;

        GetPage<SalePage>()->ShowWarePresentationPanel(index, item);
    }

    void SalePageController::warePresentation(int index, qint64 maxPresentationNum, qint64 presentationNum)
    {
        QVariantList values;
        if(maxPresentationNum == presentationNum)
        {
            // 全部赠送
            QVariantList values;
            values << Core::WareItem::WareType::WARE_TYPE_GIFT;
            m_businessManager->UpdateSaleWareItem(index, Core::BusinessManager::ItemAction::UPDATE_PRESENTATION, values << values);
        }
        else
        {
            // 部分赠送
            UpdateWareCount(index, Core::BusinessManager::ItemAction::REDUCE_COUNT, presentationNum);

            auto order = m_businessManager->GetCurrentSaleOrder();
            auto item = order->items[index];
            Core::WareItemPtr newitem = Core::WareItemPtr::create();
            *newitem = *item;
            newitem->uuid = Base::MathUtil::GetUUID();
            newitem->count = presentationNum;
            newitem->discountPrice = 0;
            newitem->promotionAmount = 0;
            newitem->amount = 0;
            newitem->wareType = Core::WareItem::WARE_TYPE_GIFT;
            m_businessManager->AddPresentationWareItem(newitem);
        }
    }

} // namespace UI
