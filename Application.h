#pragma once
#include <QApplication>
#include <QQmlEngine>
#include <QSharedMemory>
#include "Ui/MainWindow.h"
#include "Core/KeyEmitter.h"
#include "Core/KeyEventFilter.h"
// TOAST 延迟时间（毫秒）
#define TOAST_SHORT_DELAY 1500

namespace UI
{
    /** 提供应用程序运行时数据访问、页面管理等功能 */
    class Application : public QApplication
    {
        Q_OBJECT

    public:
        Application(int& argc, char** argv);
        ~Application();

        /** 获取唯一实例 */
        static Application* Get();

        /** 获取唯一主窗口 */
        static MainWindow* GetMainWindow();

        /** 获取当前QML解释器 */
        QSharedPointer<QQmlEngine> GetQmlEngine() const;

        /** 启动程序 */
        void Launch();

        /** 结束程序 */
        void Exit();

    private:
        // QML解释器
        QSharedPointer<QQmlEngine> m_qmlEngine = nullptr;

        // 主窗口
        MainWindow* m_mainWindow = nullptr;

        // 键盘事件发生器
        Context::KeyEmitter* m_keyEmitter = nullptr;

        // 键盘事件过滤器
        KeyEventFilter* m_keyEventFilter = nullptr;
    };
}
