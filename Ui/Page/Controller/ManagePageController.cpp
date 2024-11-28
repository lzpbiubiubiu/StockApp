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
        Core::BusinessManager::StorageOperate storageOperage = static_cast<Core::BusinessManager::StorageOperate>(operate);
        Core::WareItemPtr item = Core::WareItemPtr::create();

        bool enableUpdate =false;
        item->code = obj["code"].toString();
        if(obj.contains("name"))
        {
            item->name = obj["name"].toString();
            enableUpdate = true;
        }

        if(obj.contains("stock"))
        {
            item->stock = obj["stock"].toVariant().toLongLong();
            if(0 == item->stock)
            {
                UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("商品库存不能为0"), TOAST_SHORT_DELAY);
                return;
            }
            enableUpdate = true;
        }

        if(obj.contains("retailPrice"))
        {
            item->retailPrice = obj["retailPrice"].toVariant().toLongLong();
            if(0 == item->retailPrice)
            {
                UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("商品零售价不能为0"), TOAST_SHORT_DELAY);
                return;
            }
            enableUpdate = true;
        }

        if(obj.contains("wholesalePrice"))
        {
            item->wholesalePrice = obj["wholesalePrice"].toVariant().toLongLong();
            if(0 == item->wholesalePrice)
            {
                UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("商品批发价不能为0"), TOAST_SHORT_DELAY);
                return;
            }
            enableUpdate = true;
        }

        if(Core::BusinessManager::StorageOperate::WARE_UPDATE == storageOperage && !enableUpdate)
        {
            UI::Application::Get()->GetMainWindow()->ShowToast(QStringLiteral("更新商品必须填写一个值"), TOAST_SHORT_DELAY);
            return;
        }
        UI::Application::Get()->GetMainWindow()->ShowLoading();
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
