#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTimer>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

private slots:
    void on_btnReset_clicked();
    void on_btnStop_clicked();
    void on_btnStart_clicked();
    void update();
    void endTimer();
};

#endif // MAINWINDOW_H
