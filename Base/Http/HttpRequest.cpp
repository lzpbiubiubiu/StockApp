#include "HttpRequest.h"

namespace Net
{
    HttpRequest::HttpRequest(QObject* parent)
        : QObject(parent)
    {
    }

    HttpRequest::~HttpRequest()
    {}

    void HttpRequest::SetTraceNumber(const QString& number)
    {
        m_traceNumber = number;
    }

    void HttpRequest::SetUrl(const QString& url)
    {
        m_url = url;
    }

    void HttpRequest::SetVerb(HttpVerb verb)
    {
        m_verb = verb;
    }

    void HttpRequest::SetHeader(const QString& key, const QString& value)
    {
        m_header[key] = value;
    }

    void HttpRequest::SetData(const QByteArray& ba)
    {
        m_data = ba;
    }

    void HttpRequest::SetMultiParts(const QSharedPointer<QHttpMultiPart> parts)
    {
        m_multiParts = parts;
    }

    void HttpRequest::SetRedirectionEnabled(bool enabled)
    {
        m_redirectionEnabled = enabled;
    }

    void HttpRequest::SetSslEnabled(bool enabled)
    {
        m_sslEnabled = enabled;
    }

    void HttpRequest::SetLogEnabled(bool enabled)
    {
        m_logEnabled = enabled;
    }

    void HttpRequest::SetLogFormat(LogFormat format)
    {
        m_logFormat = format;
    }

    void HttpRequest::SetTimeout(int timeout)
    {
        m_timeout = timeout;
    }

    void HttpRequest::SetTimeoutRetryCount(int count)
    {
        m_timeoutRetryCount = count;
    }

    void HttpRequest::SetCompleteCallback(CompleteCallback callback)
    {
        m_completeCallback = callback;
    }

    void HttpRequest::SetProgressCallback(ProgressCallback callback)
    {
        m_progressCallback = callback;
    }

    const QString& HttpRequest::GetUrl() const
    {
        return m_url;
    }

    HttpVerb HttpRequest::GetVerb() const
    {
        return m_verb;
    }

    const QMap<QString, QString>& HttpRequest::GetHeader() const
    {
        return m_header;
    }

    QJsonObject HttpRequest::GetHeaderJson() const
    {
        QJsonObject json;
        for(auto& key : m_header.keys())
            json.insert(key, m_header.value(key));
        return json;
    }

    const QByteArray& HttpRequest::GetData() const
    {
        return m_data;
    }

    QSharedPointer<QHttpMultiPart> HttpRequest::GetMultiparts() const
    {
        return m_multiParts;
    }

    bool HttpRequest::IsRedirectionEnabled() const
    {
        return m_redirectionEnabled;
    }

    bool HttpRequest::IsSslEnabled() const
    {
        return m_sslEnabled;
    }

    bool HttpRequest::IsLogEnabled() const
    {
        return m_logEnabled;
    }

    LogFormat HttpRequest::GetLogFormat() const
    {
        return m_logFormat;
    }

    int HttpRequest::GetTimeout() const
    {
        return m_timeout;
    }

    int HttpRequest::GetTimeoutRetryCount() const
    {
        return m_timeoutRetryCount;
    }

    const QString& HttpRequest::GetTraceNumber() const
    {
        return m_traceNumber;
    }

    CompleteCallback HttpRequest::GetCompleteCallback() const
    {
        return m_completeCallback;
    }

    ProgressCallback HttpRequest::GetProgressCallback() const
    {
        return m_progressCallback;
    }

    void HttpRequest::SerializeForPost(QByteArray& ba)
    {}

    void HttpRequest::SerializeForPush(QByteArray& ba)
    {}
}
