#include "ManagePageController.h"
#include "Base/Common/ServiceManager.h"
#include "Base/Log/Log.h"
#include "Base/Util/JsonUtil.h"
#include "Application.h"
namespace UI
{
    ManagePageController::ManagePageController(Page* parent)
        : PageController(parent)
    {
        m_page = GetPage<ManagePage>();
        m_businessManager = Base::GetService<Core::BusinessManager>();
        connect(m_businessManager, &Core::BusinessManager::signalConfigWareStorageSuccess, this, &ManagePageController::onConfigWareStorageSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalConfigWareStorageError, this, &ManagePageController::onConfigWareStorageError);
    }

    ManagePageController::~ManagePageController()
    {
    }

    void ManagePageController::OnPageLoad()
    {
        LOG_INFO(QString("ManagePage => OnPageLoad()"));
    }

    void ManagePageController::OnPageShow()
    {
        LOG_INFO(QString("ManagePage => OnPageShow()"));
        InitPage();
    }

    void ManagePageController::OnPageHide()
    {
        LOG_INFO(QString("ManagePage => OnPageHide()"));
    }

    void ManagePageController::OnPageUnload()
    {
        LOG_INFO(QString("ManagePage => OnPageUnload()"));
    }


    void ManagePageController::InitPage()
    {

    }

    void ManagePageController::wareStorage(const QJsonObject& obj, int operate)
    {
        UI::Application::Get()->GetMainWindow()->ShowLoading();
        Core::BusinessManager::StorageOperate storageOperage = static_cast<Core::BusinessManager::StorageOperate>(operate);
        Core::WareItemPtr item = Core::WareItemPtr::create();
        item->code = obj["code"].toString();
        item->name = obj["name"].toString();
        item->stock = obj["stock"].toVariant().toLongLong();
        item->retailPrice = obj["retailPrice"].toVariant().toLongLong();
        item->wholesalePrice = obj["wholesalePrice"].toVariant().toLongLong();
        m_businessManager->ConfigWareStorage(item, storageOperage);
    }

    void ManagePageController::onConfigWareStorageSuccess()
    {
        UI::Application::Get()->GetMainWindow()->CloseLoading();
        UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("商品入库成功"), TOAST_SHORT_DELAY);
    }

    void ManagePageController::onConfigWareStorageError(const QString& message)
    {
        UI::Application::Get()->GetMainWindow()->CloseLoading();
        UI::Application::Get()->GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
    }

} // namespace UI
