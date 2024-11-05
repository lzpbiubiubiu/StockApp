#include <QMetaEnum>
#include <QDateTime>
#include "Application.h"
#include "KeyEventFilter.h"

namespace UI
{
    KeyEventFilter::KeyEventFilter(QObject* parent) : QObject(parent)
    {
        m_timer = new QTimer(this);
        m_timer->setInterval(50);
    }

    KeyEventFilter::~KeyEventFilter() { }

    bool KeyEventFilter::eventFilter(QObject* watched, QEvent* e)
    {
        bool handled = QObject::eventFilter(watched, e);

        // 只处理按下事件
        if(e->type() != QKeyEvent::KeyPress)
            return handled;

        // 过滤 CTRL SHIFT ALT
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        if(keyEvent->key() == Qt::Key_Control || keyEvent->key() == Qt::Key_Shift || keyEvent->key() == Qt::Key_Alt)
            return handled;

        // 过滤Auto-Repeat
        if(keyEvent->isAutoRepeat())
            return handled;

        // 只处理顶层路由，防止重复命中，事件最后路由到MainWindow.qml
        if(!QString(watched->metaObject()->className()).startsWith("MainWindow"))
            return handled;

        qint64 eventTimestamp = QDateTime::currentMSecsSinceEpoch();

        m_timestamp = eventTimestamp;
        m_virtualKey = QString::number(keyEvent->nativeVirtualKey());
        m_keyCode = keyEvent->text();

        // 确保事件队列不为空
        if(!m_timer->isActive())
            m_timer->start();

        return handled;
    }
}
