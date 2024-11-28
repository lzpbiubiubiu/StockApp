#include "Controller/MainPageController.h"
#include "MainPage.h"

namespace UI
{
    IMPLEMENT_PAGE("mainPage", MainPage)

    MainPage::MainPage(QObject* parent/* = nullptr*/)
        : Page(parent)
    {
        m_stockWarePanel = new StockWarePanel(this);
        SetContentItem("qrc:/Qml/Page/MainPage.qml");
        SetController(new MainPageController(this));
    }

    MainPage::~MainPage()
    {
    }

    StockWarePanel* MainPage::GetStockWarePanel() const
    {
        return m_stockWarePanel;
    }

    bool MainPage::GetFetching()
    {
        return m_fetching;
    }

    void MainPage::SetFetching(bool fetching)
    {
        if(m_fetching != fetching)
        {
            m_fetching = fetching;
            emit signalFetchingChanged();
        }
    }
} // namespace UI
