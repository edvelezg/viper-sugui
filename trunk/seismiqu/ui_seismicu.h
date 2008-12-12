/********************************************************************************
** Form generated from reading ui file 'seismicu.ui'
**
** Created: Wed Dec 10 20:38:22 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SEISMICU_H
#define UI_SEISMICU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SeismicU
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox;
    QLabel *label_2;
    QSpinBox *spinBox_2;
    QSpacerItem *horizontalSpacer;
    QTextEdit *outputTextEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *SeismicU)
    {
    if (SeismicU->objectName().isEmpty())
        SeismicU->setObjectName(QString::fromUtf8("SeismicU"));
    SeismicU->resize(387, 300);
    gridLayout = new QGridLayout(SeismicU);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(SeismicU);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout->addWidget(label);

    spinBox = new QSpinBox(SeismicU);
    spinBox->setObjectName(QString::fromUtf8("spinBox"));
    spinBox->setMaximum(500);

    horizontalLayout->addWidget(spinBox);

    label_2 = new QLabel(SeismicU);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout->addWidget(label_2);

    spinBox_2 = new QSpinBox(SeismicU);
    spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
    spinBox_2->setMaximum(500);

    horizontalLayout->addWidget(spinBox_2);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);


    gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

    outputTextEdit = new QTextEdit(SeismicU);
    outputTextEdit->setObjectName(QString::fromUtf8("outputTextEdit"));
    outputTextEdit->setEnabled(true);

    gridLayout->addWidget(outputTextEdit, 1, 0, 1, 1);

    buttonBox = new QDialogButtonBox(SeismicU);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Ok);

    gridLayout->addWidget(buttonBox, 2, 0, 1, 1);


    retranslateUi(SeismicU);
    QObject::connect(buttonBox, SIGNAL(rejected()), SeismicU, SLOT(close()));

    QMetaObject::connectSlotsByName(SeismicU);
    } // setupUi

    void retranslateUi(QWidget *SeismicU)
    {
    SeismicU->setWindowTitle(QApplication::translate("SeismicU", "Viper", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("SeismicU", "xs", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("SeismicU", "zs", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(SeismicU);
    } // retranslateUi

};

namespace Ui {
    class SeismicU: public Ui_SeismicU {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEISMICU_H
