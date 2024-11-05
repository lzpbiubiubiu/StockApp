#include "MainPageController.h"
#include "Base/Common/ServiceManager.h"
#include "Base/Log/Log.h"
#include "Application.h"
namespace UI
{
    MainPageController::MainPageController(Page* parent)
        : PageController(parent)
    {
        m_page = GetPage<MainPage>();
        m_stackWarePanel = m_page->GetStackWarePanel();
        m_businessManager = Base::GetService<Core::BusinessManager>();
        connect(Base::ServiceManager::Get(), &Base::ServiceManager::signalLoaded, this, &MainPageController::onLoaded);
        connect(Base::ServiceManager::Get(), &Base::ServiceManager::signalLoadError, this, &MainPageController::onSignalError);
        connect(m_businessManager, &Core::BusinessManager::signalLoadConfigWaresSuccess, this, &MainPageController::onLoadConfigWaresSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalLoadConfigWaresError, this, &MainPageController::onLoadConfigWaresError);
        connect(m_businessManager, &Core::BusinessManager::signalAddSaleWaresSuccess, this, &MainPageController::onAddSaleWaresSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalAddSaleWaresError, this, &MainPageController::onSignalError);
        connect(m_businessManager, &Core::BusinessManager::signalSaleMergeItemError, this, &MainPageController::onSignalError);
        connect(m_businessManager, &Core::BusinessManager::signalDeletedSaleItem, this, &MainPageController::onDeletedSaleWareItem);
        connect(m_businessManager, &Core::BusinessManager::signalPresentationWareSuccess, this, &MainPageController::onPresentationWareSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalRestoreOrderSuccess, this, &MainPageController::onRestoreOrder);
        connect(m_businessManager, &Core::BusinessManager::signalSubmitSaleOrderSuccess, this, &MainPageController::onSubmitSaleOrderSuccess);
    }

    MainPageController::~MainPageController()
    {
    }

    void MainPageController::OnPageLoad()
    {
        LOG_INFO(QString("MainPage => OnPageLoad()"));
        FetchConfigWares();
    }

    void MainPageController::OnPageShow()
    {
        LOG_INFO(QString("MainPage => OnPageShow()"));
        InitPage();
    }

    void MainPageController::OnPageHide()
    {
        LOG_INFO(QString("MainPage => OnPageHide()"));
    }

    void MainPageController::OnPageUnload()
    {
        LOG_INFO(QString("MainPage => OnPageUnload()"));
    }

    void MainPageController::FetchConfigWares()
    {
        m_businessManager->FetchConfigWares();
        m_page->SetFetching(true);
    }

    void MainPageController::onLoaded(Base::ServiceManager::StartupOption option)
    {
        // 延迟加载服务不处理
        if(option == Base::ServiceManager::LAZY)
            return;
    }

    void MainPageController::onLoadConfigWaresSuccess()
    {
        m_stackWarePanel->InitWares();
        m_page->SetFetching(false);
    }

    void MainPageController::onLoadConfigWaresError(const QString& message)
    {
        m_page->SetFetching(false);
        //UI::Application::Get()->GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
    }

    void MainPageController::retailWare(int index)
    {
        auto order = m_businessManager->GetCurrentSaleOrder();
        if(order)
        {
            if(Core::Order::OrderType::RETAIL != order->type)
            {
                UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("当前已经存在批发订单"), TOAST_SHORT_DELAY);
                return;
            }
        }
        auto wareItem = m_businessManager->CreatorConfigWare(index);
        if(wareItem)
            m_businessManager->AddSaleWareItem(wareItem, Core::Order::OrderType::RETAIL);
    }

    void MainPageController::wholesaleWare(int index)
    {
        auto order = m_businessManager->GetCurrentSaleOrder();
        if(order)
        {
            if(Core::Order::OrderType::WHOLESALE != order->type)
            {
                UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("当前已经存在零售订单"), TOAST_SHORT_DELAY);
                return;
            }
        }
        auto wareItem = m_businessManager->CreatorConfigWare(index);
        if(wareItem)
            m_businessManager->AddSaleWareItem(wareItem, Core::Order::OrderType::WHOLESALE);
    }

    void MainPageController::refreshWares()
    {
        m_businessManager->FetchConfigWares();
        m_page->SetFetching(true);
    }

    void MainPageController::onAddSaleWaresSuccess(Core::WareItemPtr item)
    {
        UI::Application::Get()->GetMainWindow()->UpdateToolBar();
    }

    void MainPageController::onSignalError(const QString& message)
    {
        UI::Application::Get()->GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
    }

    void MainPageController::onDeletedSaleWareItem(int index)
    {
        UI::Application::Get()->GetMainWindow()->UpdateToolBar();
    }

    void MainPageController::onPresentationWareSuccess(int index, Core::WareItemPtr item)
    {
        UI::Application::Get()->GetMainWindow()->UpdateToolBar();
    }

    void MainPageController::InitPage()
    {
        UI::Application::Get()->GetMainWindow()->UpdateToolBar();
    }

    void MainPageController::onRestoreOrder()
    {
        InitPage();
    }

    void MainPageController::onSubmitSaleOrderSuccess()
    {
        UI::Application::Get()->GetMainWindow()->UpdateToolBar();
    }

} // namespace UI
