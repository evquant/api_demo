/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   12,   11,   11, 0x08,
      79,   69,   11,   11, 0x08,
     146,  134,   11,   11, 0x08,
     199,   11,   11,   11, 0x08,
     218,   11,   11,   11, 0x08,
     231,   11,   11,   11, 0x08,
     244,   11,   11,   11, 0x08,
     261,   11,   11,   11, 0x08,
     283,   11,   11,   11, 0x08,
     305,  301,   11,   11, 0x08,
     332,   11,   11,   11, 0x08,
     355,  345,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0event,title\0"
    "titleDoubleClick(QMouseEvent*,QCPPlotTitle*)\0"
    "axis,part\0"
    "axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)\0"
    "legend,item\0"
    "legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)\0"
    "selectionChanged()\0mousePress()\0"
    "mouseWheel()\0addRandomGraph()\0"
    "removeSelectedGraph()\0removeAllGraphs()\0"
    "pos\0contextMenuRequest(QPoint)\0"
    "moveLegend()\0plottable\0"
    "graphClicked(QCPAbstractPlottable*)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->titleDoubleClick((*reinterpret_cast< QMouseEvent*(*)>(_a[1])),(*reinterpret_cast< QCPPlotTitle*(*)>(_a[2]))); break;
        case 1: _t->axisLabelDoubleClick((*reinterpret_cast< QCPAxis*(*)>(_a[1])),(*reinterpret_cast< QCPAxis::SelectablePart(*)>(_a[2]))); break;
        case 2: _t->legendDoubleClick((*reinterpret_cast< QCPLegend*(*)>(_a[1])),(*reinterpret_cast< QCPAbstractLegendItem*(*)>(_a[2]))); break;
        case 3: _t->selectionChanged(); break;
        case 4: _t->mousePress(); break;
        case 5: _t->mouseWheel(); break;
        case 6: _t->addRandomGraph(); break;
        case 7: _t->removeSelectedGraph(); break;
        case 8: _t->removeAllGraphs(); break;
        case 9: _t->contextMenuRequest((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 10: _t->moveLegend(); break;
        case 11: _t->graphClicked((*reinterpret_cast< QCPAbstractPlottable*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
