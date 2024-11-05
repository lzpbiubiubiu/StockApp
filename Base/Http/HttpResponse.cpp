#include <QDataStream>
#include "HttpConsts.h"
#include "HttpResponse.h"

namespace Net
{
    HttpResponse::HttpResponse(QObject* parent)
        : QObject(parent)
    {
    }

    HttpResponse::~HttpResponse()
    {
    }

    void HttpResponse::SetLogEnabled(bool enabled)
    {
        m_logEnabled = enabled;
    }

    void HttpResponse::SetLogFormat(LogFormat format)
    {
        m_logFormat = format;
    }

    bool HttpResponse::IsLogEnabled() const
    {
        return m_logEnabled;
    }

    LogFormat HttpResponse::GetLogFormat() const
    {
        return m_logFormat;
    }

    bool HttpResponse::IsOk() const
    {
        return m_error == ERR_NETWORK_SUCCESS;
    }

    const QString& HttpResponse::GetError() const
    {
        return m_error;
    }

    const QString& HttpResponse::GetDisplayErrorCode() const
    {
        return m_displayErrorCode;
    }

    const QString& HttpResponse::GetErrorMessage() const
    {
        return m_errorMessage;
    }

    const QString& HttpResponse::GetActionCode() const
    {
        return m_actionCode;
    }

    void HttpResponse::SetTraceNumber(const QString& number)
    {
        m_traceNumber = number;
    }

    void HttpResponse::SetElapsedTime(int time_ms)
    {
        m_elapsed = time_ms;
    }

    void HttpResponse::SetStatusCode(int code)
    {
        m_statusCode = code;
    }

    void HttpResponse::SetError(const QString& code)
    {
        m_error = code;
    }

    void HttpResponse::SetErrorMessage(const QString& msg)
    {
        m_errorMessage = msg;
    }

    void HttpResponse::AppendData(const QByteArray& ba)
    {
        m_data.append(ba);
    }

    const QString& HttpResponse::GetTraceNumber() const
    {
        return m_traceNumber;
    }

    int HttpResponse::GetElapsedTime() const
    {
        return m_elapsed;
    }

    int HttpResponse::GetStatusCode() const
    {
        return m_statusCode;
    }

    const QByteArray& HttpResponse::GetData() const
    {
        return m_data;
    }

    void HttpResponse::ClearData()
    {
        m_data.clear();
    }

    void HttpResponse::Deserialize(const QByteArray& ba) { }
}
