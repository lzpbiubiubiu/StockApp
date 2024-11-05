/****************************************************************************
** Meta object code from reading C++ file 'ServiceManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Service/ServiceManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ServiceManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Base__ServiceManager_t {
    QByteArrayData data[12];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Base__ServiceManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Base__ServiceManager_t qt_meta_stringdata_Base__ServiceManager = {
    {
QT_MOC_LITERAL(0, 0, 20), // "Base::ServiceManager"
QT_MOC_LITERAL(1, 21, 12), // "signalLoaded"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13), // "StartupOption"
QT_MOC_LITERAL(4, 49, 6), // "option"
QT_MOC_LITERAL(5, 56, 15), // "signalLoadError"
QT_MOC_LITERAL(6, 72, 7), // "message"
QT_MOC_LITERAL(7, 80, 14), // "signalUnloaded"
QT_MOC_LITERAL(8, 95, 6), // "onLoad"
QT_MOC_LITERAL(9, 102, 8), // "onUnload"
QT_MOC_LITERAL(10, 111, 4), // "BOOT"
QT_MOC_LITERAL(11, 116, 4) // "LAZY"

    },
    "Base::ServiceManager\0signalLoaded\0\0"
    "StartupOption\0option\0signalLoadError\0"
    "message\0signalUnloaded\0onLoad\0onUnload\0"
    "BOOT\0LAZY"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Base__ServiceManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       1,   50, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       7,    0,   45,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
       8,    1,   46,    2, 0x01 /* Protected */,
       9,    0,   49,    2, 0x01 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       3,    3, 0x0,    2,   55,

 // enum data: key, value
      10, uint(Base::ServiceManager::BOOT),
      11, uint(Base::ServiceManager::LAZY),

       0        // eod
};

void Base::ServiceManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ServiceManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalLoaded((*reinterpret_cast< StartupOption(*)>(_a[1]))); break;
        case 1: _t->signalLoadError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->signalUnloaded(); break;
        case 3: _t->onLoad((*reinterpret_cast< StartupOption(*)>(_a[1]))); break;
        case 4: _t->onUnload(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ServiceManager::*)(StartupOption );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServiceManager::signalLoaded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ServiceManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServiceManager::signalLoadError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ServiceManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServiceManager::signalUnloaded)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Base::ServiceManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Base__ServiceManager.data,
    qt_meta_data_Base__ServiceManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Base::ServiceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Base::ServiceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Base__ServiceManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Base::ServiceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Base::ServiceManager::signalLoaded(StartupOption _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Base::ServiceManager::signalLoadError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Base::ServiceManager::signalUnloaded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
