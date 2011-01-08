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

    ui->timeEdit->setTime(QTime(0, 1, 2));
    QMessageBox::information(this, "Hello", QDir::currentPath());
    ui->btnStart->setEnabled(false);
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

    ui->btnStart->setEnabled(true);
}

void MainWindow::on_btnStart_clicked()
{
    timer->start(1000);
    ui->btnStart->setEnabled(false);
}

void MainWindow::on_btnStop_clicked()
{
    endTimer();
}

void MainWindow::on_btnReset_clicked()
{
    endTimer();
    ui->timeEdit->setTime( QTime( 0,0,0 ) );
}
