#pragma once

/**
* @file         MainWindow.h
* @brief        主窗口类
* @copyright    lizhongpu
*/
#include <QMap>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSharedPointer>
#include <QString>
#include <QInputMethod>
#include "Ui/Page/Page.h"

namespace UI
{
    class MainWindow : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool fullScreen READ GetFullScreen WRITE SetFullScreen NOTIFY signalFullScreenChanged)
        Q_PROPERTY(int loadingRefCount READ GetLoadingRefCount WRITE SetLoadingRefCount NOTIFY signalLoadingRefCountChanged)
        Q_PROPERTY(int pageIndex READ GetPageIndex NOTIFY signalPageIndexChanged)
        Q_PROPERTY(int saleWareKind READ GetSaleWareKind NOTIFY signalSaleWareKindChanged)
        Q_PROPERTY(bool showNumPad READ GetShowNumPad WRITE SetShowNumPad NOTIFY signalShowNumPadChanged)
        Q_PROPERTY(bool showKeyboard READ GetShowKeyboard NOTIFY signalShowKeyboardChanged)
        Q_PROPERTY(bool superUser READ GetSuperUser NOTIFY signalSuperUserChanged)
        Q_PROPERTY(bool isStocker READ GetIsStocker NOTIFY signalIsStockerChanged)

    public:
        enum PageIndex
        {
            /** 首页界面 */
            PAGE_MAIN = 0,

            /** 出库界面 */
            PAGE_SALE,

            /** 管理界面 */
            PAGE_MANAGE,

            /** 报表界面 */
            PAGE_REPORT
        };

    public:
        MainWindow(QSharedPointer<QQmlEngine> engine);
        ~MainWindow();

        /** 显示窗口 */
        void Show();

        /** 最小化托盘 */
        void Tray();

        /** 加载页面 */
        void LoadPage(const QString& name, const QVariant& data = QVariant());

        /** 显示加载界面 */
        void ShowLoading(const QString& text = QString(), bool delayShow = false);

        /** 关闭加载界面 */
        void CloseLoading();

        /** 显示Toast弹窗 */
        void ShowToast(const QString text, int duration);

        /** 设置|获取是否全屏相关 */
        bool GetFullScreen() const;
        void SetFullScreen(bool fullScreen);

        /** loading计数相关 */
        int GetLoadingRefCount() const;
        void SetLoadingRefCount(int count);

        /** 设置|获取当前页面枚举值 */
        int GetPageIndex() const;
        void SetPageIndex(int pageIndex);

        /** 设置|获取当前出库品数 */
        int GetSaleWareKind() const;
        void SetSaleWareKind(int saleWareKind);

        /** 设置|获取是否显示数字键盘 */
        bool GetShowNumPad() const;
        void SetShowNumPad(bool show);

        /** 设置|获取是否超级用户 */
        bool GetSuperUser() const;
        void SetSuperUser(bool super);

        /** 设置|获取是否仓库备货员 */
        bool GetIsStocker() const;
        void SetIsStocker(bool stocker);

        /** 设置|获取是否显示系统键盘 */
        bool GetShowKeyboard() const;
        void SetShowKeyboard(bool show);

        /** 更新底部导航信息 */
        void UpdateToolBar();

    private Q_SLOTS:
        /** 处理登录信号返回 */
        void onLoginSuccess();

        /** 键盘显示变化信号 */
        void onKeyboardVisibleChanged();
    protected:

        /** 导出全局JS函数：当前界面蒙层计数器+1 */
        Q_INVOKABLE void addPageMaskRefCount();

        /** 导出全局JS函数：当前界面蒙层计数器-1 */
        Q_INVOKABLE void releasePageMaskRefCount();

        /** 导出全局JS函数, 切换到首页页面 */
        Q_INVOKABLE void switchMainPage();

        /** 导出全局JS函数, 切换到出库页面 */
        Q_INVOKABLE void switchSalePage();

        /** 导出全局JS函数, 切换到后台管理页面 */
        Q_INVOKABLE void switchManagePage();

        /** 导出全局JS函数, 切换到报表页面 */
        Q_INVOKABLE void switchReportPage();

        /** 导出全局JS函数, 隐藏系统键盘 */
        Q_INVOKABLE void hideKeyboard();
    Q_SIGNALS:
        void signalFullScreenChanged();
        void signalLoadingRefCountChanged();
        void signalPageIndexChanged();
        void signalSaleWareKindChanged();
        void signalShowNumPadChanged();
        void signalSuperUserChanged();
        void signalIsStockerChanged();
        void signalShowKeyboardChanged();
    private:
        QQuickWindow* m_window = nullptr;

        // 所有页面
        QHash<QString, Page*> m_pages;

        // 当前系统键盘对象
        QInputMethod * m_keyboard = nullptr;

        // 当前页面
        QString m_currentPageName;

        // 是否全屏
        bool m_fullScreen = false;

        // loading计数器
        int m_loadingRefCount = 0;

        // 界面蒙层计数器
        QMap<QString, int> m_pageMaskRefCount;

        // 是否全屏
        int m_pageIndex = PAGE_MAIN;

        // 当前选择的出库购物车商品品数
        int m_saleWareKind = 0;

        // 是否显示数字键盘
        bool m_showNumPad = false;

        // 是否超级用户
        bool m_superUser = true;

        // 是否仓库备货员
        bool m_isStocker = false;

        // 是否显示系统键盘
        bool m_showKeyboard = false;

    };
} // namespace UI
