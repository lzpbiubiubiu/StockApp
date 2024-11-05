#include <QtGlobal>
#include "Log.h"

// logger
std::shared_ptr<Base::AsyncLogger> g_logger = nullptr;

namespace Base
{
    void InitLogging(const QString& dirPath)
    {
        if(g_logger != nullptr)
            return;
        g_logger = std::make_shared<AsyncLogger>(dirPath);

    }

    std::shared_ptr<AsyncLogger> GetLogger()
    {
        return g_logger;
    }
}
