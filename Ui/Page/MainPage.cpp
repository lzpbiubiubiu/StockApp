#include "Controller/MainPageController.h"
#include "MainPage.h"

namespace UI
{
    IMPLEMENT_PAGE("mainPage", MainPage)

    MainPage::MainPage(QObject* parent/* = nullptr*/)
        : Page(parent)
    {
        m_stackWarePanel = new StackWarePanel(this);
        SetContentItem("qrc:/Qml/Page/MainPage.qml");
        SetController(new MainPageController(this));
    }

    MainPage::~MainPage()
    {
    }

    StackWarePanel* MainPage::GetStackWarePanel() const
    {
        return m_stackWarePanel;
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
