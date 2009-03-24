#ifndef CLASSWIZARD_H
#define CLASSWIZARD_H

#include <QWizard>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QSpinBox;
class QLineEdit;
class QRadioButton;
class QVector<QLineEdit>;
class QGridLayout;
class QStringList;
QT_END_NAMESPACE

//! [0]
class ClassWizard : public QWizard
{
    Q_OBJECT

public:
    ClassWizard(QWidget *parent = 0);
};

class NumObjectsPage : public QWizardPage
{
    Q_OBJECT

public:
    NumObjectsPage(QWidget *parent = 0);

private:
    QLabel *lblNumObjs;
    QSpinBox *leNumObjs;
};

class ObjectsPage : public QWizardPage
{
    Q_OBJECT

public:
    ObjectsPage(QWidget *parent = 0);

protected:
    void initializePage();
    void cleanupPage();

private:
    QGridLayout *layout;
    QStringList velocities;
	QVector<QSpinBox*> sbObjs; 
	QVector<QLabel*> lblObjs; 
};

#endif
