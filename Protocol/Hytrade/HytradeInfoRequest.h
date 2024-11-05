#pragma once

#include "Protocol/StockAppRequest.h"
#include "Core/StackStruct.h"
namespace Net
{
    /** 结算算价请求，${domain}/stockServer/sale/hytradeInfo */
    class HytradeInfoRequest : public Net::StockAppRequest
    {
        Q_OBJECT
    public:

        HytradeInfoRequest(QObject* parent = nullptr);
        ~HytradeInfoRequest();

        /** 设置商品列表 */
        void SetWareList(const Core::WareItemPtrList& wares);

        /** 设置小票号 */
        void SetReceiptNo(const QString& receiptNo);
        
        /** 设置订单金额 */
        void SetOrderAmount(qint64 amount);

        /** 设置订单促销金额 */
        void SetTotalPromotionAmount(qint64 amount);

        /** 设置单品促销金额 */
        void SetWarePromotionAmount(qint64 amount);

        /** 设置订单整单促销金额 */
        void SetWholeOrderPromotionAmount(qint64 amount);

        /** 设置自主配送费 */
        void SetDeliveryFeesAmount(qint64 amount);

        /** 设置订单备注 */
        void SetOrderRemark(const QString& text);

        /** 设置订单类型 */
        void SetOrderType(int type);
    protected:
        QJsonArray MakeWareJsonParam(const Core::WareItemPtrList& wares);
        virtual void MakeJsonParam(QJsonObject& val) override;

    private:
        Core::WareItemPtrList m_wares;

        /** 小票号 */
        QString m_receiptNo;

        /** 订单金额 */
        qint64 m_orderAmount = 0;

        /** 订单促销金额 */
        qint64 m_totalPromotionAmount = 0;

        /** 订单单品促销金额 */
        qint64 m_warePromotionAmount = 0;

        /** 订单整单促销金额 */
        qint64 m_wholeOrderPromotionAmount = 0;

        /** 订单配送费 */
        qint64 m_deliveryFeesAmount = 0;

        /** 订单扩展数据 */
        QJsonObject m_extension;

        /** 订单类型0: 零售订单， 1：批发订单 */
        int m_type = 0;
    };
}
