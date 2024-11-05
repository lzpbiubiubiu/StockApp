#pragma once

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QThread>
#include <QReadWriteLock>
#include <QMetaObject>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>

#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3

namespace Base {

    struct LogInfo
    {
        /** 日志信息 */
        QString logMessage;

        /** 当前调用日志的文件名 */
        QString writeLogFileName;

        /** 当前调用日志的函数名 */
        QString writeLogFunName;

        /** 当前调用日志的文件行号 */
        int writeLogFileLine = 0;

        /** 当前日志等级 */
        QString level = "info";
    };

    class AsyncLogger : public QObject
    {
        Q_OBJECT

    public:
        explicit AsyncLogger(const QString& path, QObject *parent = nullptr);
        ~AsyncLogger();

        void writeLog(const QString &message, unsigned short int level, const char* file, int line, const char* function);

    Q_SIGNALS:
        void signalProcessLogs();

    private slots:
        void onProcessLogs();
        void onCheckLogFileSize();

    protected:
        void RenameFilesInDirectory();

    private:
        QThread m_thread;
        QFile m_file;
        QTextStream m_textStream;

        QMutex m_mutex;

        // 日志信息列表
        QList<LogInfo> m_logQueue;

        // 日志路径
        QString m_logPath;
    };

}
