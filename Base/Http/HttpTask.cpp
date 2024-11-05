#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMetaEnum>
#include <QThread>
#include <QSslConfiguration>
#include <QJsonDocument>
#include "HttpConsts.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpTask.h"
#include "../Log/Log.h"
#include "../Util/MathUtil.h"

// 任务超时轮询时间间隔(ms)
const int TIMEOUT_CHECK_INTERVAL = 100;

// 网络错误码定义
static const QHash<int, const char*> NETWORK_ERROR_CODE_MAP = {
    { QNetworkReply::NoError, ERR_NETWORK_SUCCESS },
    { QNetworkReply::ConnectionRefusedError, ERR_NETWORK_CONNECTION_REFUSED },
    { QNetworkReply::RemoteHostClosedError, ERR_NETWORK_REMOTE_HOST_CLOSED },
    { QNetworkReply::HostNotFoundError, ERR_NETWORK_HOST_NOT_FOUND },
    { QNetworkReply::TimeoutError, ERR_NETWORK_TIMEOUT },
    { QNetworkReply::OperationCanceledError, ERR_NETWORK_OPERATION_CANCELED },
    { QNetworkReply::SslHandshakeFailedError, ERR_NETWORK_SSL_HANDSHAKE_FAILED },
    { QNetworkReply::TemporaryNetworkFailureError, ERR_NETWORK_TEMPORARY_NETWORK_FAILURE },
    { QNetworkReply::NetworkSessionFailedError, ERR_NETWORK_SESSION_FAILED },
    { QNetworkReply::BackgroundRequestNotAllowedError, ERR_NETWORK_BACKGROUND_REQUEST_NOT_ALLOWED },
    { QNetworkReply::TooManyRedirectsError, ERR_NETWORK_TOO_MANY_REDIRECTS },
    { QNetworkReply::InsecureRedirectError, ERR_NETWORK_INSECURED_REDIRECT },
    { QNetworkReply::UnknownNetworkError, ERR_NETWORK_UNKNOWN_ERROR },
    { QNetworkReply::ProxyConnectionRefusedError, ERR_NETWORK_PROXY_CONNECTION_REFUSED },
    { QNetworkReply::ProxyConnectionClosedError, ERR_NETWORK_PROXY_CONNECTION_CLOSED },
    { QNetworkReply::ProxyNotFoundError, ERR_NETWORK_PROXY_NOT_FOUND },
    { QNetworkReply::ProxyTimeoutError, ERR_NETWORK_PROXY_TIMEOUT },
    { QNetworkReply::ProxyAuthenticationRequiredError, ERR_NETWORK_PROXY_AUTHENTICATION_REQUIRED },
    { QNetworkReply::UnknownProxyError, ERR_NETWORK_UNKNOWN_PROXY },
    { QNetworkReply::ContentAccessDenied, ERR_NETWORK_CONTENT_ACCESS_DENIED },
    { QNetworkReply::ContentOperationNotPermittedError, ERR_NETWORK_OPERATION_NOT_PERMITTED },
    { QNetworkReply::ContentNotFoundError, ERR_NETWORK_CONTENT_NOT_FOUND },
    { QNetworkReply::AuthenticationRequiredError, ERR_NETWORK_AUTHENTICATION_REQUIRED },
    { QNetworkReply::ContentReSendError, ERR_NETWORK_CONTENT_RESEND },
    { QNetworkReply::ContentConflictError, ERR_NETWORK_CONTENT_CONFLICT },
    { QNetworkReply::ContentGoneError, ERR_NETWORK_CONTENT_GONE },
    { QNetworkReply::UnknownContentError, ERR_NETWORK_UNKNOWN_CONTENT },
    { QNetworkReply::ProtocolUnknownError, ERR_NETWORK_PROTOCOL_UNKNOWN },
    { QNetworkReply::ProtocolInvalidOperationError, ERR_NETWORK_PROTOCOL_INVALID_OPERATION },
    { QNetworkReply::ProtocolFailure, ERR_NETWORK_PROTOCOL_FAILURE },
    { QNetworkReply::InternalServerError, ERR_NETWORK_INTERNAL_SERVER },
    { QNetworkReply::OperationNotImplementedError, ERR_NETWORK_OPERATION_NOT_IMPLEMENTED },
    { QNetworkReply::ServiceUnavailableError, ERR_NETWORK_SERVICE_UNAVAILABLE },
    { QNetworkReply::UnknownServerError, ERR_NETWORK_UNKNOWN_SERVER } };

