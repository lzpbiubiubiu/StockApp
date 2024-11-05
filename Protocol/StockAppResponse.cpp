#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include "StockAppResponse.h"
#include "Base/Util/JsonUtil.h"
#include "Base/Log/Log.h"

namespace Net
{
    StockAppResponse::StockAppResponse(QObject* parent)
        : HttpResponse(parent)
    {}

    bool StockAppResponse::IsOk() const
    {
        return HttpResponse::IsOk() && (GetStatusCode() == 200) && (m_resultCode == "0000");
    }

    void StockAppResponse::ParseJsonParam(const QJsonObject& val)
    {}

    void StockAppResponse::ParseJsonParam(const QJsonArray& val)
    {}

    void StockAppResponse::ParseJsonParam(bool val)
    {}

    const QString& StockAppResponse::GetError() const
    {
        // 网络错误返回网络错误码，否则返回网关业务错误码
        if(!HttpResponse::IsOk())
            return m_error;
        else
            return m_resultCode;
    }

    const QString& StockAppResponse::GetDisplayErrorCode() const
    {
        // 网络错误返回网络错误码，否则返回网关业务错误码
        if(!HttpResponse::IsOk())
            return m_error;
        else
            return m_resultDisplayCode;
    }

    const QString& StockAppResponse::GetErrorMessage() const
    {
        // 网络错误返回网络错误描述，否则返回网关业务错误描述
        if(!HttpResponse::IsOk())
            return m_errorMessage;
        else
            return m_resultMessage;
    }


    const QString& StockAppResponse::GetActionCode() const
    {
        return m_actionCode;
    }

    void StockAppResponse::Deserialize(const QByteArray& ba)
    {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
        if(error.error != QJsonParseError::NoError)
            return;

        QJsonObject root = doc.object();
        m_resultCode = root["code"].toString();
        m_resultMessage = root["displayMsg"].toString();
        m_resultDisplayCode = root["displayCode"].toString();

        if(m_resultCode == "0000" && root["success"].toBool())
        {
            if(root["data"].isArray())
                ParseJsonParam(root["data"].toArray());
            else if(root["data"].isObject())
                ParseJsonParam(root["data"].toObject());
            else if(root["data"].isBool())
                ParseJsonParam(root["data"].toBool());
            else
                ;//LOG_ERROR(QString("Response Error:%1").arg(QString(ba)));
        }
    }
}
