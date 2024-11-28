#pragma once

#include "Protocol/StockAppResponse.h"

namespace Net
{
    /** 心跳请求，${domain}/stockServer/terminal/getConfigwares */
    class TerminalGetConfigwaresResponse : public StockAppResponse
    {
        struct ConfigWare
        {
            /** 名称 */
            QString name;

            /** 商品编码 */
            QString code;

            /** 库存 */
            qint64 stock = 0;

            /** 单品零售价格 */
            qint64 retailPrice = 0;

            /** 单品批发价格 */
            qint64 wholesalePrice = 0;

            /** 商品图片下载路径 */
            QString imageUrl;

            /** 商品图片Md5 */
            QString imageMd5;

            /** 扩展字段 */
            QJsonObject extension;
        };
        struct Result
        {
            /** 配置商品列表 */
            QList<ConfigWare> wares;
        };

    public:
        TerminalGetConfigwaresResponse(QObject* parent = Q_NULLPTR);
        ~TerminalGetConfigwaresResponse();

        const Result& GetResult() const;
    protected:
        virtual void ParseJsonParam(const QJsonObject& val) override;

    private:
        Result m_result;
    };
}
