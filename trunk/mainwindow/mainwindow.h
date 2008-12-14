#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();

private:
    void createActions();
    void createToolBars();

    QAction *exitAction;
    QAction *newAction;

    QToolBar *fileToolBar;
};

#endif
