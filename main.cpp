#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Application.h"
#include "Base/Common/ServiceManager.h"
#include "Base/Log/Log.h"
#include "Core/DatabaseManager.h"
#include "Core/BusinessManager.h"
#include "Core/SequenceGenerator.h"
#include "Core/UrlManager.h"
#include "Core/GarbageManager.h"
int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    UI::Application app(argc, argv);

    // 日志库初始化
    QString logDir = Base::PathUtil::GetLogDir();
    Base::InitLogging(logDir);
    LOG_INFO(QString("================>Stock App Start<================"));

    Base::ServiceManager::Get()->AddService<Core::DatabaseManager>();
    Base::ServiceManager::Get()->AddService<Core::UrlManager>();
    Base::ServiceManager::Get()->AddService<Core::SequenceGenerator>();
    Base::ServiceManager::Get()->AddService<Core::GarbageManager>();
    Base::ServiceManager::Get()->AddService<Core::BusinessManager>();
    app.Launch();
    return app.exec();
}
