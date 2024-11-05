#pragma once

#include "Base/Database/Base/DBBase.h"

namespace Sql
{
    /** 业务数据库 */
    class BusinessDB : public Sql::DBBase
    {
    public:
        virtual bool CreateTable() override;
    };
}
