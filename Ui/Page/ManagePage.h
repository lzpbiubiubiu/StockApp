#pragma once

#include "Page.h"
#include "Ui/Component/OrderStockListPanel.h"
namespace UI
{
    class ManagePage : public Page
    {
        Q_OBJECT
        Q_PROPERTY(QObject* orderStockListPanel MEMBER m_orderStockListPanel CONSTANT)
        DECLARE_PAGE()

    public:
        Q_INVOKABLE ManagePage(QObject* parent = nullptr);

        virtual ~ManagePage();

    Q_SIGNALS:

    private Q_SLOTS:


    private:

        // 备货清单列表面板
        OrderStockListPanel* m_orderStockListPanel = nullptr;
    };
} // namespace UI
