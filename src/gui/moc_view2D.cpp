/****************************************************************************
** Meta object code from reading C++ file 'view2D.h'
**
** Created: Wed May 16 17:43:56 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/gui/view2D.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'view2D.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View2D[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,
      20,    7,    7,    7, 0x05,
      31,    7,    7,    7, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_View2D[] = {
    "View2D\0\0rendering()\0rendered()\0"
    "hoveringElement(std::string)\0"
};

const QMetaObject View2D::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_View2D,
      qt_meta_data_View2D, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View2D::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View2D::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View2D::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View2D))
        return static_cast<void*>(const_cast< View2D*>(this));
    return QWidget::qt_metacast(_clname);
}

int View2D::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rendering(); break;
        case 1: rendered(); break;
        case 2: hoveringElement((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void View2D::rendering()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void View2D::rendered()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void View2D::hoveringElement(std::string _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
