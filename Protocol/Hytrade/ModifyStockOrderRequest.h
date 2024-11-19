#pragma once

#include "Protocol/StockAppRequest.h"

namespace Net
{
    /** 备货订单修改请求，${domain}/stockApp/sale/modifyStockOrder */
    class ModifyStockOrderRequest : public StockAppRequest
    {
    public:
        ModifyStockOrderRequest(QObject* parent = Q_NULLPTR);
        ~ModifyStockOrderRequest();

        /** 设置订单操作类型 */
        void SetOperate(int operate);

        /** 设置小票号 */
        void SetOrderNo(const QString& orderNo);

        /** 设置订单uuid */
        void SetOrderUuid(const QString& orderUuid);

        /** 备货订单明细 */
        void SetStockOrderDetail(const QJsonObject& stockOrderDetail);
    protected:
        virtual void MakeJsonParam(QJsonObject& val) override;

    private:

        // 备货订单操作类型operate, 1:订单出库 2:修改订单 3:作废订单
        int m_operate = 1;

        // 订单小票号
        QString m_orderNo;

        // 订单uuid
        QString m_orderUuid;

        // 订单金额
        qint64 m_orderAmount = 0;

        // 备货订单明细
        QJsonObject m_stockOrderDetail;
    };
}
