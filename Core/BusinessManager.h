#pragma once

#include "Base/Common/ServiceBase.h"
#include "Core/StackStruct.h"
#include "Dml/OrderDml.h"
namespace Core
{
    /** 业务功能服务，实现拉取库存，入库，出库等功能管理 */
    class BusinessManager : public Base::ServiceBase
    {
        Q_OBJECT
    public:

        /** 商品操作 */
        enum ItemAction
        {
            /** 商品修改数量 */
            UPDATE_COUNT = 0,

            /** 商品加数量 */
            ADD_COUNT,

            /** 商品减数量 */
            REDUCE_COUNT,

            /** 修改执行价 */
            UPDATE_DISCOUNT_PRICE,

            /** 更新为赠品 */
            UPDATE_PRESENTATION
        };
        Q_ENUM(ItemAction)

        /** 商品操作 */
        enum StorageOperate
        {
            /** 商品加数量 */
            WARE_ADD = 0,

            /** 商品修改 */
            WARE_UPDATE,
        };
        Q_ENUM(StorageOperate)

        /** 备货订单操作类型 */
        enum StockOrderOperate
        {
            /** 订单出库 */
            ORDER_OUTBOUND = 1,

            /** 订单修改 */
            ORDER_MODIFY = 2,

            /** 订单作废 */
            ORDER_VOID = 3
        };
        Q_ENUM(StockOrderOperate)

        BusinessManager(QObject* parent = nullptr);
        virtual ~BusinessManager();

        /** 拉取首页商品列表 */
        void FetchConfigWares();

        /** 登录接口 */
        void Login(const QString& account, const QString& password);

        /** 获取商品 */
        Core::WareItemPtr CreatorConfigWare(int index);

        /** 获取首页商品列表 */
        const Core::WareItemPtrList& GetConfigWares() const;

        /** 获取当前出库订单（出库购物车为空则返回空指针） */
        Core::OrderPtr GetCurrentSaleOrder();

        /** 创建商品 */
        Core::WareItemPtr CreatorWareItem(const QString& uuid);

        /** 增加出库商品 */
        void AddSaleWareItem(Core::WareItemPtr item, Core::Order::OrderType type);

        /** 增加赠品 */
        void AddPresentationWareItem(Core::WareItemPtr item);

        /** 更新出库商品 */
        void UpdateSaleWareItem(int index, ItemAction action, const QVariantList& values = QVariantList());

        /** 删除出库商品 */
        void DeleteSaleWareItem(int index);

        /** 提交出库订单 */
        void SubmitSaleOrder(qint64 wholeOrderPromoAmt, qint64 deliveryFeesAmt, const QString& remarkText);

        /** 订单整单改价提交 */
        void OrderChangeSubmit(qint64 wholeOrderChangeAmt, qint64 deliveryFeesAmt, const QString& remarkText);

        /** 更新app缓存 */
        void UpdateAppCache(const QString& ip, int port);

        /** 商品入库 */
        void ConfigWareStorage(Core::WareItemPtr item, StorageOperate operate = StorageOperate::WARE_ADD);

        /** 订单报表查询 */
        void QueryOrderReport(const QDateTime& date);

        /** 备货清单查询 */
        void QueryOrderStockList();

        /** 备货订单修改 */
        void ModifyStockOrder(Core::StockOrderDetailPtr stockOrder, StockOrderOperate operateType);
    protected:
        /** 创建新订单 */
        OrderPtr CreateOrder() const;

        virtual bool OnStart() override;

        virtual void OnStop() override;

        /** 恢复异常订单 */
        void RestoreOrder();

        /** 用户账号信息恢复 */
        void RestoreLoginUser();

        /** 写App.json */
        bool WriteAppCache(const QString& fileName, const QString& ip = "127.0.0.1", int port = 9090) const;

        /** 读App.json */
        bool ReadAppCache(const QString& fileName) const;

        /** 修改商品数量 */
        bool DoUpdateSaleItemCount(WareItemPtr item, int count, QString& message);

        /** 修改商品执行价数据 */
        bool DoUpdateSaleItemDiscountPriceData(WareItemPtr item, qint64 discountPrice, QString& message);

