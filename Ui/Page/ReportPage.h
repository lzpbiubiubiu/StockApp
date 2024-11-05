#pragma once

#include "Page.h"
#include <QJsonArray>
namespace UI
{
    class ReportPage : public Page
    {
        Q_OBJECT
        Q_PROPERTY(bool querying READ GetQuerying NOTIFY signalQueryingChanged)
        Q_PROPERTY(QJsonArray dataModel READ GetDataModel NOTIFY signalDataModelChanged)
        Q_PROPERTY(QJsonArray dateTitleModel READ GetDateTitleModel NOTIFY signalDateTitleModelChanged)
        Q_PROPERTY(qint64 totalAmount READ GetTotalAmount NOTIFY signalTotalAmountChanged)
        DECLARE_PAGE()

    public:
        Q_INVOKABLE ReportPage(QObject* parent = nullptr);

        virtual ~ReportPage();

        /* 设置|获取订单报表状态*/
        void SetQuerying(bool querying);
        bool GetQuerying() const;

        /* 设置|获取报表日期标题列表*/
        void SetDateTitleModel(const QJsonArray& dateTitleModel);
        QJsonArray GetDateTitleModel() const;

        /* 设置|获取订单报表数据列表*/
        void SetDataModel(const QJsonArray& dataModel);
        QJsonArray GetDataModel() const;

        /* 设置|获取当前报表总金额*/
        qint64 GetTotalAmount() const;
        void SetTotalAmount(qint64 amount);
    Q_SIGNALS:
        void signalQueryingChanged();
        void signalDataModelChanged();
        void signalDateTitleModelChanged();
        void signalTotalAmountChanged();

    private Q_SLOTS:


    private:

        // 查询中
        bool m_querying = false;

        // 报表数据
        QJsonArray m_dataModel;

        // 报表日期标题数据
        QJsonArray m_dateTitleModel;

        // 当前报表总金额
        qint64 m_totalAmount = 0;
    };
} // namespace UI
