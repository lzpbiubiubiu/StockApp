#pragma once

#include "Base/Database/Base/EntityBase.h"

#define SEQUENCE_TABLE_VERSION_V1 (1)
//#define SEQUENCE_TABLE_VERSION_V1 (2)
//#define SEQUENCE_TABLE_VERSION_V1 (3)

namespace Sql
{
    /** [tb_sequence] 流水号表  */
    class SequenceEntity : public Sql::EntityBase
    {
        Q_OBJECT

        ORM_REGISTER_FRIEND_CLASS(SequenceEntity)

        ORM_REGISTER_TABLE("tb_sequence")

        // 名称
        ORM_REGISTER_COLUMN(QString, Name, "name")

        // 序号
        ORM_REGISTER_COLUMN(qint64, Sequence, "sequence")

        // 时间戳
        ORM_REGISTER_COLUMN(QDateTime, Timestamp, "timestamp")
    };

    using SequenceEntityPtr = QSharedPointer<SequenceEntity>;
    using SequenceEntityPtrList = QList<QSharedPointer<SequenceEntity>>;
}

using namespace Sql;
