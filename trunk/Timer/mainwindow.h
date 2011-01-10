#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
class QTimer;
class QSound;

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
    void alwaysOnTop(bool checked);
    void readSettings();
    void writeSettings();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QString geometry;
    QSound *soundfile;
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
};

#endif // MAINWINDOW_H
