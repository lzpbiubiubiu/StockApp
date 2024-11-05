#include "SequenceEntity.h"

ORM_REGISTER_CPP_STOCK_DATABASE(SequenceEntity)

namespace Orm
{
    template<>
    void register_class(QdOrmClassDao<SequenceEntity>& t)
    {
        ORM_REGISTER_QD_TABLE(SequenceEntity, SEQUENCE_TABLE_VERSION_V1)
        OrmMetaMemberPtr member = nullptr;

        // SEQUENCE_TABLE_VERSION_V1
        member = ORM_REGISTER_QD_ID(SequenceEntity, Name)
        member = ORM_REGISTER_QD_DATA(SequenceEntity, Sequence, SEQUENCE_TABLE_VERSION_V1)
        member = ORM_REGISTER_QD_DATA(SequenceEntity, Timestamp, SEQUENCE_TABLE_VERSION_V1)
    }
}
