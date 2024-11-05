#include <QApplication>
#include <QQmlComponent>
#include <QQmlContext>
#include "MainWindow.h"
#include "Base/Common/ServiceManager.h"
#include "Core/BusinessManager.h"
#include "Core/GlobalData.h"
#include "Ui/Page/Controller/MainPageController.h"
#include "Base/Log/Log.h"
namespace UI
{
    MainWindow::MainWindow(QSharedPointer<QQmlEngine> engine)
        : QObject(engine.data())
    {
        engine->rootContext()->setContextProperty("mainWindow", this);
        QQmlComponent component(engine.data(), QUrl(QStringLiteral("qrc:/Qml/MainWindow.qml")));
        m_window = qobject_cast<QQuickWindow*>(component.create());
        Q_ASSERT_X(m_window != nullptr, "MainWindow", component.errorString().toUtf8());
        m_window->setScreen(QApplication::primaryScreen());
        engine->rootContext()->setContextProperty("mainFrame", m_window);

#ifndef QT_DEBUG
        SetFullScreen(true);
#endif // !DEBUG
        connect(Base::GetService<Core::BusinessManager>(), &Core::BusinessManager::signalLoginSuccess, this, &MainWindow::onLoginSuccess);
    }

    MainWindow::~MainWindow()
    {
        if(m_window != nullptr)
        {
            m_window->close();
            m_window->deleteLater();
        }
    }

    void MainWindow::Show()
    {
        m_window->show();
    }

    void MainWindow::Tray()
    {
        m_window->hide();
    }

    void MainWindow::LoadPage(const QString& name, const QVariant& data/* = QVariant()*/)
    {
        // 页面已加载
        if(m_currentPageName == name)
            return;

        // 页面未注册
        auto metaClass = UI::Page::GetMeta()->FindClass(name);
        if(!metaClass)
            return;

        // 退出上一页
        if(m_pages.contains(m_currentPageName))
        {
            auto prev = m_pages[m_currentPageName];
            prev->OnHide();
            if(prev->IsAutoUnload())
            {
                prev->OnUnload();
                prev->deleteLater();
                m_pages.remove(m_currentPageName);
            }
        }

        // 加载当前页
        m_currentPageName = name;
        if(!m_pages.contains(m_currentPageName))
        {
            auto next = qobject_cast<Page*>(metaClass->newInstance(Q_ARG(QObject*, this)));
            next->SetName(name);
            next->SetUserData(data);
            next->OnLoad();
            m_pages[m_currentPageName] = next;
        }

        // 显示当前页
        if(m_window != nullptr && m_pages[m_currentPageName]->GetContentItem() != nullptr)
        {
            m_pages[m_currentPageName]->SetUserData(data);
            QMetaObject::invokeMethod(m_window, "loadPage", Q_ARG(QVariant, QVariant::fromValue(m_pages[m_currentPageName])));
            m_pages[m_currentPageName]->OnShow();
        }
    }

    void MainWindow::ShowLoading(const QString& text/* = QString()*/, bool delayShow/* = false*/)
    {
        QMetaObject::invokeMethod(m_window, "showLoading", Q_ARG(QVariant, text), Q_ARG(QVariant, delayShow));
    }

    void MainWindow::CloseLoading()
    {
        QMetaObject::invokeMethod(m_window, "closeLoading");
    }

    void MainWindow::ShowToast(const QString text, int duration)
    {
        QMetaObject::invokeMethod(m_window, "showToast", Q_ARG(QVariant, text), Q_ARG(QVariant, duration));
    }

    bool MainWindow::GetFullScreen() const
    {
        return m_fullScreen;
    }

    void MainWindow::SetFullScreen(bool fullScreen)
    {
        if(m_fullScreen != fullScreen)
        {
            m_fullScreen = fullScreen;
            emit signalFullScreenChanged();
        }
    }

    int MainWindow::GetLoadingRefCount() const
    {
        return m_loadingRefCount;
    }

