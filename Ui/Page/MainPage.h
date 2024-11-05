#pragma once

#include "Page.h"
#include "Ui/Component/StackWarePanel.h"

namespace UI
{
    class MainPage : public Page
    {
        Q_OBJECT
        Q_PROPERTY(QObject* stackWarePanel MEMBER m_stackWarePanel CONSTANT)
        Q_PROPERTY(bool fetching READ GetFetching NOTIFY signalFetchingChanged)
        DECLARE_PAGE()

    public:
        Q_INVOKABLE MainPage(QObject* parent = nullptr);

        virtual ~MainPage();

        /** 获取商品加购面板 */
        StackWarePanel* GetStackWarePanel() const;

        /** 设置|获取是否拉取配置中*/
        bool GetFetching();
        void SetFetching(bool fetching);

    Q_SIGNALS:
        void signalFetchingChanged();

    private Q_SLOTS:


    private:

        // 首页商品列表面板
        StackWarePanel* m_stackWarePanel = nullptr;

        // 首页商品拉取中
        bool m_fetching = true;
    };
} // namespace UI
