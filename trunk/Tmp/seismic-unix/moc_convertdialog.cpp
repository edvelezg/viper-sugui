/****************************************************************************
** Meta object code from reading C++ file 'convertdialog.h'
**
** Created: Tue Nov 11 07:04:46 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "convertdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'convertdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConvertDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      41,   14,   14,   14, 0x08,
      56,   14,   14,   14, 0x08,
      99,   79,   14,   14, 0x08,
     147,  141,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConvertDialog[] = {
    "ConvertDialog\0\0on_browseButton_clicked()\0"
    "convertImage()\0updateOutputTextEdit()\0"
    "exitCode,exitStatus\0"
    "processFinished(int,QProcess::ExitStatus)\0"
    "error\0processError(QProcess::ProcessError)\0"
};

const QMetaObject ConvertDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ConvertDialog,
      qt_meta_data_ConvertDialog, 0 }
};

const QMetaObject *ConvertDialog::metaObject() const
{
    return &staticMetaObject;
}

void *ConvertDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConvertDialog))
        return static_cast<void*>(const_cast< ConvertDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ConvertDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_browseButton_clicked(); break;
        case 1: convertImage(); break;
        case 2: updateOutputTextEdit(); break;
        case 3: processFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 4: processError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
