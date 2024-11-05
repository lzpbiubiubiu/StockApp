#include <QApplication>
#include <QQuickItem>
#include "KeyEmitter.h"

namespace UI
{
    namespace Context
    {
        KeyEmitter::KeyEmitter(QObject* parent)
            : QObject(parent)
        {}

        KeyEmitter::~KeyEmitter()
        {}

        void KeyEmitter::emitKey(Qt::Key key, int modifier, const QString& text /*= QString("")*/)
        {
            QQuickItem* receiver = qobject_cast<QQuickItem*>(QGuiApplication::focusObject());
            if(receiver != nullptr)
            {
                QString keyText = QKeySequence(key).toString();
                if(!text.isEmpty())
                    keyText = text;
                QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, (Qt::KeyboardModifier)modifier, keyText, true);
                QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, (Qt::KeyboardModifier)modifier);
                QCoreApplication::sendEvent(receiver, &pressEvent);
                QCoreApplication::sendEvent(receiver, &releaseEvent);
            }
        }
    }
}