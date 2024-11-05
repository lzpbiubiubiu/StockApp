#include <QDir>
#include <QRegularExpression>
#include "AsyncLogger.h"

const qint64 g_maxFileSize = 5 * 1024 * 1024;
const int g_maxFileCount = 100;

namespace Base
{
    AsyncLogger::AsyncLogger(const QString& path, QObject *parent)
        : QObject(parent),
          m_logPath(path)
    {
        moveToThread(&m_thread);

        // 打开日志文件（以追加模式）
        m_file.setFileName(path + "Main.log");
        if(m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            m_textStream.setDevice(&m_file);
        }

        // 启动日志处理槽函数和文件大小检查槽函数
        connect(this, &AsyncLogger::signalProcessLogs, this, &AsyncLogger::onProcessLogs, Qt::QueuedConnection);
        connect(this, &AsyncLogger::signalProcessLogs, this, &AsyncLogger::onCheckLogFileSize, Qt::QueuedConnection);
        m_thread.start();
    }

    AsyncLogger::~AsyncLogger()
    {
        // 停止日志处理槽函数和文件大小检查槽函数
        disconnect(this, &AsyncLogger::signalProcessLogs, this, &AsyncLogger::onProcessLogs);
        disconnect(this, &AsyncLogger::signalProcessLogs, this, &AsyncLogger::onCheckLogFileSize);

        m_file.close();
        m_thread.quit();
        m_thread.wait(20000);
    }

    void AsyncLogger::writeLog(const QString &message, unsigned short int level, const char* file, int line, const char* function)
    {
        // 将日志消息添加到队列
        QMutexLocker locker(&m_mutex);
        LogInfo logInfo;
        logInfo.logMessage = message;
        logInfo.writeLogFileName = QFileInfo(file).fileName();
        logInfo.writeLogFileLine = line;
        logInfo.writeLogFunName = QString(function);
        switch (level) {
            case LOG_LEVEL_INFO:
                logInfo.level = "info";
                break;
            case LOG_LEVEL_WARN:
                logInfo.level = "warnning";
                break;
            case LOG_LEVEL_ERROR:
                logInfo.level = "error";
                break;
            default:
                logInfo.level = "info";
                break;
        }
        m_logQueue.append(logInfo);
        emit signalProcessLogs();
    }

    void AsyncLogger::onProcessLogs()
	{
		// 逐个处理日志消息并写入日志文件
		QMutexLocker locker(&m_mutex);

        for (const auto & item : qAsConst(m_logQueue))
		{
			// 获取当前时间戳
			QDateTime timestamp = QDateTime::currentDateTime();
            auto timeStr = timestamp.toString("yyyy-MM-dd HH:mm:ss");

			// 构建日志行
            QString logLine = QString("[%1][Stock App][%2] [%3:%4][%5]")
                                .arg(item.level)
                                .arg(timeStr)
                                .arg(item.writeLogFileName)
                                .arg(item.writeLogFileLine)
                                .arg(item.writeLogFunName) + item.logMessage;
	
			// 写入日志文件
			m_textStream << logLine << '\n';
			m_textStream.flush();
		}
	
		// 清空日志队列
		m_logQueue.clear();
	}
	
    void AsyncLogger::onCheckLogFileSize()
	{
		// 检查日志文件大小
		if (m_file.size() >= g_maxFileSize)
		{
			// 关闭当前日志文件
			m_textStream.flush();
			m_file.close();
	
			// 滚动日志
			RenameFilesInDirectory();
	
			// 打开新的日志文件（以追加模式）
			m_file.setFileName(m_logPath + "Main.log");
			if(m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
			{
				m_textStream.setDevice(&m_file);
			}
			m_textStream.setDevice(&m_file);
		}
	}
	
	void AsyncLogger::RenameFilesInDirectory()
	{
		QDir directory(m_logPath);
	
		// 判断目录是否存在
		if (!directory.exists()) {
			return;
		}
	
		// 获取目录下所有文件
		QStringList files = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
	
		// 遍历文件列表并逐个重命名
		for (int i = 0; i < files.size(); i++)
		{
			QRegularExpression regex("\\d+");  // 匹配连续的数字
			QString oldName = files[i];
			QRegularExpressionMatch match = regex.match(oldName);
			QString oldFilePath = directory.absoluteFilePath(oldName);
			QString newName;
			auto newFileIndex = i + 1;
			if(!match.hasMatch() && "Main.log" == oldName)
			{
				newName = QString("Main1.log");
			}
			else if(oldName.contains("Main"))
			{
				// 使用正则表达式替换数字为空字符串
				newName = oldName.replace(regex, QString::number(newFileIndex));
			}
			else
			{
				continue;
			}
	
			QString newFilePath = directory.absoluteFilePath(newName);
			QFile::rename(oldFilePath, newFilePath);
			if(newFileIndex > g_maxFileCount)
			{
				QFile newFile(newFilePath);
				newFile.remove();
				return;
			}
		}
	}
}
