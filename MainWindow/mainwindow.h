#ifndef MAINWINDOW_H_MX6J3SE
#define MAINWINDOW_H_MX6J3SE

#include <QDialog>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
};

#endif /* end of include guard: MAINWINDOW_H_MX6J3SE */

