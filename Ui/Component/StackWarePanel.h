
#pragma once

#include <QAbstractListModel>
#include "Core/StackStruct.h"

namespace UI
{
    /** 首页商品面板数据 */
    class StackWarePanelModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        StackWarePanelModel(QObject* parent);
        ~StackWarePanelModel();

        enum ItemRole
        {
            /** UUID */
            UUID_ROLE = Qt::UserRole + 1,

            /** 名称 */
            NAME_ROLE,

            /** 库存 */
            STACK_ROLE,

            /** 批发价格 */
            WHOLESALE_PRICE_ROLE,

            /** 零售价格 */
            RETAIL_PRICE_ROLE,

            /** 使能 */
            DISABLE_ROLE
        };

        /** 商品列表项 */
        struct StackWareItem
        {
            /** UUID */
            QString uuid;

            /** 商品名称 */
            QString name;

            /** 单品零售价格 */
            QString retailPrice;

            /** 单品批发价格 */
            QString wholesalePrice;

            /** 库存 */
            QString stock;

            /** 禁用 */
            bool disable = false;
        };

        using StackWareItemPtr = QSharedPointer<StackWarePanelModel::StackWareItem>;
        using StackWareItemPtrList = QList<QSharedPointer<StackWarePanelModel::StackWareItem>>;

        /** 设置列表数据 */
        void SetItems(const QList<StackWareItem>& items);

        /** 获取商品 */
        StackWareItem GetItem(int index) const;

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex& index, int role) const override;

        virtual QHash<int, QByteArray> roleNames() const override;


    private:
        // 商品列表
        QList<StackWareItem> m_items;

        // 当前修改商品的索引
        int m_currentIndex = 0;
    };

    /** 购物面板 StackWarePanel */
    class StackWarePanel : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QObject* model MEMBER m_modelData CONSTANT)
    public:
        StackWarePanel(QObject* parent = nullptr);
        ~StackWarePanel();

        /** 获取UI商品数据 */
        StackWarePanelModel::StackWareItem GetPanelItem(Core::WareItemPtr item) const;

        /** 获取数据模型 */
        StackWarePanelModel* GetModel() const;

        /** 初始化商品 */
        void InitWares();

    Q_SIGNALS:

    private:
        StackWarePanelModel* m_modelData = nullptr;
    };
}
