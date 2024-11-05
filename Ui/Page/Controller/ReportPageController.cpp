#include "ReportPageController.h"
#include "Base/Common/ServiceManager.h"
#include "Base/Log/Log.h"
#include "Base/Util/JsonUtil.h"
#include "Application.h"
namespace UI
{
    ReportPageController::ReportPageController(Page* parent)
        : PageController(parent)
    {
        m_page = GetPage<ReportPage>();
        m_businessManager = Base::GetService<Core::BusinessManager>();
        connect(m_businessManager, &Core::BusinessManager::signalQueryOrderReportSuccess, this, &ReportPageController::onQueryOrderReportSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalQueryOrderReportError, this, &ReportPageController::onQueryOrderReportError);
    }

    ReportPageController::~ReportPageController()
    {
    }

    void ReportPageController::OnPageLoad()
    {
        LOG_INFO(QString("ReportPage => OnPageLoad()"));
    }

    void ReportPageController::OnPageShow()
    {
        LOG_INFO(QString("ReportPage => OnPageShow()"));
        m_page->SetQuerying(true);
        m_businessManager->QueryOrderReport(QDateTime::currentDateTime());
    }

    void ReportPageController::OnPageHide()
    {
        LOG_INFO(QString("ReportPage => OnPageHide()"));
    }

    void ReportPageController::OnPageUnload()
    {
        LOG_INFO(QString("ReportPage => OnPageUnload()"));
    }


    void ReportPageController::InitPage()
    {

    }

    void ReportPageController::onQueryOrderReportSuccess(const Core::OrderReportPtrList& orderReports)
    {
        m_page->SetQuerying(false);

        m_orderReports = orderReports;
        ShowOrderReport(ReportPageController::ShowAction::MONTHLY_SHOW);
    }

    void ReportPageController::onQueryOrderReportError(const QString& message)
    {
        m_page->SetQuerying(false);
    }

    void ReportPageController::ShowOrderReport(ReportPageController::ShowAction action)
    {
        if(m_orderReports.isEmpty())
        {
            return;
        }

        Core::OrderReportPtrList orderReports = m_orderReports;
        if(ReportPageController::ShowAction::BYWEEK_SHOW == action)
        {
            if(m_orderReports.size() >= 7)
            {
                orderReports = m_orderReports.mid(orderReports.size() - 7);
            }
        }

        QJsonArray dataModel;
        QJsonArray dateTitleModel;
        auto orderSize = orderReports.size();
        // 计算间隔
        int specifiedInterval  = orderSize / 4;

        qint64 totalAmount = 0;
        for(int i = 0; i < orderSize; i++)
        {
            QJsonObject obj;
            auto date = QDate::fromString(orderReports[i]->date, "yyyy-MM-dd");
            obj["date"] = date.toString("yyyy-MM-dd");
            obj["amount"] = orderReports[i]->amount / 100.00;

            // 如果不均匀时取前四个均分的，最后一个取最后一个
            if(0 == i % specifiedInterval && dateTitleModel.size() < 4)
            {
                dateTitleModel.append(date.toString("MM-dd"));
            }

            if((orderSize - 1) == i)
            {
                dateTitleModel.append(date.toString("MM-dd"));
            }

            dataModel.append(obj);
            totalAmount += orderReports[i]->amount;
        }

        m_page->SetDateTitleModel(dateTitleModel);
        m_page->SetDataModel(dataModel);
        m_page->SetTotalAmount(totalAmount);
    }

    void ReportPageController::monthlyReport()
    {
        ShowOrderReport(ReportPageController::ShowAction::MONTHLY_SHOW);
    }

    Q_INVOKABLE void ReportPageController::byweekReport()
    {
        ShowOrderReport(ReportPageController::ShowAction::BYWEEK_SHOW);
    }

} // namespace UI
