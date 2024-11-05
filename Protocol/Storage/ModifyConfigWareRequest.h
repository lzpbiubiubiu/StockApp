#pragma once

#include "Protocol/StockAppRequest.h"

namespace Net
{
    /** 入库请求，${domain}/stockApp/storage/modifyConfigWare */
    class ModifyConfigWareRequest : public StockAppRequest
    {
    public:
        ModifyConfigWareRequest(QObject* parent = Q_NULLPTR);
        ~ModifyConfigWareRequest();

        /** 设置商品编码 */
        void SetCode(const QString& code);

        /** 设置商品名称 */
        void SetName(const QString& name);

        /** 设置商品库存 */
        void SetStock(qint64 stock);

        /** 设置商品零售价 */
        void SetRetailPrice(qint64 price);

        /** 设置商品零售价 */
        void SetWholeSalePrice(qint64 price);

        /** 设置入库操作类型 */
        void SetOperate(int operate);
    protected:
        virtual void MakeJsonParam(QJsonObject& val) override;

    private:

        // 商品编码
        QString m_wareCode;

        // 商品名称
        QString m_wareName;

        // 商品库存
        qint64 m_stock = 0;

        // 商品零售价
        qint64 m_retailPrice = 0;

        // 商品批发价
        qint64 m_wholeSalePrice = 0;

        // 入库操作类型operate, 1:新增 2:更新
        int m_operate = 1;
    };
}
