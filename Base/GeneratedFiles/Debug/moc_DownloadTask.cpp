/****************************************************************************
** Meta object code from reading C++ file 'DownloadTask.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Http/Client/DownloadTask.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DownloadTask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Http__DownloadTask_t {
    QByteArrayData data[14];
    char stringdata0[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Http__DownloadTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Http__DownloadTask_t qt_meta_stringdata_Http__DownloadTask = {
    {
QT_MOC_LITERAL(0, 0, 18), // "Http::DownloadTask"
QT_MOC_LITERAL(1, 19, 16), // "onReplyReadReady"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 15), // "onReplyFinished"
QT_MOC_LITERAL(4, 53, 12), // "onReplyError"
QT_MOC_LITERAL(5, 66, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(6, 94, 4), // "code"
QT_MOC_LITERAL(7, 99, 16), // "onReplySslErrors"
QT_MOC_LITERAL(8, 116, 16), // "QList<QSslError>"
QT_MOC_LITERAL(9, 133, 6), // "errors"
QT_MOC_LITERAL(10, 140, 23), // "onReplyDownloadProgress"
QT_MOC_LITERAL(11, 164, 4), // "recv"
QT_MOC_LITERAL(12, 169, 5), // "total"
QT_MOC_LITERAL(13, 175, 9) // "onTimeout"

    },
    "Http::DownloadTask\0onReplyReadReady\0"
    "\0onReplyFinished\0onReplyError\0"
    "QNetworkReply::NetworkError\0code\0"
    "onReplySslErrors\0QList<QSslError>\0"
    "errors\0onReplyDownloadProgress\0recv\0"
    "total\0onTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Http__DownloadTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x09 /* Protected */,
       3,    0,   45,    2, 0x09 /* Protected */,
       4,    1,   46,    2, 0x09 /* Protected */,
       7,    1,   49,    2, 0x09 /* Protected */,
      10,    2,   52,    2, 0x09 /* Protected */,
      13,    0,   57,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   11,   12,
    QMetaType::Void,

       0        // eod
};

void Http::DownloadTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DownloadTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onReplyReadReady(); break;
        case 1: _t->onReplyFinished(); break;
        case 2: _t->onReplyError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 3: _t->onReplySslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 4: _t->onReplyDownloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 5: _t->onTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Http::DownloadTask::staticMetaObject = { {
    &Task::staticMetaObject,
    qt_meta_stringdata_Http__DownloadTask.data,
    qt_meta_data_Http__DownloadTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Http::DownloadTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Http::DownloadTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Http__DownloadTask.stringdata0))
        return static_cast<void*>(this);
    return Task::qt_metacast(_clname);
}

int Http::DownloadTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Task::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
