#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include <QProcess>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void newFile();
    void saveFile();
    void runSimulation();
    void on_browseButton_clicked();

private:
    void createActions();
    void createToolBars();
    QProcess process;
    QString targetFile;
    QToolBar *fileToolBar;
};

#endif
