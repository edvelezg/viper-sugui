#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolBars();
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);

//  separatorAction = fileMenu->addSeparator();
//  for (int i = 0; i < MaxRecentFiles; ++i)
//      fileMenu->addAction(recentFileActions[i]);
//  fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
}


void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
}

void MainWindow::newFile()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}
