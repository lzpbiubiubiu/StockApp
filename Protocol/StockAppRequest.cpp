#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QDateTime>
#include "Base/Util/MathUtil.h"
#include "StockAppRequest.h"

namespace Net
{
    StockAppRequest::StockAppRequest(QObject *parent)
        : HttpRequest(parent)
    {
        SetVerb(HttpVerb::POST);
        SetSslEnabled(true);
        SetTimeout(5000);
        SetDeviceType("Stock App");
        SetAppVersion("1.0.0.0");
        SetTraceNumber(Base::MathUtil::GetUUID());
    }

    StockAppRequest::~StockAppRequest()
    {}

    void StockAppRequest::MakeJsonParam(QJsonObject& val)
    {}

    void StockAppRequest::SerializeForPost(QByteArray& ba)
    {
        QJsonObject val;
        MakeJsonParam(val);

        QJsonDocument doc;
        doc.setObject(val);
        QByteArray jsonstr = doc.toJson(QJsonDocument::Compact).toPercentEncoding();
        ba.append("param=").append(jsonstr);

        // header参数
        SetHeader("Content-Type", "application/x-www-form-urlencoded");
        SetHeader("traceId", GetTraceNumber());
        SetHeader("deviceType", m_deviceType);
        SetHeader("version", m_appVersion);
    }

    void StockAppRequest::SerializeForPush(QByteArray& ba)
    {
        QJsonObject val;
        MakeJsonParam(val);

        QJsonDocument doc;
        doc.setObject(val);
        ba = doc.toJson(QJsonDocument::Compact);
    }

    void StockAppRequest::SetDeviceType(const QString& deviceType)
    {
        m_deviceType = deviceType;
    }

    void StockAppRequest::SetAppVersion(const QString& version)
    {
        m_appVersion = version;
    }


    const QString& StockAppRequest::GetDeviceType() const
    {
        return m_deviceType;
    }


    const QString& StockAppRequest::GetAppVersion() const
    {
        return m_appVersion;
    }
}
