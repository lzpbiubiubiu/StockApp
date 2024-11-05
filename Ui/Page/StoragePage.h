#pragma once

#include "Page.h"

namespace UI
{
    class StoragePage : public Page
    {
        Q_OBJECT
        DECLARE_PAGE()

    public:
        Q_INVOKABLE StoragePage(QObject* parent = nullptr);

        virtual ~StoragePage();

    Q_SIGNALS:

    private Q_SLOTS:


    private:

    };
} // namespace UI
