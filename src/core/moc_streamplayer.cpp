/****************************************************************************
** Meta object code from reading C++ file 'streamplayer.h'
**
** Created: Sat Nov 13 23:39:04 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "streamplayer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'streamplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StreamPlayer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      21,   13,   13,   13, 0x0a,
      32,   28,   13,   13, 0x0a,
      52,   48,   13,   13, 0x0a,
      64,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_StreamPlayer[] = {
    "StreamPlayer\0\0play()\0stop()\0url\0"
    "setUrl(QString)\0val\0setVol(int)\0"
    "update()\0"
};

const QMetaObject StreamPlayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_StreamPlayer,
      qt_meta_data_StreamPlayer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StreamPlayer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StreamPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StreamPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StreamPlayer))
        return static_cast<void*>(const_cast< StreamPlayer*>(this));
    return QObject::qt_metacast(_clname);
}

int StreamPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: play(); break;
        case 1: stop(); break;
        case 2: setUrl((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: setVol((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: update(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
