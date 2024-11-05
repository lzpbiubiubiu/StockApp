#pragma once

#include "PageController.h"
#include "Core/StackStruct.h"
#include "Base/Common/ServiceManager.h"
#include "Ui/Page/ReportPage.h"
#include "Core/BusinessManager.h"

namespace UI
{

    class ReportPageController : public PageController
    {
        Q_OBJECT

    public:
        /** 展示操作 */
        enum ShowAction
        {
            /** 按月统计显示 */
            MONTHLY_SHOW = 0,

            /** 按周统计显示 */
            BYWEEK_SHOW
        };

        ReportPageController(Page* parent);

        virtual ~ReportPageController();

        /** 按月报表 */
        Q_INVOKABLE void monthlyReport();

        /** 按周报表 */
        Q_INVOKABLE void byweekReport();
    protected:
        /** 页面加载时回调 */
        virtual void OnPageLoad() override;

        /** 页面显示前回调 */
        virtual void OnPageShow() override;

        /** 页面隐藏前回调 */
        virtual void OnPageHide() override;

        /** 页面卸载前回调 */
        virtual void OnPageUnload() override;

        /** 初始化page页面 */
        void InitPage();

        /** 显示订单报表曲线 */
        void ShowOrderReport(ShowAction action);

    private Q_SLOTS:

        /** 处理订单报表查询返回信号 */
        void onQueryOrderReportSuccess(const Core::OrderReportPtrList& orderReports);
        void onQueryOrderReportError(const QString& message);

    private:
        // 页面
        ReportPage* m_page = nullptr;

        // 业务管理器
        Core::BusinessManager* m_businessManager = nullptr;

        // 当前查询的订单报表列表
        Core::OrderReportPtrList m_orderReports;
    };
}
