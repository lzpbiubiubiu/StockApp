#pragma once

#include <QObject>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "Base/Http/HttpRequest.h"

namespace Net
{
    /** Stock App HTTP请求基类 */
    class StockAppRequest : public HttpRequest
    {
        Q_OBJECT
    public:
        StockAppRequest(QObject* parent = nullptr);
        ~StockAppRequest();

        void SetDeviceType(const QString& deviceType);

        void SetAppVersion(const QString& version);

        const QString& GetDeviceType() const;

        const QString& GetAppVersion() const;

        virtual void SerializeForPost(QByteArray& ba) override;

        virtual void SerializeForPush(QByteArray& ba) override;

    protected:
        virtual void MakeJsonParam(QJsonObject& val);

    protected:

        // 设备类型 Stock App
        QString m_deviceType;

        // 客户端版本号 (放请求头)
        QString m_appVersion;
    };
}