// 网络错误描述定义(翻译需要数据源)
static const QHash<int, QString> NETWORK_ERROR_MESSAGE_MAP = {
    { QNetworkReply::NoError, "Request succeeded"},
    { QNetworkReply::ConnectionRefusedError, "The remote server refused the connection (the server is not accepting requests)"},
    { QNetworkReply::RemoteHostClosedError, "The remote server closed the connection prematurely, before the entire reply was received and processed"},
    { QNetworkReply::HostNotFoundError, "The remote host name was not found (invalid hostname)"},
    { QNetworkReply::TimeoutError, "Network times out. Please contact IT Help or try again."},
    { QNetworkReply::OperationCanceledError, "The operation was canceled via calls to abort() or close() before it was finished."},
    { QNetworkReply::SslHandshakeFailedError, "The SSL/TLS handshake failed and the encrypted channel could not be established. The sslErrors() signal should have been emitted."},
    { QNetworkReply::TemporaryNetworkFailureError, "The connection was broken due to disconnection from the network, however the system has initiated roaming to another access point. The request should be resubmitted and will be processed as soon as the connection is re-established."},
    { QNetworkReply::NetworkSessionFailedError, "The connection was broken due to disconnection from the network or failure to start the network."},
    { QNetworkReply::BackgroundRequestNotAllowedError, "The background request is not currently allowed due to platform policy."},
    { QNetworkReply::TooManyRedirectsError, "While following redirects, the maximum limit was reached. The limit is by default set to 50 or as set by QNetworkRequest::setMaxRedirectsAllowed(). (This value was introduced in 5.6.)"},
    { QNetworkReply::InsecureRedirectError, "While following redirects, the network access API detected a redirect from a encrypted protocol (https) to an unencrypted one (http). (This value was introduced in 5.6.)"},
    { QNetworkReply::UnknownNetworkError, "Unknown network error"},
    { QNetworkReply::ProxyConnectionRefusedError, "The connection to proxy server was refused (proxy server does not accept requests)."},
    { QNetworkReply::ProxyConnectionClosedError, "The proxy server closed the connection prematurely, before the entire reply was received and processed."},
    { QNetworkReply::ProxyNotFoundError, "The proxy host name was not found (invalid proxy hostname)."},
    { QNetworkReply::ProxyTimeoutError, "The connection to the proxy timed out or the proxy did not reply in time to the request sent."},
    { QNetworkReply::ProxyAuthenticationRequiredError, "The proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)."},
    { QNetworkReply::UnknownProxyError, "An unknown proxy-related error was detected."},
    { QNetworkReply::ContentAccessDenied, "The access to the remote content was denied (similar to HTTP error 403)"},
    { QNetworkReply::ContentOperationNotPermittedError, "The operation requested on the remote content is not permitted."},
    { QNetworkReply::ContentNotFoundError, "The content was not found at the server (similar to HTTP error 404)."},
    { QNetworkReply::AuthenticationRequiredError, "The remote server requires authentication to serve the content but the credentials provided were not accepted (if any)."},
    { QNetworkReply::ContentReSendError, "The request needed to be sent again, but this failed for example because the upload data could not be read a second time."},
    { QNetworkReply::ContentConflictError, "The request could not be completed due to a conflict with the current state of the resource."},
    { QNetworkReply::ContentGoneError, "The requested resource is no longer available at the server."},
    { QNetworkReply::UnknownContentError, "Unknown error detected related to remote content."},
    { QNetworkReply::ProtocolUnknownError, "The Network Access API cannot honor the request because the protocol is not known."},
    { QNetworkReply::ProtocolInvalidOperationError, "The requested operation is invalid for this protocol."},
    { QNetworkReply::ProtocolFailure, "A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)."},
    { QNetworkReply::InternalServerError, "The server encountered an unexpected condition which prevented it from fulfilling the request."},
    { QNetworkReply::OperationNotImplementedError, "The server does not support the functionality required to fulfill the request."},
    { QNetworkReply::ServiceUnavailableError, "The server is unable to handle the request at this time."},
    { QNetworkReply::UnknownServerError, "An unknown error related to the server response was detected."}
};

