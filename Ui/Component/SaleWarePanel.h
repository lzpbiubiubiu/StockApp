
#pragma once

#include <QAbstractListModel>
#include "Core/StackStruct.h"
#include "Core/BusinessManager.h"
namespace UI
{
    /** 出库购物车商品面板数据 */
    class SaleWarePanelModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        SaleWarePanelModel(QObject* parent);
        ~SaleWarePanelModel();

        enum ItemRole
        {
            /** UUID */
            UUID_ROLE = Qt::UserRole + 1,

            /** 名称 */
            NAME_ROLE,

            /** 数量 */
            COUNT_ROLE,

            /** 价格 */
            PRICE_ROLE,

            /** 库存 */
            STOCK_ROLE,

            /** 使能 */
            DISABLE_ROLE,

            /** 赠品 */
            GIFTS_ROLE,

            /** 商品图片 */
            IMG_ROLE
        };

        /** 商品列表项 */
        struct SaleWareItem
        {
            /** UUID */
            QString uuid;

            /** 商品名称 */
            QString name;

            /** 数量 */
            QString count;

            /** 原价 */
            QString price;

            /** 库存 */
            QString stock;

            /** 赠品 */
            bool gifts = false;

            /** 图片 */
            QString imgPath;

            /** 是否禁用 */
            bool disable = false;
        };

        /** 设置列表数据 */
        void SetItems(const QList<SaleWareItem>& items);

        /** 获取商品 */
        SaleWareItem GetItem(int index) const;

        /** 添加商品 */
        void AddItem(const SaleWareItem& item);

        /** 删除商品 */
        void DeleteItem(int index);

        /** 修改商品 */
        void UpdateItem(int index, const Core::WareItemPtr item);

        /** 清空商品 */
        void ClearItems();

        /** 通过url获取本地图片路径 */
        QString LoadLocalWare(const QString &uri);

        /** 获取商品列表 */
        const QList<SaleWareItem>& GetSaleWares() const;

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex& index, int role) const override;

        virtual QHash<int, QByteArray> roleNames() const override;

        /** 通过uuid找index */
        Q_INVOKABLE int getIndex(const QString& uuid);
    private:
        // 商品列表
        QList<SaleWareItem> m_items;

        // 当前修改商品的索引
        int m_currentIndex = 0;
    };

    /** 购物面板 SaleWarePanel */
    class SaleWarePanel : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QObject* model MEMBER m_modelData CONSTANT)
    public:
        SaleWarePanel(QObject* parent = nullptr);
        ~SaleWarePanel();

        /** 获取UI商品数据 */
        SaleWarePanelModel::SaleWareItem GetPanelItem(Core::WareItemPtr item) const;

        /** 获取数据模型 */
        SaleWarePanelModel* GetModel() const;

        /** 添加商品 */
        void OnAddItem(Core::WareItemPtr item);

        /** 修改商品数量 */
        void OnUpdateItem(int index, const Core::WareItemPtr item);

        /** 删除商品 */
        void OnDeleteItem(int index);

        /** 更新商品禁用状态 */
        void OnUpdateItemDisable(const Core::WareItemPtr item);

        /** 合行 */
        void OnMergeItem(const Core::WareItemPtr item);

        /** 初始化商品 */
        void InitWares();

        /** 获取异常商品列表 */
        QList<SaleWarePanelModel::SaleWareItem> GetTradeErrorWares();

    Q_SIGNALS:

    private:
        // model数据
        SaleWarePanelModel* m_modelData = nullptr;
    };
}
