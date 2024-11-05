#pragma once

#include <QString>
#include <QSharedPointer>

/** HTTP 网络错误码 */
#define ERR_NETWORK_SUCCESS                             "ERR_NETWORK_SUCCESS"
#define ERR_NETWORK_CONNECTION_REFUSED                  "ERR_NETWORK_CONNECTION_REFUSED"
#define ERR_NETWORK_REMOTE_HOST_CLOSED                  "ERR_NETWORK_REMOTE_HOST_CLOSED"
#define ERR_NETWORK_HOST_NOT_FOUND                      "ERR_NETWORK_HOST_NOT_FOUND"
#define ERR_NETWORK_TIMEOUT                             "ERR_NETWORK_TIMEOUT"
#define ERR_NETWORK_OPERATION_CANCELED                  "ERR_NETWORK_OPERATION_CANCELED"
#define ERR_NETWORK_SSL_HANDSHAKE_FAILED                "ERR_NETWORK_SSL_HANDSHAKE_FAILED"
#define ERR_NETWORK_TEMPORARY_NETWORK_FAILURE           "ERR_NETWORK_TEMPORARY_NETWORK_FAILURE"
#define ERR_NETWORK_SESSION_FAILED                      "ERR_NETWORK_SESSION_FAILED"
#define ERR_NETWORK_BACKGROUND_REQUEST_NOT_ALLOWED      "ERR_NETWORK_BACKGROUND_REQUEST_NOT_ALLOWED"
#define ERR_NETWORK_TOO_MANY_REDIRECTS                  "ERR_NETWORK_TOO_MANY_REDIRECTS"
#define ERR_NETWORK_INSECURED_REDIRECT                  "ERR_NETWORK_INSECURED_REDIRECT"
#define ERR_NETWORK_UNKNOWN_ERROR                       "ERR_NETWORK_UNKNOWN_ERROR"
#define ERR_NETWORK_PROXY_CONNECTION_REFUSED            "ERR_NETWORK_PROXY_CONNECTION_REFUSED"
#define ERR_NETWORK_PROXY_CONNECTION_CLOSED             "ERR_NETWORK_PROXY_CONNECTION_CLOSED"
#define ERR_NETWORK_PROXY_NOT_FOUND                     "ERR_NETWORK_PROXY_NOT_FOUND"
#define ERR_NETWORK_PROXY_TIMEOUT                       "ERR_NETWORK_PROXY_TIMEOUT"
#define ERR_NETWORK_PROXY_AUTHENTICATION_REQUIRED       "ERR_NETWORK_PROXY_AUTHENTICATION_REQUIRED"
#define ERR_NETWORK_UNKNOWN_PROXY                       "ERR_NETWORK_UNKNOWN_PROXY"
#define ERR_NETWORK_CONTENT_ACCESS_DENIED               "ERR_NETWORK_CONTENT_ACCESS_DENIED"
#define ERR_NETWORK_OPERATION_NOT_PERMITTED             "ERR_NETWORK_OPERATION_NOT_PERMITTED"
#define ERR_NETWORK_CONTENT_NOT_FOUND                   "ERR_NETWORK_CONTENT_NOT_FOUND"
#define ERR_NETWORK_AUTHENTICATION_REQUIRED             "ERR_NETWORK_AUTHENTICATION_REQUIRED"
#define ERR_NETWORK_CONTENT_RESEND                      "ERR_NETWORK_CONTENT_RESEND"
#define ERR_NETWORK_CONTENT_CONFLICT                    "ERR_NETWORK_CONTENT_CONFLICT"
#define ERR_NETWORK_CONTENT_GONE                        "ERR_NETWORK_CONTENT_GONE"
#define ERR_NETWORK_UNKNOWN_CONTENT                     "ERR_NETWORK_UNKNOWN_CONTENT"
#define ERR_NETWORK_PROTOCOL_UNKNOWN                    "ERR_NETWORK_PROTOCOL_UNKNOWN"
#define ERR_NETWORK_PROTOCOL_INVALID_OPERATION          "ERR_NETWORK_PROTOCOL_INVALID_OPERATION"
#define ERR_NETWORK_PROTOCOL_FAILURE                    "ERR_NETWORK_PROTOCOL_FAILURE"
#define ERR_NETWORK_INTERNAL_SERVER                     "ERR_NETWORK_INTERNAL_SERVER"
#define ERR_NETWORK_OPERATION_NOT_IMPLEMENTED           "ERR_NETWORK_OPERATION_NOT_IMPLEMENTED"
#define ERR_NETWORK_SERVICE_UNAVAILABLE                 "ERR_NETWORK_SERVICE_UNAVAILABLE"
#define ERR_NETWORK_UNKNOWN_SERVER                      "ERR_NETWORK_UNKNOWN_SERVER"

/** HTTP 用户扩展错误码 */
#define ERR_NETWORK_DISCONNECTED                        "ERR_NETWORK_DISCONNECTED"

namespace Net
{
    /** HTTP请求模式 */
    enum HttpVerb
    {
        GET,
        POST,
    };

    /** HTTP日志格式 */
    enum LogFormat
    {
        /** 详细信息（默认） */
        DETAIL = 0,

        /** 数据大小（KB） */
        SIZE,
    };

    /** HTTP请求结果 */
    class HttpResult
    {
    public:
        /** UUID */
        QString trace;

        /** URL地址 */
        QString url;

        /** 请求方法 */
        QString method;

        /** 请求结果 */
        bool ok = false;

        /** 状态码 */
        int status = 0;

        /** 错误码 */
        QString error;

        /** 错误描述 */
        QString message;

        /** 耗时（毫秒) */
        int elapsed = 0;

        /** 已发送字节数 */
        qint64 sent = 0;

        /** 已接收字节数*/
        qint64 recv = 0;

        QString ToString() const
        {
            QString str =
                "\n==========> HTTP RESULT\n"
                "|UUID: "    + trace                                    + "\n"
                "|URL: "     + url                                      + "\n"
                "|Method: "  + method                                   + "\n"
                "|Status: "  + QString::number(status)                  + "\n"
                "|Error: "   + error                                    + "\n"
                "|Message: " + message                                  + "\n"
                "|Time: "    + QString::number(elapsed)                 + " ms \n"
                "|Sent: "    + QString::number(sent/1000.0, 'f', 2)     + " KB \n"
                "|Recv: "    + QString::number(recv / 1000.0, 'f', 2)   + " KB \n";
            return str;
        }
    };
    using HttpResultPtr = QSharedPointer<Net::HttpResult>;
}

Q_DECLARE_METATYPE(Net::HttpResultPtr)
