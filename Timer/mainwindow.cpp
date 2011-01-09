#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <qDebug>
#include <QTimer>
#include <QSound>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    QMessageBox::information(this, "Path of this app is:", QDir::currentPath());
    ui->actionStart->setEnabled(false);
    on_actionAlways_on_Top_triggered(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update(){
    QTime newTime( ui->timeEdit->time() );
    if (newTime == QTime(0, 0, 0))
    {
        QSound::play("../../../sounds/alarm-clock1.wav");
        endTimer();
    }
    else
    {
        newTime = newTime.addSecs( -1 ); // doh
        ui->timeEdit->setTime( newTime );
    }
}

void MainWindow::on_actionAlways_on_Top_triggered(bool checked)
{
#ifdef Q_OS_WIN
    // #include <windows.h>
    if (checked)
    {
        SetWindowPos(this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    else
    {
        SetWindowPos(this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
#else
    Qt::WindowFlags flags = this->windowFlags();
    if (checked)
    {
        this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->show();
    }
    else
    {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }
#endif
}

//void MainWindow::on_pushButton_clicked()
//{
//    timer->stop();
////    QString timeText = ui->timeEdit->text();
////    QStringList list = timeText.split(":");
////    int hour = list[0].toInt();
////    int min = list[1].toInt();
////    int sec = list[2].toInt();
//
//
////    QStringList::const_iterator constIterator;
////     for (constIterator = list.constBegin(); constIterator != list.constEnd();
////            ++constIterator)
////         qDebug() << (*constIterator).toLocal8Bit().constData() << " ";
//
////    int duration = 3600*hour + 60*min + sec;
//
//
//
////    MainWindow::msleep(1000);
////    ui->timeEdit->setTime(QTime(0, 1, 2));
////    MainWindow::msleep(1000);
////    ui->timeEdit->setTime(QTime(0, 2, 3));
////    MainWindow::msleep(1000);
////    ui->timeEdit->setTime(QTime(0, 1, 2));
////    MainWindow::msleep(1000);
////    ui->timeEdit->setTime(QTime(0, 2, 3));
////    timer->start(1000);
//
////    QMessageBox::information(this, "Hello", QString::number(duration));
//
//}

void MainWindow::endTimer()
{
    if( timer->isActive() )
        timer->stop();

    ui->actionStart->setEnabled(true);
}

void MainWindow::on_actionStart_triggered()
{
    timer->start(1000);
    ui->actionStart->setEnabled(false);
}

void MainWindow::on_actionStop_triggered()
{
    endTimer();
}

void MainWindow::on_actionReset_triggered()
{
    endTimer();
    ui->timeEdit->setTime( QTime( 0,0,0 ) );
}

void MainWindow::on_action15_minutes_activated()
{
    endTimer();
    ui->timeEdit->setTime( QTime( 0,15,0 ) );
}

void MainWindow::on_action25_minutes_triggered()
{
    endTimer();
    ui->timeEdit->setTime( QTime( 0,25,0 ) );
}

void MainWindow::on_action35_minutes_triggered()
{
    endTimer();
    ui->timeEdit->setTime( QTime( 0,35,0 ) );
}

void MainWindow::on_action45_minutes_activated()
{
    endTimer();
    ui->timeEdit->setTime( QTime( 0,45,0 ) );
}

void MainWindow::on_action1_hour_triggered()
{
    endTimer();
    ui->timeEdit->setTime( QTime( 1,0,0 ) );
}
