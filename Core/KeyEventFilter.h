#pragma once

#include <QObject>
#include <QKeyEvent>
#include <QTimer>

namespace UI
{
    /** 
     * 键盘事件过滤器，实现按键事件和扫码器事件分离
     * 参与过滤的事件以及事件接收对象：
     * 1. 事件：QKeyEvent::KeyPress，但不包括 CTRL SHIFT ALT
     * 2. 对象：非QQuickTextField，paymentPage_amountTextInput和paymentPage_amountTextInput1
     */
    class KeyEventFilter : public QObject
    {
        Q_OBJECT
    public:
        KeyEventFilter(QObject* parent = nullptr);
        ~KeyEventFilter();

        virtual bool eventFilter(QObject* watched, QEvent* e) override;

    Q_SIGNALS:
        /** 键盘按下 */
        void signalKeyPressed(const QString& keyCode);

    private:
        // 事件时间戳
        qint64 m_timestamp = 0;

        // Win32虚拟键码
        QString m_virtualKey;

        // 键码
        QString m_keyCode;

        // 激发定时器
        QTimer* m_timer = nullptr;
    };
}
