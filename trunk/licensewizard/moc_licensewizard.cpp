/****************************************************************************
** Meta object code from reading C++ file 'licensewizard.h'
**
** Created: Mon Mar 23 12:38:31 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "licensewizard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'licensewizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LicenseWizard[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LicenseWizard[] = {
    "LicenseWizard\0\0showHelp()\0"
};

const QMetaObject LicenseWizard::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_LicenseWizard,
      qt_meta_data_LicenseWizard, 0 }
};

const QMetaObject *LicenseWizard::metaObject() const
{
    return &staticMetaObject;
}

void *LicenseWizard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LicenseWizard))
        return static_cast<void*>(const_cast< LicenseWizard*>(this));
    return QWizard::qt_metacast(_clname);
}

int LicenseWizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showHelp(); break;
        }
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_IntroPage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_IntroPage[] = {
    "IntroPage\0"
};

const QMetaObject IntroPage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_IntroPage,
      qt_meta_data_IntroPage, 0 }
};

const QMetaObject *IntroPage::metaObject() const
{
    return &staticMetaObject;
}

void *IntroPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IntroPage))
        return static_cast<void*>(const_cast< IntroPage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int IntroPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_EvaluatePage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_EvaluatePage[] = {
    "EvaluatePage\0"
};

const QMetaObject EvaluatePage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_EvaluatePage,
      qt_meta_data_EvaluatePage, 0 }
};

const QMetaObject *EvaluatePage::metaObject() const
{
    return &staticMetaObject;
}

void *EvaluatePage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EvaluatePage))
        return static_cast<void*>(const_cast< EvaluatePage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int EvaluatePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_RegisterPage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_RegisterPage[] = {
    "RegisterPage\0"
};

const QMetaObject RegisterPage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_RegisterPage,
      qt_meta_data_RegisterPage, 0 }
};

const QMetaObject *RegisterPage::metaObject() const
{
    return &staticMetaObject;
}

void *RegisterPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RegisterPage))
        return static_cast<void*>(const_cast< RegisterPage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int RegisterPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DetailsPage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_DetailsPage[] = {
    "DetailsPage\0"
};

const QMetaObject DetailsPage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_DetailsPage,
      qt_meta_data_DetailsPage, 0 }
};

const QMetaObject *DetailsPage::metaObject() const
{
    return &staticMetaObject;
}

void *DetailsPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DetailsPage))
        return static_cast<void*>(const_cast< DetailsPage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int DetailsPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_ConclusionPage[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConclusionPage[] = {
    "ConclusionPage\0\0printButtonClicked()\0"
};

const QMetaObject ConclusionPage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_ConclusionPage,
      qt_meta_data_ConclusionPage, 0 }
};

const QMetaObject *ConclusionPage::metaObject() const
{
    return &staticMetaObject;
}

void *ConclusionPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConclusionPage))
        return static_cast<void*>(const_cast< ConclusionPage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int ConclusionPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: printButtonClicked(); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
