#include "StoragePageController.h"
#include "Base/Common/ServiceManager.h"
#include "Base/Log/Log.h"
#include "Base/Util/JsonUtil.h"
#include "Application.h"
namespace UI
{
    StoragePageController::StoragePageController(Page* parent)
        : PageController(parent)
    {
        m_page = GetPage<StoragePage>();
        m_businessManager = Base::GetService<Core::BusinessManager>();
        connect(m_businessManager, &Core::BusinessManager::signalConfigWareStorageSuccess, this, &StoragePageController::onConfigWareStorageSuccess);
        connect(m_businessManager, &Core::BusinessManager::signalConfigWareStorageError, this, &StoragePageController::onConfigWareStorageError);
    }

    StoragePageController::~StoragePageController()
    {
    }

    void StoragePageController::OnPageLoad()
    {
        LOG_INFO(QString("StoragePage => OnPageLoad()"));
    }

    void StoragePageController::OnPageShow()
    {
        LOG_INFO(QString("StoragePage => OnPageShow()"));
        InitPage();
    }

    void StoragePageController::OnPageHide()
    {
        LOG_INFO(QString("StoragePage => OnPageHide()"));
    }

    void StoragePageController::OnPageUnload()
    {
        LOG_INFO(QString("StoragePage => OnPageUnload()"));
    }


    void StoragePageController::InitPage()
    {

    }

    void StoragePageController::wareStorage(const QJsonObject& obj, int operate)
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

    void StoragePageController::onConfigWareStorageSuccess()
    {
        UI::Application::Get()->GetMainWindow()->CloseLoading();
        UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("商品入库成功"), TOAST_SHORT_DELAY);
    }

    void StoragePageController::onConfigWareStorageError(const QString& message)
    {
        UI::Application::Get()->GetMainWindow()->CloseLoading();
        UI::Application::Get()->GetMainWindow()->ShowToast(message, TOAST_SHORT_DELAY);
    }

} // namespace UI
