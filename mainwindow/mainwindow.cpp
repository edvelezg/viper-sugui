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
    action_New->setStatusTip(tr("Create a new spreadsheet file"));
    connect(action_New, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(action_New);
}

void MainWindow::newFile()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}
