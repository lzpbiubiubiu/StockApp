#pragma once

#include <QObject>
#include <QMap>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "Base/Http/HttpResponse.h"

namespace Net
{
    /** 交易网关HTTP响应基类 */
    class StockAppResponse : public HttpResponse
    {
        Q_OBJECT
    public:
        StockAppResponse(QObject* parent = nullptr);

        virtual bool IsOk() const override;

        virtual const QString& GetError() const override;

        virtual const QString& GetDisplayErrorCode() const override;

        virtual const QString& GetErrorMessage() const override;

        virtual const QString& GetActionCode() const override;
        
    protected:
        /** 解析对象型JSON参数 */
        virtual void ParseJsonParam(const QJsonObject& val);

        /** 解析数组型JSON参数 */
        virtual void ParseJsonParam(const QJsonArray& val);

        /** 解析布尔型Bool参数 */
        virtual void ParseJsonParam(bool val);

        /** 反序列化响应数据 */
        virtual void Deserialize(const QByteArray& ba) override;

    protected:
        // 错误码
        QString m_resultCode;

        // 错误描述
        QString m_resultMessage;

        // 界面要显示的错误码
        QString m_resultDisplayCode;

    };
}
