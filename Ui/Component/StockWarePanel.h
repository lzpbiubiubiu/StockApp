
#pragma once

#include <QAbstractListModel>
#include "Core/StackStruct.h"

namespace UI
{
    /** 首页商品面板数据 */
    class StockWarePanelModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        StockWarePanelModel(QObject* parent);
        ~StockWarePanelModel();

        enum ItemRole
        {
            /** UUID */
            UUID_ROLE = Qt::UserRole + 1,

            /** 商品码 */
            CODE_ROLE,

            /** 名称 */
            NAME_ROLE,

            /** 库存 */
            STACK_ROLE,

            /** 批发价格 */
            WHOLESALE_PRICE_ROLE,

            /** 零售价格 */
            RETAIL_PRICE_ROLE,

            /** 使能 */
            DISABLE_ROLE,

            /** 商品图片 */
            IMG_ROLE,
        };

        /** 商品列表项 */
        struct StockWareItem
        {
            /** UUID */
            QString uuid;

            /** 商品编码 */
            QString code;

            /** 商品名称 */
            QString name;

            /** 单品零售价格 */
            QString retailPrice;

            /** 单品批发价格 */
            QString wholesalePrice;

            /** 库存 */
            QString stock;

            /** 图片 */
            QString imgUrl;

            /** 禁用 */
            bool disable = false;
        };

        /** 设置列表数据 */
        void SetItems(const QList<StockWareItem>& items);

        /** 获取商品 */
        StockWareItem GetItem(int index) const;

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex& index, int role) const override;

        virtual QHash<int, QByteArray> roleNames() const override;


    private:
        // 商品列表
        QList<StockWareItem> m_items;

        // 当前修改商品的索引
        int m_currentIndex = 0;
    };

    /** 购物面板 StockWarePanel */
    class StockWarePanel : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QObject* model MEMBER m_modelData CONSTANT)
    public:
        StockWarePanel(QObject* parent = nullptr);
        ~StockWarePanel();

        /** 获取UI商品数据 */
        StockWarePanelModel::StockWareItem GetPanelItem(Core::WareItemPtr item) const;

        /** 获取数据模型 */
        StockWarePanelModel* GetModel() const;

        /** 初始化商品 */
        void InitWares();

        /** 根据URL获取本地商品图片 */
        Q_INVOKABLE QString loadLocalWare(const QString &uri);

    Q_SIGNALS:

    private:
        StockWarePanelModel* m_modelData = nullptr;
    };
}
