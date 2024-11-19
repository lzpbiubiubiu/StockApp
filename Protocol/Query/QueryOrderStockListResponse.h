#pragma once

#include "Protocol/StockAppResponse.h"
#include "Core/StackStruct.h"

namespace Net
{
    /** 查询订单报表返回 */
    class QueryOrderStockListResponse : public Net::StockAppResponse
    {
        Q_OBJECT
    public:

        struct StockOrderDetail
        {
            /** 备货清单商品信息*/
            struct StockOrderWare
            {
                /** 商品编码*/
                QString code;

                /** 商品名称*/
                QString name;

                /** 商品小计金额*/
                qint64 amount = 0;

                /** 订单总优惠*/
                qint64 count = 0;

                /** 商品执行价*/
                qint64 discountPrice = 0;

                /** 商品原单价*/
                qint64 price = 0;

                /** 商品优惠金额*/
                qint64 promotionAmount = 0;

                /** 是否是赠品*/
                bool gifts = false;

                /** 商品扩展信息*/
                QString extension;
            };

            /** 小票号*/
            QString orderNo;

            /** 订单UUID*/
            QString orderUuid;

            /** 订单金额*/
            qint64 orderAmount = 0;

            /** 订单总优惠*/
            qint64 totalPromotionAmount = 0;

            /** 订单配送费*/
            qint64 deliveryFreeAmount = 0;

            /** 订单时间 */
            QString time;

            /** 商品列表*/
            QList<StockOrderWare> wares;

            /** 订单扩展信息*/
            QString extension;
        };

        struct Result
        {
            // 请求唯一id
            QString uniqueId;

            // 订单报表列表数据
            QList<StockOrderDetail> stockingOrders;
        };

        QueryOrderStockListResponse(QObject* parent = nullptr);
        ~QueryOrderStockListResponse();

        const Result& GetResult() const;

    protected:
        
        virtual void ParseJsonParam(const QJsonObject& val) override;

    private:
        Result m_result;
    };
}
