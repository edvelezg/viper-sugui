#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    setupUi( this );
    createActions();
    createToolBars();
}

void MainWindow::createActions()
{
    action_New->setIcon(QIcon(":/images/new.png"));
    action_New->setShortcut(QKeySequence::New);
    action_New->setStatusTip(tr("Create a new * file"));
    connect(action_New, SIGNAL(triggered()), this, SLOT(newFile()));

    actionSave->setIcon(QIcon(":/images/save.png"));
    actionSave->setShortcut(QKeySequence::Save);
    actionSave->setStatusTip(tr("Save a * file"));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(action_New);
    fileToolBar->addAction(actionSave);
}

void MainWindow::newFile()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::saveFile()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}