        /** 更新商品为赠品 */
        bool DoUpdateItemPresentationData(WareItemPtr item, QString& message);

        /** 订单完成 */
        bool FinishOrder(QString& message);

        Q_INVOKABLE void onFetchConfigWares();
        Q_INVOKABLE void onLogin(const QString& account, const QString& password);
        Q_INVOKABLE void onAddSaleWareItem(Core::WareItemPtr item, Core::Order::OrderType type);
        Q_INVOKABLE void onAddPresentationWareItem(Core::WareItemPtr item);
        Q_INVOKABLE void onUpdateSaleWareItem(int index, ItemAction action, const QVariantList& values);
        Q_INVOKABLE void onDeleteSaleWareItem(int index);
        Q_INVOKABLE void onSubmitSaleOrder(qint64 wholeOrderPromoAmt, qint64 deliveryFeesAmt, const QString& remarkText);
        Q_INVOKABLE void onOrderChangeSubmit(qint64 wholeOrderChangeAmt, qint64 deliveryFeesAmt, const QString& remarkText);
        Q_INVOKABLE void onUpdateAppCache(const QString& ip, int port);
        Q_INVOKABLE void onConfigWareStorage(Core::WareItemPtr item, StorageOperate operate);
        Q_INVOKABLE void onQueryOrderReport(const QDateTime& date);
        Q_INVOKABLE void onQueryOrderStockList();
        Q_INVOKABLE void onModifyStockOrder(Core::StockOrderDetailPtr stockOrder, StockOrderOperate operateType);
    Q_SIGNALS:
        /** 拉取配置商品成功|失败信号 */
        void signalLoadConfigWaresSuccess();
        void signalLoadConfigWaresError(const QString& message);

        /** 登录信号 */
        void signalLoginSuccess();
        void signalLoginError(const QString& message);

        /** 增加商品出库成功|失败信号 */
        void signalAddSaleWaresSuccess(Core::WareItemPtr item);
        void signalAddSaleWaresError(const QString& message);

        /** 更新出库商品信号 */
        void signalUpdateSaleItemSuccess(int index, Core::WareItemPtr item);
        void signalUpdateSaleItemError(const QString& message);

        /** 更新出库商品合行信号 */
        void signalSaleMergeItemSuccess(Core::WareItemPtr item);
        void signalSaleMergeItemError(const QString& message);

        /** 删除出库商品 */
        void signalDeletedSaleItem(int index);

        /** 异常恢复订单信号 */
        void signalRestoreOrderSuccess();

        /** 提交出库订单信号 */
        void signalSubmitSaleOrderSuccess();
        void signalSubmitSaleOrderError(const QString& message);

        /** 恢复上次登录的用户 */
        void signalRestoreUserAccount(const QString& account);

        /** 商品入库信号 */
        void signalConfigWareStorageSuccess();
        void signalConfigWareStorageError(const QString& message);

        /** 赠送商品信号 */
        void signalPresentationWareSuccess(int index, Core::WareItemPtr item);
        void signalPresentationWareError(const QString& message);

        /** 订单报表查询信号 */
        void signalQueryOrderReportSuccess(const Core::OrderReportPtrList& orderReports);
        void signalQueryOrderReportError(const QString& message);

        /** 备货清单查询信号 */
        void signalQueryOrderStockListSuccess(const Core::StockOrderDetailPtrList& stockOrders);
        void signalQueryOrderStockListError(const QString& message);

        /** 备货订单修改 */
        void signalModifyStockOrderSuccess();
        void signalModifyStockOrderError(const QString& message, StockOrderOperate operateType);
    private:

        // 数据库操作对象
        Sql::OrderDml m_orderDml;

        // 首页商品列表
        Core::WareItemPtrList m_configWares;

        // 出库订单
        Core::OrderPtr m_saleOrder = nullptr;

        // 出库商品code查找表
        QHash<QString, WareItemPtr> m_codeSaleLookup;

        // 赠送商品code查找表
        QHash<QString, WareItemPtr> m_codePresentationLookup;

    };
}
