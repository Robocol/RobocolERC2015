/****************************************************************************
** Meta object code from reading C++ file 'qmpwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../robocol/qmpwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qmpwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QMPwidget_t {
    QByteArrayData data[34];
    char stringdata0[331];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QMPwidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QMPwidget_t qt_meta_stringdata_QMPwidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "QMPwidget"
QT_MOC_LITERAL(1, 10, 12), // "stateChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 5), // "state"
QT_MOC_LITERAL(4, 30, 5), // "error"
QT_MOC_LITERAL(5, 36, 6), // "reason"
QT_MOC_LITERAL(6, 43, 18), // "readStandardOutput"
QT_MOC_LITERAL(7, 62, 4), // "line"
QT_MOC_LITERAL(8, 67, 17), // "readStandardError"
QT_MOC_LITERAL(9, 85, 5), // "start"
QT_MOC_LITERAL(10, 91, 4), // "args"
QT_MOC_LITERAL(11, 96, 4), // "load"
QT_MOC_LITERAL(12, 101, 3), // "url"
QT_MOC_LITERAL(13, 105, 4), // "play"
QT_MOC_LITERAL(14, 110, 5), // "pause"
QT_MOC_LITERAL(15, 116, 4), // "stop"
QT_MOC_LITERAL(16, 121, 4), // "seek"
QT_MOC_LITERAL(17, 126, 6), // "offset"
QT_MOC_LITERAL(18, 133, 6), // "whence"
QT_MOC_LITERAL(19, 140, 16), // "toggleFullScreen"
QT_MOC_LITERAL(20, 157, 12), // "writeCommand"
QT_MOC_LITERAL(21, 170, 7), // "command"
QT_MOC_LITERAL(22, 178, 9), // "setVolume"
QT_MOC_LITERAL(23, 188, 6), // "volume"
QT_MOC_LITERAL(24, 195, 14), // "mpStateChanged"
QT_MOC_LITERAL(25, 210, 23), // "mpStreamPositionChanged"
QT_MOC_LITERAL(26, 234, 8), // "position"
QT_MOC_LITERAL(27, 243, 15), // "mpVolumeChanged"
QT_MOC_LITERAL(28, 259, 11), // "delayedSeek"
QT_MOC_LITERAL(29, 271, 5), // "State"
QT_MOC_LITERAL(30, 277, 14), // "streamPosition"
QT_MOC_LITERAL(31, 292, 11), // "videoOutput"
QT_MOC_LITERAL(32, 304, 11), // "mplayerPath"
QT_MOC_LITERAL(33, 316, 14) // "mplayerVersion"

    },
    "QMPwidget\0stateChanged\0\0state\0error\0"
    "reason\0readStandardOutput\0line\0"
    "readStandardError\0start\0args\0load\0url\0"
    "play\0pause\0stop\0seek\0offset\0whence\0"
    "toggleFullScreen\0writeCommand\0command\0"
    "setVolume\0volume\0mpStateChanged\0"
    "mpStreamPositionChanged\0position\0"
    "mpVolumeChanged\0delayedSeek\0State\0"
    "streamPosition\0videoOutput\0mplayerPath\0"
    "mplayerVersion"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QMPwidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       5,  174, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  119,    2, 0x06 /* Public */,
       4,    1,  122,    2, 0x06 /* Public */,
       6,    1,  125,    2, 0x06 /* Public */,
       8,    1,  128,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,  131,    2, 0x0a /* Public */,
       9,    0,  134,    2, 0x2a /* Public | MethodCloned */,
      11,    1,  135,    2, 0x0a /* Public */,
      13,    0,  138,    2, 0x0a /* Public */,
      14,    0,  139,    2, 0x0a /* Public */,
      15,    0,  140,    2, 0x0a /* Public */,
      16,    2,  141,    2, 0x0a /* Public */,
      16,    1,  146,    2, 0x2a /* Public | MethodCloned */,
      16,    2,  149,    2, 0x0a /* Public */,
      16,    1,  154,    2, 0x2a /* Public | MethodCloned */,
      19,    0,  157,    2, 0x0a /* Public */,
      20,    1,  158,    2, 0x0a /* Public */,
      22,    1,  161,    2, 0x08 /* Private */,
      24,    1,  164,    2, 0x08 /* Private */,
      25,    1,  167,    2, 0x08 /* Private */,
      27,    1,  170,    2, 0x08 /* Private */,
      28,    0,  173,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::Int, QMetaType::Int,   17,   18,
    QMetaType::Bool, QMetaType::Int,   17,
    QMetaType::Bool, QMetaType::Double, QMetaType::Int,   17,   18,
    QMetaType::Bool, QMetaType::Double,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,   26,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,

 // properties: name, type, flags
       3, 0x80000000 | 29, 0x00095009,
      30, QMetaType::Double, 0x00095001,
      31, QMetaType::QString, 0x00095103,
      32, QMetaType::QString, 0x00095003,
      33, QMetaType::QString, 0x00095001,

 // enums: name, flags, count, data

 // enum data: key, value

       0        // eod
};

void QMPwidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QMPwidget *_t = static_cast<QMPwidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->readStandardOutput((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->readStandardError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->start((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 5: _t->start(); break;
        case 6: _t->load((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->play(); break;
        case 8: _t->pause(); break;
        case 9: _t->stop(); break;
        case 10: { bool _r = _t->seek((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->seek((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->seek((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->seek((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: _t->toggleFullScreen(); break;
        case 15: _t->writeCommand((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->mpStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->mpStreamPositionChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 19: _t->mpVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->delayedSeek(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QMPwidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QMPwidget::stateChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (QMPwidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QMPwidget::error)) {
                *result = 1;
            }
        }
        {
            typedef void (QMPwidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QMPwidget::readStandardOutput)) {
                *result = 2;
            }
        }
        {
            typedef void (QMPwidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QMPwidget::readStandardError)) {
                *result = 3;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        QMPwidget *_t = static_cast<QMPwidget *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< State*>(_v) = _t->state(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->tell(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->videoOutput(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->mplayerPath(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->mplayerVersion(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        QMPwidget *_t = static_cast<QMPwidget *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setVideoOutput(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setMPlayerPath(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QMPwidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QMPwidget.data,
      qt_meta_data_QMPwidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QMPwidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QMPwidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QMPwidget.stringdata0))
        return static_cast<void*>(const_cast< QMPwidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int QMPwidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QMPwidget::stateChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QMPwidget::error(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QMPwidget::readStandardOutput(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QMPwidget::readStandardError(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
