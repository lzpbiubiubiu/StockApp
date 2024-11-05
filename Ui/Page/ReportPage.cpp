#include "Controller/ReportPageController.h"
#include "ReportPage.h"

namespace UI
{
    IMPLEMENT_PAGE("reportPage", ReportPage)

    ReportPage::ReportPage(QObject* parent/* = nullptr*/)
        : Page(parent)
    {
        SetContentItem("qrc:/Qml/Page/ReportPage.qml");
        SetController(new ReportPageController(this));
    }

    ReportPage::~ReportPage()
    {
    }

    void ReportPage::SetQuerying(bool querying)
    {
        if(m_querying != querying)
        {
           m_querying =  querying;
           emit signalQueryingChanged();
        }
    }

    bool ReportPage::GetQuerying() const
    {
        return m_querying;
    }

    void ReportPage::SetDateTitleModel(const QJsonArray& dateTitleModel)
    {
        m_dateTitleModel = dateTitleModel;
        emit signalDateTitleModelChanged();
    }

    QJsonArray ReportPage::GetDateTitleModel() const
    {
        return m_dateTitleModel;
    }

    void ReportPage::SetDataModel(const QJsonArray& dataModel)
    {
        m_dataModel = dataModel;
        emit signalDataModelChanged();
    }

    QJsonArray ReportPage::GetDataModel() const
    {
        return m_dataModel;
    }

    qint64 ReportPage::GetTotalAmount() const
    {
        return m_totalAmount;
    }

    void ReportPage::SetTotalAmount(qint64 amount)
    {
        if(m_totalAmount != amount)
        {
            m_totalAmount = amount;
            emit signalTotalAmountChanged();
        }
    }
} // namespace UI
