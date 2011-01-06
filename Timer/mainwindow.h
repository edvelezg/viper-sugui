#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QThread>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow, QThread
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer* iTimer;

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
