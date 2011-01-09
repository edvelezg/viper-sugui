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
    void on_actionStop_triggered();
    void on_actionReset_triggered();
    void on_actionStart_triggered();
    void on_action15_minutes_activated();
    void on_action35_minutes_triggered();
    void on_action25_minutes_triggered();
    void on_action45_minutes_activated();
    void on_action1_hour_triggered();
    void update();
    void endTimer();
    void on_actionAlways_on_Top_triggered(bool checked);
};

#endif // MAINWINDOW_H
