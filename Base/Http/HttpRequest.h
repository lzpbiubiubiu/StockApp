// Copyright (c) 2021 Dmall Pos Team

#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QHttpMultiPart>
#include <QJsonObject>
#include <functional>
#include "../Http/HttpConsts.h"

#define ASSIGN_FROM_INT32(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(int));object[name]=var; } while(0)

#define ASSIGN_FROM_INT32_IF_NOT_ZERO(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(int));if(var!=0)object[name]=var; } while(0)

#define ASSIGN_FROM_INT32_IF_GREATER_THAN_ZERO(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(int));if(var>0)object[name]=var; } while(0)

#define ASSIGN_FROM_INT32_IF_LESS_THAN_ZERO(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(int));if(var<0)object[name]=var; } while(0)

#define ASSIGN_FROM_INT64(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(qint64));object[name]=var; } while(0)

#define ASSIGN_FROM_INT64_IF_NOT_ZERO(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(qint64));if(var!=0)object[name]=var; } while(0)

#define ASSIGN_FROM_INT64_IF_GREATER_THAN_ZERO(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(qint64));if(var>0)object[name]=var; } while(0)

#define ASSIGN_FROM_INT64_IF_LESS_THAN_ZERO(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(qint64));if(var<0)object[name]=var; } while(0)

#define ASSIGN_FROM_BOOL(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(bool));object[name]=var; } while(0)

#define ASSIGN_FROM_BOOL_IF_TRUE(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(bool));if(var)object[name]=var; } while(0)

#define ASSIGN_FROM_BOOL_IF_FALSE(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(bool));if(!var)object[name]=var; } while(0)

#define ASSIGN_FROM_STRING(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(QString));object[name]=var; } while(0)

#define ASSIGN_FROM_STRING_IF_NOT_EMPTY(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(QString));if(!var.isEmpty())object[name]=var; } while(0)

#define ASSIGN_FROM_DOUBLE(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(double));object[name]=var; } while(0)

#define ASSIGN_FROM_DOUBLE_IF_GREATER_THAN_ZERO(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(double));if(var>0)object[name]=var; } while(0)

#define ASSIGN_FROM_DOUBLE_IF_LESS_THAN_ZERO(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(double));if(var<0)object[name]=var; } while(0)

#define ASSIGN_FROM_OBJECT(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(QJsonObject));object[name]=var; } while(0)

#define ASSIGN_FROM_ARRAY(object, name, var)\
    do { Q_ASSERT(typeid(var)==typeid(QJsonArray));object[name]=var; } while(0)

namespace Net
{
    class HttpRequest;
    class HttpResponse;

    using CompleteCallback
        = std::function<void(QSharedPointer<HttpRequest> request, QSharedPointer<HttpResponse> response)>;

    using ProgressCallback
        = std::function<void(QSharedPointer<HttpRequest> request, QSharedPointer<HttpResponse> response, qint64 current, qint64 total, bool upload)>;

    /** HTTP请求类 */
    class HttpRequest : public QObject
    {
        Q_OBJECT
    public:
        friend class HttpTask;
        HttpRequest(QObject* parent = nullptr);
        virtual ~HttpRequest();

        /** 设置追踪流水号，与Response相同 */
        void SetTraceNumber(const QString& number);

        /** 设置请求地址 */
        void SetUrl(const QString& url);

        /** 设置请求类型 */
        void SetVerb(HttpVerb vb);

        /** 设置HEADER数据 */
        void SetHeader(const QString& key, const QString& value);

        /** 设置POST请求数据 */
        void SetData(const QByteArray& ba);

        /** 设置POST请求数据（优先）*/
        void SetMultiParts(const QSharedPointer<QHttpMultiPart> parts);

        /** 设置是否允许重定向 */
        void SetRedirectionEnabled(bool enabled);

        /** 设置设否开启SSL */
        void SetSslEnabled(bool enabled);

        /** 设置是否打印请求日志 */
        void SetLogEnabled(bool enabled);

        /** 设置请求日志打印格式 */
        void SetLogFormat(LogFormat format);

        /** 设置超时时间（毫秒），-1表示不超时 */
        void SetTimeout(int timeout);

        /** 设置请求超时重试次数，-1表示不重试 */
        void SetTimeoutRetryCount(int count);

        /** 设置完成回调函数 */
        void SetCompleteCallback(CompleteCallback cb);

        /** 设置进度回调函数*/
        void SetProgressCallback(ProgressCallback cb);

        /** 返回请求URL */
        const QString& GetUrl() const;

        /** 获取请求类型 */
        HttpVerb GetVerb() const;

        /** 获取请求头部 */
        const QMap<QString,QString>& GetHeader() const;

        /** 获取Json格式请求头部 */
        QJsonObject GetHeaderJson() const;

        /** 获取请求数据 */
        const QByteArray& GetData() const;

        /** 获取请求数据 */
        QSharedPointer<QHttpMultiPart> GetMultiparts() const;

        /** 是否允许重定向 */
        bool IsRedirectionEnabled() const;

        /** 获取SSL开启状态 */
        bool IsSslEnabled() const;

        /** 获取是否打印请求日志 */
        bool IsLogEnabled() const;

        /** 获取打印日志格式 */
        LogFormat GetLogFormat() const;

        /** 获取超时时间（毫秒）*/
        int GetTimeout() const;

        /** 获取超时重试次数 */
        int GetTimeoutRetryCount() const;

        /** 获取请求流水号 */
        const QString& GetTraceNumber() const;

        /** 获取完成回调函数 */
        CompleteCallback GetCompleteCallback() const;

        /** 获取进度回调函数 */
        ProgressCallback GetProgressCallback() const;

        /** 序列化POST请求数据 */
        virtual void SerializeForPost(QByteArray& ba);

        /** 序列化离线任务数据 */
        virtual void SerializeForPush(QByteArray& ba);

    private:
        // 请求地址
        QString m_url;

        // 请求方式
        HttpVerb m_verb;

        // POST数据
        QByteArray m_data;

        // POST数据
        QSharedPointer<QHttpMultiPart> m_multiParts;

        // 请求头部
        QMap<QString,QString> m_header;

        // 是否开启SSL
        bool m_sslEnabled = false;

        // 是否打印请求日志
        bool m_logEnabled = true;

        // 打印日志格式
        LogFormat m_logFormat = LogFormat::DETAIL;

        // 是否允许重定向
        bool m_redirectionEnabled = true;

        // 超时自动重试次数
        int m_timeoutRetryCount = 0;

        // 超时时间（毫秒）
        int m_timeout = -1;

        // 追踪串号
        QString m_traceNumber;

        // 完成回调函数（成功+错误）
        CompleteCallback m_completeCallback = nullptr;

        // 进度回调函数（下载+上传）
        ProgressCallback m_progressCallback = nullptr;
    };
    using HttpRequestPtr = QSharedPointer<Net::HttpRequest>;
}
Q_DECLARE_METATYPE(Net::HttpRequestPtr)
