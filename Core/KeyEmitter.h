// Copyright (c) 2021 Dmall Pos Team

#pragma once

#include <QObject>

namespace UI
{
    namespace Context
    {
        /** [emitter] QML全局上下文对象，实现键盘事件模拟 */
        class KeyEmitter : public QObject
        {
            Q_OBJECT
        public:
            KeyEmitter(QObject* parent = nullptr);
            ~KeyEmitter();

            Q_INVOKABLE void emitKey(Qt::Key key, int modifier, const QString& text = QString(""));
        };
    }
}