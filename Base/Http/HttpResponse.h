// Copyright (c) 2021 Dmall Pos Team

#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QNetworkReply>
#include "../Http/HttpConsts.h"

#define ASSIGN_TO_INT32(object, name, var)                  \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(int));               \
        if(object.contains(name) && !object[name].isNull()) \
            var = object[name].toInt();                     \
    } while(0)

#define ASSIGN_TO_INT64(object, name, var)                  \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(qint64));            \
        if(object.contains(name) && !object[name].isNull()) \
            var = object[name].toVariant().toLongLong();    \
    } while(0)

#define ASSIGN_TO_STRING(object, name, var)                 \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(QString));           \
        if(object.contains(name) && !object[name].isNull()) \
            var = object[name].toString();                  \
    } while(0)

#define ASSIGN_TO_LSTRING(object, name, defVal, var)        \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(Base::LString)); \
        var = Base::LStringUtil::FromLanMap(object, name, defVal); \
    } while(0)                                              \

#define ASSIGN_TO_BOOL(object, name, var)                   \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(bool));              \
        if(object.contains(name) && !object[name].isNull()) \
            var = object[name].toBool();                    \
    } while(0)

#define ASSIGN_TO_DOUBLE(object, name, var)                 \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(double));            \
        if(object.contains(name) && !object[name].isNull()) \
            var = object[name].toDouble();                  \
    } while(0)

#define ASSIGN_TO_DATETIME(object, name, var)               \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(QDateTime));         \
        if(object.contains(name) && !object[name].isNull()) \
            var = object[name].toVariant().toDateTime();    \
    } while(0)

#define ASSIGN_TO_OBJECT(object, name, var)                 \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(QJsonObject));       \
        if(object.contains(name) && !object[name].isNull()) \
            var = object[name].toObject();                  \
    } while(0)

#define ASSIGN_TO_ARRAY(object, name, var)                  \
    do                                                      \
    {                                                       \
        Q_ASSERT(typeid(var) == typeid(QJsonArray));        \
        if(object.contains(name) && !object[name].isNull()) \
            var = object[name].toArray();                   \
    } while(0)

namespace Offline {

    class OfflineServiceTask;
}

namespace Net
{
    /** HTTP响应类 */
    class HttpResponse : public QObject
    {
        Q_OBJECT
    public:
        friend class HttpTask;
        friend class Offline::OfflineServiceTask;

        HttpResponse(QObject* parent = nullptr);
        virtual ~HttpResponse();

        /** 设置是否返回日志 */
        void SetLogEnabled(bool enabled);

        /** 设置请求日志打印格式 */
        void SetLogFormat(LogFormat format);

        /** 获取是否打印返回日志 */
        bool IsLogEnabled() const;

        /** 获取返回日志格式 */
        LogFormat GetLogFormat() const;

        /** 获取请求是否成功 */
        virtual bool IsOk() const;

        /** 获取网络错误码 */
        virtual const QString& GetError() const;

        /** 获取界面显示的错误码 */
        virtual const QString& GetDisplayErrorCode() const;

        /** 获取网络错误描述 */
        virtual const QString& GetErrorMessage() const;

        /**获取鉴权code  */
        virtual const QString& GetActionCode() const;

        /** 响应流水号 */
        const QString& GetTraceNumber() const;

        /** 响应耗时(ms) */
        int GetElapsedTime() const;

        /** HTTP状态码 */
        int GetStatusCode() const;

        /** 获取数据 */
        const QByteArray& GetData() const;

        /** 清空数据 */
        void ClearData();

        /** 设置网络错误描述 */
        void SetErrorMessage(const QString& msg);

        /** 设置网络错误码 */
        void SetError(const QString& code);

    protected:
        /** 反序列化响应数据 */
        virtual void Deserialize(const QByteArray& ba);

        /** 设置追踪流水号，与Resquest相同 */
        void SetTraceNumber(const QString& number);

        /** 设置响应耗时(ms) */
        void SetElapsedTime(int time);

        /** 设置HTTP状态码 */
        void SetStatusCode(int code);

        /** 追加响应数据 */
        void AppendData(const QByteArray& ba);

    protected:
        // 是否打印返回日志
        bool m_logEnabled = true;

        // 返回日志格式
        LogFormat m_logFormat = LogFormat::DETAIL;

        // 流水号
        QString m_traceNumber;

        // HTTP状态码
        int m_statusCode = 0;

        // 响应耗时（毫秒）
        int m_elapsed = 0;

        // 网络错误码
        QString m_error;

        // 界面要显示的错误码
        QString m_displayErrorCode;

        // 网络错误描述
        QString m_errorMessage;

        // 响应数据
        QByteArray m_data;

        // 鉴权code
        QString m_actionCode;
    };

    using HttpResponsePtr = QSharedPointer<Net::HttpResponse>;
}

Q_DECLARE_METATYPE(Net::HttpResponsePtr)
