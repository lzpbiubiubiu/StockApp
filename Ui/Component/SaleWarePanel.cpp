#include "SaleWarePanel.h"
#include <QDebug>
#include <QtMath>
#include "Base/Common/ServiceManager.h"
#include "Base/Util/PriceUtil.h"
#include "Core/BusinessManager.h"

namespace UI
{
    SaleWarePanelModel::SaleWarePanelModel(QObject* parent)
        : QAbstractListModel(parent)
    {}

    SaleWarePanelModel::~SaleWarePanelModel()
    {
    }

    void SaleWarePanelModel::SetItems(const QList<SaleWareItem>& items)
    {
        beginResetModel();
        m_items = items;
        endResetModel();
    }

    SaleWarePanelModel::SaleWareItem SaleWarePanelModel::GetItem(int index) const
    {
        SaleWareItem item;
        if(index >= 0 && index < m_items.size())
            item = m_items[index];
        return item;
    }

    void SaleWarePanelModel::AddItem(const SaleWareItem& item)
    {
        if(m_items.isEmpty() || item.uuid != m_items.first().uuid)
        {
            // 插入
            beginInsertRows(QModelIndex(), 0, 0);
            m_items.prepend(item);
            endInsertRows();
        }
    }
    void SaleWarePanelModel::DeleteItem(int index)
    {
        if(index >= 0 && index < m_items.size())
        {
            beginRemoveRows(QModelIndex(), index, index);
            m_items.removeAt(index);
            endRemoveRows();
        }
    }

    void SaleWarePanelModel::UpdateItem(int index, const Core::WareItemPtr item)
    {
        if(index >= 0 && index < m_items.size())
        {
            auto& it = m_items[index];
            it.disable = (item->count > item->stock);
            it.stock = QString::number(item->stock);
            it.count = QString::number(item->count);
            it.price = Base::PriceUtil::FenToString(item->discountPrice);
            it.gifts = (Core::WareItem::WareType::WARE_TYPE_GIFT == item->wareType);
            auto modelIndex = QAbstractListModel::index(index);
            emit dataChanged(modelIndex, modelIndex);
        }
    }

    void SaleWarePanelModel::ClearItems()
    {
        beginResetModel();
        m_items.clear();
        endResetModel();
    }

    const QList<SaleWarePanelModel::SaleWareItem>& SaleWarePanelModel::GetSaleWares() const
    {
        return m_items;
    }

    int SaleWarePanelModel::rowCount(const QModelIndex& parent) const
    {
        return m_items.size();
    }

    QVariant SaleWarePanelModel::data(const QModelIndex& index, int role) const
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
        case ItemRole::DISABLE_ROLE:
            value = itemData.disable;
            break;
        case ItemRole::COUNT_ROLE:
            value = itemData.count;
            break;
        case ItemRole::PRICE_ROLE:
            value = itemData.price;
            break;
        case ItemRole::STOCK_ROLE:
            value = itemData.stock;
            break;
        case ItemRole::GIFTS_ROLE:
            value = itemData.gifts;
            break;
        default:
            break;
        }
        return value;
    }

    QHash<int, QByteArray> SaleWarePanelModel::roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles[ItemRole::UUID_ROLE] = "uuid";
        roles[ItemRole::NAME_ROLE] = "name";
        roles[ItemRole::COUNT_ROLE] = "count";
        roles[ItemRole::PRICE_ROLE] = "price";
        roles[ItemRole::STOCK_ROLE] = "stock";
        roles[ItemRole::DISABLE_ROLE] = "disable";
        roles[ItemRole::GIFTS_ROLE] = "gifts";
        return roles;
    }

    int SaleWarePanelModel::getIndex(const QString& uuid)
    {
        int index = -1;
        for(int i = 0; i < m_items.size(); i++)
        {
            if(m_items[i].uuid == uuid && m_items[i].count >= 0)
            {
                index = i;
                break;
            }
        }

        return index;
    }

    SaleWarePanel::SaleWarePanel(QObject* parent)
        : QObject(parent)
    {
        m_modelData = new SaleWarePanelModel(this);
    }

    SaleWarePanel::~SaleWarePanel()
    {

    }

    SaleWarePanelModel* SaleWarePanel::GetModel() const
    {
        return m_modelData;
    }

    SaleWarePanelModel::SaleWareItem SaleWarePanel::GetPanelItem(Core::WareItemPtr item) const
    {
        SaleWarePanelModel::SaleWareItem wareItem;
        wareItem.name = item->name;
        wareItem.price = Base::PriceUtil::FenToString(item->discountPrice);
        wareItem.uuid = item->uuid;
        wareItem.disable = (item->count > item->stock);
        wareItem.count = QString::number(item->count);
        wareItem.stock = QString::number(item->stock);
        wareItem.gifts = (Core::WareItem::WareType::WARE_TYPE_GIFT == item->wareType);
        return wareItem;
    }

    void SaleWarePanel::InitWares()
    {
        QList<SaleWarePanelModel::SaleWareItem> wareItemList;
        auto order = Base::GetService<Core::BusinessManager>()->GetCurrentSaleOrder();
        if(order)
        {
            for(auto& item : order->items)
            {
                wareItemList << GetPanelItem(item);
            }
            m_modelData->SetItems(wareItemList);
        }
        else
        {
            m_modelData->ClearItems();
        }

    }

    void SaleWarePanel::OnAddItem(Core::WareItemPtr item)
    {
        m_modelData->AddItem(GetPanelItem(item));
    }

    void SaleWarePanel::OnUpdateItem(int index, const Core::WareItemPtr item)
    {
        m_modelData->UpdateItem(index, item);
    }

    void SaleWarePanel::OnMergeItem(const Core::WareItemPtr item)
    {
        m_modelData->UpdateItem(m_modelData->getIndex(item->uuid), item);
    }

    void SaleWarePanel::OnDeleteItem(int index)
    {
        m_modelData->DeleteItem(index);
    }

    void SaleWarePanel::OnUpdateItemDisable(const Core::WareItemPtr item)
    {
        m_modelData->UpdateItem(m_modelData->getIndex(item->uuid), item);
    }

    QList<SaleWarePanelModel::SaleWareItem> SaleWarePanel::GetTradeErrorWares()
    {
        QList<SaleWarePanelModel::SaleWareItem> errorWares;
        auto& wares = m_modelData->GetSaleWares();
        for(auto item : wares)
        {
            if(item.disable)
                errorWares << item;
        }
        return errorWares;
    }
}
