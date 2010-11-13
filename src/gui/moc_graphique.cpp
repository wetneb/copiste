/****************************************************************************
** Meta object code from reading C++ file 'graphique.h'
**
** Created: Sat Nov 13 23:39:04 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "graphique.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphique.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Graphique[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Graphique[] = {
    "Graphique\0\0updateScrolling()\0"
};

const QMetaObject Graphique::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Graphique,
      qt_meta_data_Graphique, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Graphique::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Graphique::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Graphique::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Graphique))
        return static_cast<void*>(const_cast< Graphique*>(this));
    return QWidget::qt_metacast(_clname);
}

int Graphique::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateScrolling(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
