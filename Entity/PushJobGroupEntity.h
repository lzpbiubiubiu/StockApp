// Copyright (c) 2023 Dmall Pos Team

#pragma once

#include "ModuleData/ModuleDataApi.h"
#include "ModuleBase/Database/Base/EntityBase.h"

#define PUSH_JOB_TABLE_VERSION_V1 (1)
//#define PUSH_JOB_TABLE_VERSION_V1 (2)
//#define PUSH_JOB_TABLE_VERSION_V1 (3)

namespace Sql
{
    /** [tb_push_job] 离线任务表  */
    class MODULE_DATA_API PushJobGroupEntity : public Sql::EntityBase
    {
        Q_OBJECT

        ORM_REGISTER_FRIEND_CLASS(PushJobGroupEntity)

        ORM_REGISTER_TABLE("tb_push_job")

        // 任务唯一标识
        ORM_REGISTER_COLUMN(QString, UniqueId, "unique_id")

        // 商家ID
        ORM_REGISTER_COLUMN(QString, VenderId, "vender_id")

        // 门店ID
        ORM_REGISTER_COLUMN(QString, StoreId, "store_id")

        // 设备ID
        ORM_REGISTER_COLUMN(QString, DeviceId, "device_id")

        // 任务数据
        ORM_REGISTER_COLUMN(QString, Data, "data")

        // 创建时间
        ORM_REGISTER_COLUMN(QDateTime, Time, "time")

        // 在线推送标志
        ORM_REGISTER_COLUMN(bool, onlinePushFlag, "online_push_flag")

        // 前置机推送标志
        ORM_REGISTER_COLUMN(bool, instoreServerPushFlag, "instore_server_push_flag")
    };

    using PushJobGroupEntityPtr = QSharedPointer<PushJobGroupEntity>;
    using PushJobGroupEntityPtrList = QList<QSharedPointer<PushJobGroupEntity>>;
}

using namespace Sql;
ORM_REGISTER_HPP_PVT_DATABASE(PushJobGroupEntity)