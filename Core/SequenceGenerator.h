#pragma once

#include <QReadWriteLock>
#include "Base/Common/ServiceBase.h"
#include "Base/Database/Base/DmlBase.h"
#include "Entity/SequenceEntity.h"

namespace Core
{
    /**
     * 序号生成器服务，实现流水号、小票号的生成
     *
     * 出库小票号：
     * N位门店号 + 3位机台号(000~999)+ 12位时间戳(yyMMddHHmmss) + 2位userId + 4位流水号（0000~9999）
     *
     * 入库小票号：
     * 数字1 + N位门店号 + 3位机台号(000~999) + 12位时间戳(yyMMddHHmmss) + 3位流水号（000~999）
     *
     */
    class SequenceGenerator : public Base::ServiceBase
    {
        Q_OBJECT
    public:
        /** 序号名称 */
        enum SequenceName
        {
            /** 出库 */
            SALE = 0,

            /** 入库 */
            RETURN,
        };
        Q_ENUM(SequenceName)

        /** 重置策略 */
        enum ResetPolicy
        {
            /** 跨天 */
            DAILY = 1,

            /** 跨月 */
            MONTHLY = 2,

            /** 跨年 */
            YEARLY = 4,

            /** 最大值 */
            MAXIMUM = 8
        };
        Q_DECLARE_FLAGS(ResetPolicys, ResetPolicy)

        SequenceGenerator(QObject* parent = nullptr);
        ~SequenceGenerator();

        /** 设置流水号重置策略 */
        void SetResetPolicy(SequenceName seqName, ResetPolicys policys);

        /** 设置流水号最大值（重置策略包含MAXIMUM时有效） */
        void SetMaximum(SequenceName seqName, qint64 maximum);

        /** 获取当前流水号 */
        QString GetSequence(SequenceName seqName);

        /** 滚动当前流水号 */
        QString Next(SequenceName seqName);

        /** 生成小票号 */
        QString GetReceiptNumber(SequenceName seqName);

    Q_SIGNALS:
        /** 流水号滚动信号 */
        void signalRollOver(int seqName);

        /** 流水号重置信号 */
        void signalReset(int seqName);

    protected:
        /** 流水号是否有效 */
        bool IsValid(SequenceName seqName) const;

        /** 流水号滚动 */
        qint64 Roll(SequenceName seqName);

        /** 重置流水号 */
        qint64 Reset(SequenceName seqName);

        /** 格式化流水号 */
        QString Format(qint64 sequence) const;

        virtual bool OnStart() override;

        virtual void OnStop() override;

    private:
        // 序号单元
        struct SequenceUnit
        {
            quint64 value = 1;
            quint64 maximum = 9999;
            QDateTime timestamp;
            ResetPolicys policys = DAILY;
        };

        // 序号数据
        QHash<SequenceName,SequenceUnit> m_sequenceUnits;

        // 操作锁
        QReadWriteLock m_rwLock;

        // Pos.sequence表操作对象
        Sql::DmlBase<Sql::SequenceEntity> m_dmlSequence;
    };
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Core::SequenceGenerator::ResetPolicys)
