#pragma once

#include "PageController.h"
#include "Core/StackStruct.h"
#include "Base/Common/ServiceManager.h"
#include "Ui/Page/ManagePage.h"
#include "Core/BusinessManager.h"

namespace UI
{
    class ManagePageController : public PageController
    {
        Q_OBJECT

    public:
        ManagePageController(Page* parent);

        virtual ~ManagePageController();

        /** 商品入库 */
        Q_INVOKABLE void wareStorage(const QJsonObject& obj, int operate);
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
    private Q_SLOTS:

        /** 商品出库信号返回处理 */
        void onConfigWareStorageSuccess();
        void onConfigWareStorageError(const QString& message);

    private:
        // 页面
        ManagePage* m_page = nullptr;

        // 业务管理器
        Core::BusinessManager* m_businessManager = nullptr;
    };
}
