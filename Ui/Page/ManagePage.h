#pragma once

#include "Page.h"

namespace UI
{
    class ManagePage : public Page
    {
        Q_OBJECT
        DECLARE_PAGE()

    public:
        Q_INVOKABLE ManagePage(QObject* parent = nullptr);

        virtual ~ManagePage();

    Q_SIGNALS:

    private Q_SLOTS:


    private:

    };
} // namespace UI
