#include <QtGui>
#include "mainwindow.h"
#include "seismicu.h"
#include "convertdialog.h"

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolBars();
    suDialog=0; // Initializing as null, so that it is only initialized one time;
    dialog=0;
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/images/save.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing spreadsheet file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(exitAction);
}


void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
}

void MainWindow::newFile()
{
    if (!suDialog) {
        suDialog = new SeismicU;
    }
    this->setCentralWidget( suDialog );
    suDialog->show();
}


void MainWindow::openFile()
{    
    if (!dialog) {
        dialog = new ConvertDialog;
    }
    this->setCentralWidget( dialog );
    dialog->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}
