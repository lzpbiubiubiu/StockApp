#include <QQmlContext>
#include <QSharedMemory>
#include <QTimer>
#include <QQmlComponent>
#include "Base/Common/ServiceManager.h"
#include "Application.h"
#include "Core/StackStruct.h"
#include "Core/BusinessManager.h"
// 模块初始化
static void InitModule()
{
    qRegisterMetaType<Base::ServiceManager::StartupOption>("StartupOption");

    qRegisterMetaType<Core::WareItem>("WareItem");
    qRegisterMetaType<Core::WareItemPtr>("WareItemPtr");
    qRegisterMetaType<Core::WareItemPtrList>("WareItemPtrList");

    qRegisterMetaType<Core::Order>("Order");
    qRegisterMetaType<Core::OrderPtr>("OrderPtr");
    qRegisterMetaType<Core::OrderPtrList>("OrderPtrList");
    qRegisterMetaType<Core::Order::OrderType>("OrderType");

    qRegisterMetaType<Core::User>("User");
    qRegisterMetaType<Core::UserPtr>("UserPtr");
    qRegisterMetaType<Core::UserPtrList>("UserPtrList");

    qRegisterMetaType<Core::OrderReport>("OrderReport");
    qRegisterMetaType<Core::OrderReportPtr>("OrderReportPtr");
    qRegisterMetaType<Core::OrderReportPtrList>("OrderReportPtrList");

    qRegisterMetaType<Core::StockOrderDetail>("StockOrderDetail");
    qRegisterMetaType<Core::StockOrderDetailPtr>("StockOrderDetailPtr");
    qRegisterMetaType<Core::StockOrderDetailPtrList>("StockOrderDetailPtrList");

    qRegisterMetaType<Core::BusinessManager::ItemAction>("ItemAction");
    qRegisterMetaType<Core::BusinessManager::StorageOperate>("StorageOperate");
    qRegisterMetaType<Core::BusinessManager::StockOrderOperate>("StockOrderOperate");
}
Q_COREAPP_STARTUP_FUNCTION(InitModule)

namespace UI
{
    Application::Application(int& argc, char** argv)
        : QApplication(argc, argv)
    {
        m_qmlEngine = QSharedPointer<QQmlEngine>::create();

        m_keyEmitter = new Context::KeyEmitter(this);
        m_keyEventFilter = new KeyEventFilter(this);
        installEventFilter(m_keyEventFilter);
    }

    Application::~Application()
    {
    }

    Application* Application::Get()
    {
        return qobject_cast<Application*>(instance());
    }

    MainWindow* Application::GetMainWindow()
    {
        return qobject_cast<Application*>(instance())->m_mainWindow;
    }

    QSharedPointer<QQmlEngine> Application::GetQmlEngine() const
    {
        return m_qmlEngine;
    }

    void Application::Launch()
    {
        // 添加import指令模块搜索路径
        m_qmlEngine->addImportPath("qrc:/Qml");

        // 注入全局qml对象
        m_qmlEngine->rootContext()->setContextProperty("app", this);
        m_qmlEngine->rootContext()->setContextProperty("emitter", m_keyEmitter);

        // 创建主窗口
        m_mainWindow = new UI::MainWindow(m_qmlEngine);

        // 加载主页面
        m_mainWindow->LoadPage("loginPage");

        // 显示主窗口
        m_mainWindow->Show();
    }

    void Application::Exit()
    {
        quit();
    }
}