    void MainWindow::SetLoadingRefCount(int count)
    {
        if(m_loadingRefCount != count)
        {
            m_loadingRefCount = count;
            emit signalLoadingRefCountChanged();
        }
    }

    void MainWindow::addPageMaskRefCount()
    {
        ++m_pageMaskRefCount[m_currentPageName];
        LOG_INFO(QStringLiteral("increase, currentPage is %1 mask count is %2").arg(m_currentPageName).arg(m_pageMaskRefCount[m_currentPageName]));
    }

    void MainWindow::releasePageMaskRefCount()
    {
        if(m_pageMaskRefCount[m_currentPageName] <= 0)
        {
            LOG_INFO(QStringLiteral("decrease currentPage is %1 mask count is %2 less than zero").arg(m_currentPageName).arg(m_pageMaskRefCount[m_currentPageName]));
            return;
        }

        --m_pageMaskRefCount[m_currentPageName];
        LOG_INFO(QStringLiteral("decrease, currentPage is %1 mask count is %2").arg(m_currentPageName).arg(m_pageMaskRefCount[m_currentPageName]));
    }

    int MainWindow::GetPageIndex() const
    {
        return m_pageIndex;

    }

    void MainWindow::SetPageIndex(int pageIndex)
    {
        if(m_pageIndex != pageIndex)
        {
            m_pageIndex = pageIndex;
            emit signalPageIndexChanged();
        }
    }
    void MainWindow::switchSalePage()
    {
        LoadPage("salePage");
        SetPageIndex(PAGE_SALE);
    }

    void MainWindow::switchMainPage()
    {
        if(PAGE_MAIN == m_pageIndex)
        {
            auto controller = static_cast<UI::MainPageController*>(m_pages[m_currentPageName]->GetController());
            if(controller)
            {
                controller->FetchConfigWares();
            }
            return;
        }
        LoadPage("mainPage");
        SetPageIndex(PAGE_MAIN);
    }

    void MainWindow::switchStoragePage()
    {
        LoadPage("storagePage");
        SetPageIndex(PAGE_STORAGE);
    }

    void MainWindow::switchReportPage()
    {
        LoadPage("reportPage");
        SetPageIndex(PAGE_REPORT);
    }

    int MainWindow::GetSaleWareKind() const
    {
        return m_saleWareKind;
    }
    void MainWindow::SetSaleWareKind(int saleWareKind)
    {
        if(m_saleWareKind != saleWareKind)
        {
            m_saleWareKind = saleWareKind;
            emit signalSaleWareKindChanged();
        }
    }

    int MainWindow::GetReturnWareKind() const
    {
        return m_returnWareKind;
    }

    void MainWindow::SetReturnWareKind(int returnWareKind)
    {
        if(m_returnWareKind != returnWareKind)
        {
            m_returnWareKind = returnWareKind;
            emit signalReturnWareKindChanged();
        }
    }

    void MainWindow::UpdateToolBar()
    {
        auto businessManager = Base::GetService<Core::BusinessManager>();
        auto saleOrder = businessManager->GetCurrentSaleOrder();
        if(saleOrder)
            SetSaleWareKind(saleOrder->kind);
        else
            SetSaleWareKind(0);

    }

    bool MainWindow::GetShowNumPad() const
    {
        return m_showNumPad;
    }

    void MainWindow::SetShowNumPad(bool show)
    {
        if(m_showNumPad != show)
        {
            m_showNumPad = show;
            emit signalShowNumPadChanged();
        }
    }

    bool MainWindow::GetSuperUser() const
    {
        return m_superUser;
    }

    void MainWindow::SetSuperUser(bool super)
    {
        if(m_superUser != super)
        {
            m_superUser = super;
            emit signalSuperUserChanged();
        }
    }

    void MainWindow::onLoginSuccess()
    {
        SetSuperUser(Core::User::Status::SUPER == Core::GlobalData::Get()->GetUser().status);
    }

} // namespace UI
