/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sun Jan 9 17:20:46 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      38,   11,   11,   11, 0x08,
      65,   11,   11,   11, 0x08,
      92,   11,   11,   11, 0x08,
     124,   11,   11,   11, 0x08,
     156,   11,   11,   11, 0x08,
     188,   11,   11,   11, 0x08,
     220,   11,   11,   11, 0x08,
     248,   11,   11,   11, 0x08,
     257,   11,   11,   11, 0x08,
     276,  268,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_actionStop_triggered()\0"
    "on_actionReset_triggered()\0"
    "on_actionStart_triggered()\0"
    "on_action15_minutes_activated()\0"
    "on_action35_minutes_triggered()\0"
    "on_action25_minutes_triggered()\0"
    "on_action45_minutes_activated()\0"
    "on_action1_hour_triggered()\0update()\0"
    "endTimer()\0checked\0"
    "on_actionAlways_on_Top_triggered(bool)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
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
        switch (_id) {
        case 0: on_actionStop_triggered(); break;
        case 1: on_actionReset_triggered(); break;
        case 2: on_actionStart_triggered(); break;
        case 3: on_action15_minutes_activated(); break;
        case 4: on_action35_minutes_triggered(); break;
        case 5: on_action25_minutes_triggered(); break;
        case 6: on_action45_minutes_activated(); break;
        case 7: on_action1_hour_triggered(); break;
        case 8: update(); break;
        case 9: endTimer(); break;
        case 10: on_actionAlways_on_Top_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