namespace Net
{
    HttpTask::HttpTask(QSharedPointer<HttpRequest> request, QSharedPointer<HttpResponse> response, QObject* parent)
        : QObject(parent)
        , m_request(request)
        , m_response(response)
    {
        Q_ASSERT(request != nullptr);
        Q_ASSERT(response != nullptr);
        //m_request->SetTraceNumber(Base::MathUtil::GetUUID());
        m_response->SetTraceNumber(m_request->GetTraceNumber());
    }

    HttpTask::~HttpTask()
    {

    }

    bool HttpTask::IsRunning() const
    {
        return m_isRunning;
    }

    void HttpTask::Run()
    {
        // 请求重试复用同一个HttpResponse，所以每次请求前需要清空数据
        m_response->SetError("");
        m_response->SetErrorMessage("");
        m_response->SetStatusCode(0);
        m_response->ClearData();

        QNetworkRequest req;
        req.setUrl(m_request->GetUrl());

        if (m_request->IsSslEnabled())
            EnabledSSL(req);

        // 当网络适配器从禁用状态恢复时，QNetworkAccessManager组件可能不可用，需要手动启用
        if (m_networkAccessManager->networkAccessible() != QNetworkAccessManager::Accessible)
            m_networkAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);

        switch (m_request->GetVerb())
        {
        case HttpVerb::GET:
        {
            m_networkReply = m_networkAccessManager->get(req);

            // 打印请求日志
            if (m_request->IsLogEnabled())
            {
                LOG_INFO(QString("HTTP-GET [%1] %2").arg(m_request->GetTraceNumber()).arg(m_request->GetUrl()));
            }
        }
        break;
        case HttpVerb::POST:
        {
            // 优先以MultiPart类型数据作为POST内容
            if (!m_request->GetMultiparts().isNull())
            {
                SetRawHeader(req);
                m_networkReply = m_networkAccessManager->post(req, m_request->GetMultiparts().data());
            }
            else
            {
                QByteArray ba;
                m_request->SerializeForPost(ba);
                m_request->SetData(ba);
                SetRawHeader(req);

                m_networkReply = m_networkAccessManager->post(req, ba);

                // 打印请求日志
                if (m_request->IsLogEnabled())
                {
                    QByteArray contentType = req.rawHeader("Content-Type");
                    if (contentType == "application/x-www-form-urlencoded")
                    {
                        if (m_request->GetLogFormat() == LogFormat::DETAIL)
                        {
                            // 详细信息
                            QByteArray param = QByteArray::fromPercentEncoding(m_request->GetData());

                            QString header = QJsonDocument(m_request->GetHeaderJson()).toJson(QJsonDocument::Compact);
                            LOG_INFO(QString("HTTP-POST [%1] Content: %2?%3 Header: %4").arg(m_request->GetTraceNumber()).arg(m_request->GetUrl()).arg(QString(param)).arg(header));
                        }
                        else
                        {
                            // 概要信息
                            LOG_INFO(QString("HTTP-POST [%1] %2 Size: %3 KB").arg(m_request->GetTraceNumber()).arg(m_request->GetUrl()).arg(QString::number(m_request->GetData().size() / 1024.0, 'f', 2)));
                        }
                    }
                    else if (contentType == "application/json")
                    {
                        QString header = QJsonDocument(m_request->GetHeaderJson()).toJson(QJsonDocument::Compact);
                        LOG_INFO(QString("HTTP-POST [%1] URL: %2 Header: %3 Content: %4").arg(m_request->GetTraceNumber()).arg(m_request->GetUrl()).arg(header).arg(QString(m_request->GetData())));
                    }
                    else
                    {
                        Q_ASSERT(0);
                    }
                }
            }
        }
        break;
        default:
            Q_ASSERT(0);
        }

        m_timestamp = QDateTime::currentMSecsSinceEpoch();
        m_isRunning = true;
        m_neverTimeout = false;

