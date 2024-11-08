﻿
#pragma once

#include <QObject>
#include "Ui/Page/Page.h"
#include "Application.h"

namespace UI
{
    class Page;
    class PageController : public QObject
    {
        Q_OBJECT
    public:
        friend class Page;
        PageController(Page* page);
        virtual ~PageController();

        /** 获取页面 */
        template<typename T = Page>
        T* GetPage() const
        {
            Q_ASSERT(m_page != nullptr);
            return qobject_cast<T*>(m_page);
        }

    protected:
        /** 页面加载时回调 */
        virtual void OnPageLoad();

        /** 页面显示前回调 */
        virtual void OnPageShow();

        /** 页面隐藏前回调 */
        virtual void OnPageHide();

        /** 页面卸载前回调 */
        virtual void OnPageUnload();

    protected:
        Page* m_page = nullptr;
    };
}
