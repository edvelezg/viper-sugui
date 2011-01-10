/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sun Jan 9 22:38:47 2011
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTimeEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Close;
    QAction *action15_minutes;
    QAction *action25_minutes;
    QAction *action35_minutes;
    QAction *action45_minutes;
    QAction *action1_hour;
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionReset;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTimeEdit *timeEdit;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuSet;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(116, 71);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMouseTracking(false);
        MainWindow->setWindowOpacity(1);
        MainWindow->setDocumentMode(false);
        action_Close = new QAction(MainWindow);
        action_Close->setObjectName(QString::fromUtf8("action_Close"));
        action15_minutes = new QAction(MainWindow);
        action15_minutes->setObjectName(QString::fromUtf8("action15_minutes"));
        action15_minutes->setIconVisibleInMenu(false);
        action25_minutes = new QAction(MainWindow);
        action25_minutes->setObjectName(QString::fromUtf8("action25_minutes"));
        action35_minutes = new QAction(MainWindow);
        action35_minutes->setObjectName(QString::fromUtf8("action35_minutes"));
        action45_minutes = new QAction(MainWindow);
        action45_minutes->setObjectName(QString::fromUtf8("action45_minutes"));
        action1_hour = new QAction(MainWindow);
        action1_hour->setObjectName(QString::fromUtf8("action1_hour"));
        actionStart = new QAction(MainWindow);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        timeEdit = new QTimeEdit(centralWidget);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setBaseSize(QSize(10, 10));
        timeEdit->setMaximumTime(QTime(23, 59, 59));
        timeEdit->setTime(QTime(0, 1, 0));

        gridLayout->addWidget(timeEdit, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 116, 22));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menuSet = new QMenu(menuBar);
        menuSet->setObjectName(QString::fromUtf8("menuSet"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuSet->menuAction());
        menu_File->addAction(actionStart);
        menu_File->addAction(actionStop);
        menu_File->addAction(actionReset);
        menu_File->addAction(action_Close);
        menuSet->addAction(action15_minutes);
        menuSet->addAction(action25_minutes);
        menuSet->addAction(action35_minutes);
        menuSet->addAction(action45_minutes);
        menuSet->addAction(action1_hour);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Timer", 0, QApplication::UnicodeUTF8));
        action_Close->setText(QApplication::translate("MainWindow", "&Close", 0, QApplication::UnicodeUTF8));
        action15_minutes->setText(QApplication::translate("MainWindow", "15 minutes", 0, QApplication::UnicodeUTF8));
        action15_minutes->setShortcut(QApplication::translate("MainWindow", "Ctrl+1", 0, QApplication::UnicodeUTF8));
        action25_minutes->setText(QApplication::translate("MainWindow", "25 minutes", 0, QApplication::UnicodeUTF8));
        action25_minutes->setShortcut(QApplication::translate("MainWindow", "Ctrl+2", 0, QApplication::UnicodeUTF8));
        action35_minutes->setText(QApplication::translate("MainWindow", "35 minutes", 0, QApplication::UnicodeUTF8));
        action35_minutes->setShortcut(QApplication::translate("MainWindow", "Ctrl+3", 0, QApplication::UnicodeUTF8));
        action45_minutes->setText(QApplication::translate("MainWindow", "45 minutes", 0, QApplication::UnicodeUTF8));
        action45_minutes->setShortcut(QApplication::translate("MainWindow", "Ctrl+4", 0, QApplication::UnicodeUTF8));
        action1_hour->setText(QApplication::translate("MainWindow", "1 hour", 0, QApplication::UnicodeUTF8));
        action1_hour->setShortcut(QApplication::translate("MainWindow", "Ctrl+5", 0, QApplication::UnicodeUTF8));
        actionStart->setText(QApplication::translate("MainWindow", "&Start", 0, QApplication::UnicodeUTF8));
        actionStart->setShortcut(QApplication::translate("MainWindow", "Ctrl+G", 0, QApplication::UnicodeUTF8));
        actionStop->setText(QApplication::translate("MainWindow", "S&top", 0, QApplication::UnicodeUTF8));
        actionStop->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionReset->setText(QApplication::translate("MainWindow", "&Reset", 0, QApplication::UnicodeUTF8));
        actionReset->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        timeEdit->setDisplayFormat(QApplication::translate("MainWindow", "hh:mm:ss", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menuSet->setTitle(QApplication::translate("MainWindow", "&Set", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
