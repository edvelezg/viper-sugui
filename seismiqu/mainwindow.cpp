#include <QtGui>
#include "mainwindow.h"
#include "seismicu.h"

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolBars();
    suDialog=0; // Initializing as null, so that it is only initialized one time;
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
    fileMenu->addAction(exitAction);
}


void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
}

void MainWindow::newFile()
{
    if (!suDialog) {
        suDialog = new SeismicU;
    }
    this->setCentralWidget( suDialog );
    suDialog->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    suDialog->hide();
}
