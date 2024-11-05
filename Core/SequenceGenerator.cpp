#include <QReadLocker>
#include <QWriteLocker>
#include <QDateTime>
#include "SequenceGenerator.h"
#include "Base/Common/ServiceManager.h"
#include "Core/DatabaseManager.h"

namespace Core
{
    SequenceGenerator::SequenceGenerator(QObject* parent) : ServiceBase(parent)
    {
        SequenceUnit seq;
        seq.policys = ResetPolicy::DAILY;
        seq.timestamp = QDateTime::currentDateTime();
        m_sequenceUnits[SequenceName::SALE] = seq;
        m_sequenceUnits[SequenceName::RETURN] = seq;
    }

    SequenceGenerator::~SequenceGenerator() { }

    void SequenceGenerator::SetResetPolicy(SequenceName seqName, ResetPolicys policys)
    {
        QWriteLocker locker(&m_rwLock);
        m_sequenceUnits[seqName].policys = policys;
    }

    void SequenceGenerator::SetMaximum(SequenceName seqName, qint64 maximum)
    {
        QWriteLocker locker(&m_rwLock);
        if(m_sequenceUnits.contains(seqName))
            m_sequenceUnits[seqName].maximum = maximum;
    }

    QString SequenceGenerator::GetSequence(SequenceName seqName)
    {
        QReadLocker locker(&m_rwLock);
        qint64 sequence = 0;
        if(!IsValid(seqName))
        {
            auto dbm = Base::GetService<Core::DatabaseManager>();
            m_dmlSequence.SetDatabase(dbm->GetStockDatabase());
            sequence = Reset(seqName);
        }
        else
        {
            sequence = m_sequenceUnits[seqName].value;
        }
        return Format(sequence);
    }

    QString SequenceGenerator::Next(SequenceName seqName)
    {
        QWriteLocker locker(&m_rwLock);
        auto dbm = Base::GetService<Core::DatabaseManager>();
        m_dmlSequence.SetDatabase(dbm->GetStockDatabase());
        qint64 next = 0;
        if(!IsValid(seqName))
            next = Reset(seqName);
        else
            next = Roll(seqName);
        return Format(next);
    }

    QString SequenceGenerator::GetReceiptNumber(SequenceName seqName)
    {
        QString number;

        // N位门店号
        QString s0 = "2024";

        // 3位机台号
        QString d0 = "888";

        // 12位时间串
        QString t0 = QDateTime::currentDateTime().toString("yyMMddHHmmss");

        // 1位随机数：'0' - '9'
        QString r0 = QString::number(Base::MathUtil::GetRandomInt(0, 9));

        // 2位随机数：'00' - '99'
        QString r1 = QString("%1").arg(Base::MathUtil::GetRandomInt(0, 99), 2, 10, QLatin1Char('0'));

        number = s0 + d0 + t0 + r0 + r1 + GetSequence(seqName);
        return number;
    }


    bool SequenceGenerator::IsValid(SequenceName seqName) const
    {
        quint64 next = m_sequenceUnits[seqName].value + 1;
        QDate last = m_sequenceUnits[seqName].timestamp.date();
        QDate current = QDateTime::currentDateTime().date();

        // 最大值校验
        if(m_sequenceUnits[seqName].policys.testFlag(ResetPolicy::MAXIMUM))
        {
            if(next > m_sequenceUnits[seqName].maximum)
                return false;
        }

        // 跨天校验
        if(m_sequenceUnits[seqName].policys.testFlag(ResetPolicy::DAILY))
        {
            if(current.day() != last.day())
                return false;

            if(current.month() != last.month())
                return false;

            if(current.year() != last.year())
                return false;
        }

        // 跨月校验
        if(m_sequenceUnits[seqName].policys.testFlag(ResetPolicy::MONTHLY))
        {
            if(current.month() != last.month())
                return false;

            if(current.year() != last.year())
                return false;
        }

        // 跨年校验
        if(m_sequenceUnits[seqName].policys.testFlag(ResetPolicy::YEARLY))
        {
            if(current.year() != last.year())
                return false;
        }

        return true;
    }

    qint64 SequenceGenerator::Roll(SequenceName seqName)
    {
        qint64 next = ++m_sequenceUnits[seqName].value;
        QMetaEnum meta = QMetaEnum::fromType<SequenceName>();
        auto entity = Sql::SequenceEntityPtr::create();
        entity->SetName(meta.valueToKey(seqName));
        entity->SetSequence(next);
        entity->SetTimestamp(QDateTime::currentDateTime());
        QString sqlError;
        if(!m_dmlSequence.Save(entity, sqlError))
        {
            Q_ASSERT_X(0, "", sqlError.toUtf8());
            //LOG_ERROR("sql error: {}", sqlError.toUtf8());
        }
        emit signalRollOver(seqName);
        return next;
    }

    qint64 SequenceGenerator::Reset(SequenceName seqName)
    {
        QDateTime current = QDateTime::currentDateTime();
        m_sequenceUnits[seqName].value = 1;
        m_sequenceUnits[seqName].timestamp = current;

        QMetaEnum enumMeta = QMetaEnum::fromType<SequenceName>();
        auto entity = Sql::SequenceEntityPtr::create();
        entity->SetName(enumMeta.valueToKey(seqName));
        entity->SetSequence(1);
        entity->SetTimestamp(current);

        QString sqlError;
        if(!m_dmlSequence.Save(entity, sqlError))
        {
            Q_ASSERT_X(0, "", sqlError.toUtf8());
            //LOG_ERROR("sql error: {}", sqlError.toUtf8());
        }
        emit signalReset(seqName);
        return m_sequenceUnits[seqName].value;
    }

    QString SequenceGenerator::Format(qint64 sequence) const
    {
         return QString("%1").arg(sequence, 4, 10, QLatin1Char('0'));;
    }

    bool SequenceGenerator::OnStart()
    {
        auto dbm = Base::GetService<Core::DatabaseManager>();
        m_dmlSequence.SetDatabase(dbm->GetStockDatabase());

        Sql::SequenceEntityPtrList entities;
        QString sqlError;
        if(!m_dmlSequence.ListAll(entities, sqlError))
        {
            Q_ASSERT_X(0, "", sqlError.toUtf8());
            //LOG_ERROR("sql error: {}", sqlError.toUtf8());
            return false;
        }

        QMetaEnum enumMeta = QMetaEnum::fromType<SequenceName>();
        for(auto& i : entities)
        {
            int enumValue = enumMeta.keysToValue(i->GetName().toUtf8());
            if(enumValue != -1)
            {
                SequenceName seqName = static_cast<SequenceName>(enumValue);
                m_sequenceUnits[seqName].value = i->GetSequence();
                m_sequenceUnits[seqName].timestamp = i->GetTimestamp();

                if(!IsValid(seqName))
                    Reset(seqName);
            }
        }
        return true;
    }

    void SequenceGenerator::OnStop() { }
}
