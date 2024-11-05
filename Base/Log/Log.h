#pragma once

#include <QString>
#include <memory>
#include "AsyncLogger.h"

#define LOG_INFO(msg)                     do { Q_ASSERT(Base::GetLogger()); Base::GetLogger()->writeLog(msg, LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__); } while(0)
#define LOG_WARN(msg)                     do { Q_ASSERT(Base::GetLogger()); Base::GetLogger()->writeLog(msg, LOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__); } while(0)
#define LOG_ERROR(msg)                    do { Q_ASSERT(Base::GetLogger()); Base::GetLogger()->writeLog(msg, LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__); } while(0)

namespace Base
{
    /** 开启日志 */
    void InitLogging(const QString& dirPath);

    /** 返回日志模块 */
    std::shared_ptr<AsyncLogger> GetLogger();
}
