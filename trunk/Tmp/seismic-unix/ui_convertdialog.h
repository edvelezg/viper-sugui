/********************************************************************************
** Form generated from reading ui file 'convertdialog.ui'
**
** Created: Tue Nov 11 07:04:46 2008
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONVERTDIALOG_H
#define UI_CONVERTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ConvertDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *sourceFileLabel;
    QLineEdit *sourceFileEdit;
    QPushButton *browseButton;
    QLabel *targetFormatLabel;
    QComboBox *targetFormatComboBox;
    QSpacerItem *spacerItem;
    QGroupBox *optionsGroupBox;
    QHBoxLayout *hboxLayout;
    QCheckBox *enhanceCheckBox;
    QCheckBox *monochromeCheckBox;
    QTextEdit *outputTextEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConvertDialog)
    {
    if (ConvertDialog->objectName().isEmpty())
        ConvertDialog->setObjectName(QString::fromUtf8("ConvertDialog"));
    ConvertDialog->resize(285, 327);
    gridLayout = new QGridLayout(ConvertDialog);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    sourceFileLabel = new QLabel(ConvertDialog);
    sourceFileLabel->setObjectName(QString::fromUtf8("sourceFileLabel"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(sourceFileLabel->sizePolicy().hasHeightForWidth());
    sourceFileLabel->setSizePolicy(sizePolicy);
    sourceFileLabel->setFrameShape(QFrame::NoFrame);
    sourceFileLabel->setFrameShadow(QFrame::Plain);

    gridLayout->addWidget(sourceFileLabel, 0, 0, 1, 1);

    sourceFileEdit = new QLineEdit(ConvertDialog);
    sourceFileEdit->setObjectName(QString::fromUtf8("sourceFileEdit"));

    gridLayout->addWidget(sourceFileEdit, 0, 1, 1, 2);

    browseButton = new QPushButton(ConvertDialog);
    browseButton->setObjectName(QString::fromUtf8("browseButton"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(browseButton->sizePolicy().hasHeightForWidth());
    browseButton->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(browseButton, 0, 3, 1, 1);

    targetFormatLabel = new QLabel(ConvertDialog);
    targetFormatLabel->setObjectName(QString::fromUtf8("targetFormatLabel"));
    sizePolicy.setHeightForWidth(targetFormatLabel->sizePolicy().hasHeightForWidth());
    targetFormatLabel->setSizePolicy(sizePolicy);

    gridLayout->addWidget(targetFormatLabel, 1, 0, 1, 1);

    targetFormatComboBox = new QComboBox(ConvertDialog);
    targetFormatComboBox->setObjectName(QString::fromUtf8("targetFormatComboBox"));
    sizePolicy1.setHeightForWidth(targetFormatComboBox->sizePolicy().hasHeightForWidth());
    targetFormatComboBox->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(targetFormatComboBox, 1, 1, 1, 1);

    spacerItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem, 1, 2, 1, 2);

    optionsGroupBox = new QGroupBox(ConvertDialog);
    optionsGroupBox->setObjectName(QString::fromUtf8("optionsGroupBox"));
    hboxLayout = new QHBoxLayout(optionsGroupBox);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    enhanceCheckBox = new QCheckBox(optionsGroupBox);
    enhanceCheckBox->setObjectName(QString::fromUtf8("enhanceCheckBox"));

    hboxLayout->addWidget(enhanceCheckBox);

    monochromeCheckBox = new QCheckBox(optionsGroupBox);
    monochromeCheckBox->setObjectName(QString::fromUtf8("monochromeCheckBox"));

    hboxLayout->addWidget(monochromeCheckBox);


    gridLayout->addWidget(optionsGroupBox, 2, 0, 1, 4);

    outputTextEdit = new QTextEdit(ConvertDialog);
    outputTextEdit->setObjectName(QString::fromUtf8("outputTextEdit"));
    outputTextEdit->setReadOnly(true);

    gridLayout->addWidget(outputTextEdit, 3, 0, 1, 4);

    buttonBox = new QDialogButtonBox(ConvertDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    gridLayout->addWidget(buttonBox, 4, 1, 1, 3);


#ifndef QT_NO_SHORTCUT
    sourceFileLabel->setBuddy(sourceFileEdit);
    targetFormatLabel->setBuddy(targetFormatComboBox);
#endif // QT_NO_SHORTCUT

    QWidget::setTabOrder(sourceFileEdit, browseButton);
    QWidget::setTabOrder(browseButton, targetFormatComboBox);
    QWidget::setTabOrder(targetFormatComboBox, enhanceCheckBox);
    QWidget::setTabOrder(enhanceCheckBox, monochromeCheckBox);
    QWidget::setTabOrder(monochromeCheckBox, outputTextEdit);

    retranslateUi(ConvertDialog);

    QMetaObject::connectSlotsByName(ConvertDialog);
    } // setupUi

    void retranslateUi(QDialog *ConvertDialog)
    {
    ConvertDialog->setWindowTitle(QApplication::translate("ConvertDialog", "Image Converter", 0, QApplication::UnicodeUTF8));
    sourceFileLabel->setText(QApplication::translate("ConvertDialog", "&Source File:", 0, QApplication::UnicodeUTF8));
    browseButton->setText(QApplication::translate("ConvertDialog", "&Browse", 0, QApplication::UnicodeUTF8));
    browseButton->setShortcut(QApplication::translate("ConvertDialog", "Alt+B", 0, QApplication::UnicodeUTF8));
    targetFormatLabel->setText(QApplication::translate("ConvertDialog", "&Target Format:", 0, QApplication::UnicodeUTF8));
    targetFormatComboBox->clear();
    targetFormatComboBox->insertItems(0, QStringList()
     << QApplication::translate("ConvertDialog", "BMP", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ConvertDialog", "EPS", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ConvertDialog", "GIF", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ConvertDialog", "JPEG", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ConvertDialog", "PNG", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("ConvertDialog", "TIFF", 0, QApplication::UnicodeUTF8)
    );
    optionsGroupBox->setTitle(QApplication::translate("ConvertDialog", "Options", 0, QApplication::UnicodeUTF8));
    enhanceCheckBox->setText(QApplication::translate("ConvertDialog", "&Enhance", 0, QApplication::UnicodeUTF8));
    enhanceCheckBox->setShortcut(QApplication::translate("ConvertDialog", "Alt+E", 0, QApplication::UnicodeUTF8));
    monochromeCheckBox->setText(QApplication::translate("ConvertDialog", "&Monochrome", 0, QApplication::UnicodeUTF8));
    monochromeCheckBox->setShortcut(QApplication::translate("ConvertDialog", "Alt+M", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ConvertDialog);
    } // retranslateUi

};

namespace Ui {
    class ConvertDialog: public Ui_ConvertDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERTDIALOG_H
