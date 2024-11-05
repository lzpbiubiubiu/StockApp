#include "StackWarePanel.h"
#include <QDebug>
#include <QtMath>
#include "Base/Common/ServiceManager.h"
#include "Base/Util/PriceUtil.h"
#include "Core/BusinessManager.h"

namespace UI
{
    StackWarePanelModel::StackWarePanelModel(QObject* parent)
        : QAbstractListModel(parent)
    {}

    StackWarePanelModel::~StackWarePanelModel()
    {}

    void StackWarePanelModel::SetItems(const QList<StackWareItem>& items)
    {
        beginResetModel();
        m_items = items;
        endResetModel();
    }

    StackWarePanelModel::StackWareItem StackWarePanelModel::GetItem(int index) const
    {
        StackWareItem item;
        if(index >= 0 && index < m_items.size())
            item = m_items[index];
        return item;
    }


    int StackWarePanelModel::rowCount(const QModelIndex& parent) const
    {
        return m_items.size();
    }

    QVariant StackWarePanelModel::data(const QModelIndex& index, int role) const
    {
        auto& itemData = m_items[index.row()];
        QVariant value;
        switch(role)
        {
        case ItemRole::UUID_ROLE:
            value = itemData.uuid;
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
        default:
            break;
        }
        return value;
    }

    QHash<int, QByteArray> StackWarePanelModel::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[ItemRole::UUID_ROLE] = "uuid";
        roles[ItemRole::NAME_ROLE] = "name";
        roles[ItemRole::STACK_ROLE] = "stock";
        roles[ItemRole::WHOLESALE_PRICE_ROLE] = "wholesalePrice";
        roles[ItemRole::RETAIL_PRICE_ROLE] = "retailPrice";
        roles[ItemRole::DISABLE_ROLE] = "disable";
        return roles;
    }

    StackWarePanel::StackWarePanel(QObject* parent)
        : QObject(parent)
    {
        m_modelData = new StackWarePanelModel(this);
    }

    StackWarePanel::~StackWarePanel()
    {

    }

    StackWarePanelModel* StackWarePanel::GetModel() const
    {
        return m_modelData;
    }

    StackWarePanelModel::StackWareItem StackWarePanel::GetPanelItem(Core::WareItemPtr item) const
    {
        StackWarePanelModel::StackWareItem wareItem;
        wareItem.name = item->name;
        wareItem.wholesalePrice = Base::PriceUtil::FenToString(item->wholesalePrice);
        wareItem.retailPrice = Base::PriceUtil::FenToString(item->retailPrice);
        wareItem.uuid = item->uuid;
        wareItem.stock = QString::number(item->stock);
        wareItem.disable = (0 == item->stock);
        return wareItem;
    }

    void StackWarePanel::InitWares()
    {
        QList<StackWarePanelModel::StackWareItem> wareItemList;
        for(auto& item : Base::GetService<Core::BusinessManager>()->GetConfigWares())
        {
            wareItemList << GetPanelItem(item);
        }
        m_modelData->SetItems(wareItemList);
    }
}
