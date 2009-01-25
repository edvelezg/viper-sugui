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

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void saveFile();
    void convertImage();

private:
    void createActions();
    void createToolBars();
    QProcess process;
    QString targetFile;
    QToolBar *fileToolBar;
};

#endif
