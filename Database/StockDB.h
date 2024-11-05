
#pragma once
#include "Base/Database/Base/DBBase.h"

namespace Sql
{
    /** Stock App数据库 */
    class StockDB : public Sql::DBBase
    {
    public:
        virtual bool CreateTable() override;
    };
}
