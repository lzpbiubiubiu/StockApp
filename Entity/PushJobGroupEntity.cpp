#include "PushJobGroupEntity.h"

ORM_REGISTER_CPP_PVT_DATABASE(PushJobGroupEntity)

namespace Orm
{
    template<>
    void register_class(QdOrmClassDao<PushJobGroupEntity>& t)
    {
        ORM_REGISTER_QD_TABLE(PushJobGroupEntity, PUSH_JOB_TABLE_VERSION_V1)
        OrmMetaMemberPtr member = nullptr;

        // PUSH_JOB_TABLE_VERSION_V1
        member = ORM_REGISTER_QD_ID(PushJobGroupEntity, UniqueId)
        member = ORM_REGISTER_QD_DATA(PushJobGroupEntity, VenderId, PUSH_JOB_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(PushJobGroupEntity, StoreId, PUSH_JOB_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(PushJobGroupEntity, DeviceId, PUSH_JOB_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(PushJobGroupEntity, Data, PUSH_JOB_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(PushJobGroupEntity, Time, PUSH_JOB_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(PushJobGroupEntity, onlinePushFlag, PUSH_JOB_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(PushJobGroupEntity, instoreServerPushFlag, PUSH_JOB_TABLE_VERSION_V1)
    }
}