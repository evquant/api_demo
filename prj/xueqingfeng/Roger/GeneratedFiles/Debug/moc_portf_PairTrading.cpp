/****************************************************************************
** Meta object code from reading C++ file 'portf_PairTrading.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../portf_PairTrading.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'portf_PairTrading.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_portf_PairTrading[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   36,   18,   18, 0x08,
      70,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_portf_PairTrading[] = {
    "portf_PairTrading\0\0SignalUpdateUi()\0"
    "event\0SlotMouseMove(QMouseEvent*)\0"
    "SlotUpdateUi()\0"
};

void portf_PairTrading::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        portf_PairTrading *_t = static_cast<portf_PairTrading *>(_o);
        switch (_id) {
        case 0: _t->SignalUpdateUi(); break;
        case 1: _t->SlotMouseMove((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->SlotUpdateUi(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData portf_PairTrading::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject portf_PairTrading::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_portf_PairTrading,
      qt_meta_data_portf_PairTrading, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &portf_PairTrading::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *portf_PairTrading::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *portf_PairTrading::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_portf_PairTrading))
        return static_cast<void*>(const_cast< portf_PairTrading*>(this));
    if (!strcmp(_clname, "WindDataSpi"))
        return static_cast< WindDataSpi*>(const_cast< portf_PairTrading*>(this));
    if (!strcmp(_clname, "BaseUi"))
        return static_cast< BaseUi*>(const_cast< portf_PairTrading*>(this));
    return QWidget::qt_metacast(_clname);
}

int portf_PairTrading::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void portf_PairTrading::SignalUpdateUi()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