        connect(m_networkReply, &QNetworkReply::readyRead, this, &HttpTask::onReplyReadReady);
        connect(m_networkReply, &QNetworkReply::finished, this, &HttpTask::onReplyFinished);
        connect(m_networkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &HttpTask::onReplyError);
        connect(m_networkReply, &QNetworkReply::sslErrors, this, &HttpTask::onReplySslErrors);
        connect(m_networkReply, &QNetworkReply::downloadProgress, this, &HttpTask::onReplyDownloadProgress);
        connect(m_networkReply, &QNetworkReply::uploadProgress, this, &HttpTask::onReplyUploadProgress);

        m_timer = new QTimer(m_networkReply);
        connect(m_timer, &QTimer::timeout, this, &HttpTask::onTimeout, Qt::UniqueConnection);
        m_timer->start(TIMEOUT_CHECK_INTERVAL);
    }

    bool HttpTask::IsExpired(qint64 timestamp) const
    {
        Q_ASSERT(m_request != nullptr);
        Q_ASSERT(m_timestamp > 0);
        if (m_neverTimeout || m_request->GetTimeout() <= 0)
            return false;

        return (timestamp - m_timestamp >= m_request->GetTimeout());
    }

    void HttpTask::SetRawHeader(QNetworkRequest& request)
    {
        for (const auto& k : m_request->GetHeader().keys())
            request.setRawHeader(k.toUtf8(), m_request->GetHeader()[k].toUtf8());
    }

    void HttpTask::SetNetworkAccessManager(QNetworkAccessManager* instance)
    {
        m_networkAccessManager = instance;
    }

    void HttpTask::EnabledSSL(QNetworkRequest& request)
    {
        QSslConfiguration ssl;
        ssl.setPeerVerifyMode(QSslSocket::VerifyNone);
        ssl.setProtocol(QSsl::TlsV1SslV3);
        request.setSslConfiguration(ssl);
    }

    void HttpTask::onReplyReadReady()
    {
        m_response->AppendData(m_networkReply->readAll());
    }

    void HttpTask::onReplyFinished()
    {
        Q_ASSERT(m_networkReply != nullptr);

        // 用户abort()
        QNetworkReply::NetworkError code = m_networkReply->error();
        if (code != QNetworkReply::NetworkError::NoError)
        {
            onReplyError(code);
            return;
        }

        m_timer->disconnect();
        m_timer->stop();
        m_elapsed += (QDateTime::currentMSecsSinceEpoch() - m_timestamp);

        // 302重定向
        const QVariant redirection = m_networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
        if (m_request->IsRedirectionEnabled() && !redirection.isNull())
        {
            m_networkReply->disconnect();
            m_networkReply->deleteLater();
            m_request->SetUrl(redirection.toUrl().toString());
            Run();
            return;
        }

        m_neverTimeout = true;
        m_isRunning = false;

        QString error = NETWORK_ERROR_CODE_MAP[QNetworkReply::NoError];
        QString message = NETWORK_ERROR_MESSAGE_MAP[QNetworkReply::NoError];

        m_response->SetError(error);
        m_response->SetErrorMessage(message);
        m_response->SetElapsedTime(m_elapsed);
        m_response->SetStatusCode(m_networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>());
        m_response->Deserialize(m_response->GetData());
        

        if (m_request->GetCompleteCallback() != nullptr)
            m_request->GetCompleteCallback()(m_request, m_response);

        m_networkReply->disconnect();
        m_networkReply->close();
        m_networkReply->deleteLater();

        // 打印日志
        if (m_response->IsLogEnabled())
        {
            QString verb = HttpVerb::GET ? "HTTP-GET" : "HTTP-POST";
            if (m_response->GetLogFormat() == LogFormat::DETAIL)
            {
                LOG_INFO(QString("%1 [%2] Time:%3 ms Status:%4 Size:%5 KB Content: %6").arg(verb)
                    .arg(m_response->GetTraceNumber())
                    .arg(m_response->GetElapsedTime())
                    .arg(m_response->GetStatusCode())
                    .arg(QString::number(m_response->GetData().size() / 1000.0, 'f', 2))
                    .arg(QString(m_response->GetData()))
                    );
            }
            else
            {
                LOG_INFO(QString("%1 [%2] Time:%3 ms Status:%4 Size:%5 KB").arg(verb)
                         .arg(m_response->GetTraceNumber())
                         .arg(m_response->GetElapsedTime())
                         .arg(m_response->GetStatusCode())
                         .arg(QString::number(m_response->GetData().size() / 1000.0, 'f', 2)));
            }
        }

        auto result = QSharedPointer<Net::HttpResult>::create();
        result->trace = m_request->GetTraceNumber();
        result->url = m_request->GetUrl();
        result->method = (m_request->GetVerb() == HttpVerb::GET) ? "GET" : "POST";
        result->ok = m_response->IsOk();
        result->status = m_response->GetStatusCode();
        result->error = m_response->GetError();;
        result->message = m_response->GetErrorMessage();
        result->elapsed = m_elapsed;
        result->sent = m_request->GetData().size();
        result->recv = m_response->GetData().size();
        emit signalTaskFinished(result);
    }

    void HttpTask::onReplyError(QNetworkReply::NetworkError code)
    {
        Q_ASSERT(m_networkReply != nullptr);

        m_timer->disconnect();
        m_timer->stop();
        m_elapsed += (QDateTime::currentMSecsSinceEpoch() - m_timestamp);

        // abort()的错误码替换为TimeoutError
        if (code == QNetworkReply::NetworkError::OperationCanceledError)
            code = QNetworkReply::NetworkError::TimeoutError;

        // 超时重试
        int count = m_request->GetTimeoutRetryCount();
        if (count > 0 && code == QNetworkReply::NetworkError::TimeoutError)
        {
            m_request->SetTimeoutRetryCount(--count);
            m_networkReply->disconnect();
            m_networkReply->deleteLater();
            Run();
            LOG_ERROR(QString("http request timeout, retry: %1").arg(count));
            return;
        }

        m_isRunning = false;
        QString error = NETWORK_ERROR_CODE_MAP[code];
        const QString message = NETWORK_ERROR_MESSAGE_MAP[code];

        m_response->SetError(error);
        m_response->SetErrorMessage(message);
        m_response->SetElapsedTime(m_elapsed);

        if (m_request->GetCompleteCallback() != nullptr)
            m_request->GetCompleteCallback()(m_request, m_response);

        // 打印日志
        if (m_response->IsLogEnabled())
        {
            LOG_ERROR(QString("[%1] %2 http code:%3").arg(m_response->GetTraceNumber()).arg(NETWORK_ERROR_MESSAGE_MAP[code]).arg(m_networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>()));
        }

        m_networkReply->disconnect();
        m_networkReply->deleteLater();

        auto result = QSharedPointer<Net::HttpResult>::create();
        result->trace = m_request->GetTraceNumber();
        result->url = m_request->GetUrl();
        result->method = (m_request->GetVerb() == HttpVerb::GET) ? "GET" : "POST";
        result->ok = m_response->IsOk();
        result->status = m_response->GetStatusCode();
        result->error = m_response->GetError();
        result->message = m_response->GetErrorMessage();
        result->elapsed = m_elapsed;
        result->sent = m_request->GetData().size();
        result->recv = m_response->GetData().size();
        emit signalTaskFinished(result);
    }

    void HttpTask::onReplySslErrors(const QList<QSslError>& errors)
    {
        m_timer->disconnect();
        m_timer->stop();
        for (const auto& i : errors)
        {
            // 打印日志
            if (m_response->IsLogEnabled())
            {
                LOG_ERROR(i.errorString());
            }
        }
    }

    void HttpTask::onReplyDownloadProgress(qint64 recv, qint64 total)
    {
        Q_ASSERT(m_networkReply != nullptr);
        m_neverTimeout = true;

        if (m_request->GetProgressCallback() != nullptr && total > 0)
            m_request->GetProgressCallback()(m_request, m_response, recv, total, false);
    }

    void HttpTask::onReplyUploadProgress(qint64 sent, qint64 total)
    {
        Q_ASSERT(m_networkReply != nullptr);
        if (m_request->GetProgressCallback() != nullptr && total > 0)
            m_request->GetProgressCallback()(m_request, m_response, sent, total, true);
    }

    void HttpTask::onTimeout()
    {
        if (IsExpired(QDateTime::currentMSecsSinceEpoch()))
        {
            m_timer->disconnect();
            m_timer->stop();
            m_networkReply->abort();
        }
    }
}
