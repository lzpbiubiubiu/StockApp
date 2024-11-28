#include "StockWarePanel.h"
#include <QDebug>
#include <QtMath>
#include "Base/Common/ServiceManager.h"
#include "Base/Util/PriceUtil.h"
#include "Base/Util/PathUtil.h"
#include "Core/BusinessManager.h"

namespace UI
{
    StockWarePanelModel::StockWarePanelModel(QObject* parent)
        : QAbstractListModel(parent)
    {}

    StockWarePanelModel::~StockWarePanelModel()
    {}

    void StockWarePanelModel::SetItems(const QList<StockWareItem>& items)
    {
        beginResetModel();
        m_items = items;
        endResetModel();
    }

    StockWarePanelModel::StockWareItem StockWarePanelModel::GetItem(int index) const
    {
        StockWareItem item;
        if(index >= 0 && index < m_items.size())
            item = m_items[index];
        return item;
    }


    int StockWarePanelModel::rowCount(const QModelIndex& parent) const
    {
        return m_items.size();
    }

    QVariant StockWarePanelModel::data(const QModelIndex& index, int role) const
    {
        auto& itemData = m_items[index.row()];
        QVariant value;
        switch(role)
        {
        case ItemRole::UUID_ROLE:
            value = itemData.uuid;
            break;
        case ItemRole::CODE_ROLE:
            value = itemData.code;
            break;
        case ItemRole::NAME_ROLE:
            value = itemData.name;
            break;
        case ItemRole::STACK_ROLE:
            value = itemData.stock;
            break;
        case ItemRole::WHOLESALE_PRICE_ROLE:
            value = itemData.wholesalePrice;
            break;
        case ItemRole::RETAIL_PRICE_ROLE:
            value = itemData.retailPrice;
            break;
        case ItemRole::DISABLE_ROLE:
            value = itemData.disable;
            break;
        case ItemRole::IMG_ROLE:
            value = itemData.imgUrl;
            break;
        default:
            break;
        }
        return value;
    }

    QHash<int, QByteArray> StockWarePanelModel::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[ItemRole::UUID_ROLE] = "uuid";
        roles[ItemRole::CODE_ROLE] = "code";
        roles[ItemRole::NAME_ROLE] = "name";
        roles[ItemRole::STACK_ROLE] = "stock";
        roles[ItemRole::WHOLESALE_PRICE_ROLE] = "wholesalePrice";
        roles[ItemRole::RETAIL_PRICE_ROLE] = "retailPrice";
        roles[ItemRole::DISABLE_ROLE] = "disable";
        roles[ItemRole::IMG_ROLE] = "imgUrl";
        return roles;
    }

    StockWarePanel::StockWarePanel(QObject* parent)
        : QObject(parent)
    {
        m_modelData = new StockWarePanelModel(this);
    }

    StockWarePanel::~StockWarePanel()
    {

    }

    StockWarePanelModel* StockWarePanel::GetModel() const
    {
        return m_modelData;
    }

    StockWarePanelModel::StockWareItem StockWarePanel::GetPanelItem(Core::WareItemPtr item) const
    {
        StockWarePanelModel::StockWareItem wareItem;
        wareItem.name = item->name;
        wareItem.code = item->code;
        wareItem.wholesalePrice = Base::PriceUtil::FenToString(item->wholesalePrice);
        wareItem.retailPrice = Base::PriceUtil::FenToString(item->retailPrice);
        wareItem.uuid = item->uuid;
        wareItem.stock = QString::number(item->stock);
        wareItem.imgUrl = item->imageUrl;
        wareItem.disable = (0 == item->stock);
        return wareItem;
    }

    void StockWarePanel::InitWares()
    {
        QList<StockWarePanelModel::StockWareItem> wareItemList;
        for(auto& item : Base::GetService<Core::BusinessManager>()->GetConfigWares())
        {
            wareItemList << GetPanelItem(item);
        }
        m_modelData->SetItems(wareItemList);
    }

    QString StockWarePanel::loadLocalWare(const QString &uri)
    {
        QString path = Base::PathUtil::GetAssetsDir().append("wareImage/");
        QUrl url(uri);
        if(url.isValid())
        {
            QString fileName = url.fileName();
            if(QFile::exists(path + fileName))
            {
                return "file:///" + path + fileName;
            }
        }
        return "qrc:/Resources/Images/default_ware.svg";
    }
}
