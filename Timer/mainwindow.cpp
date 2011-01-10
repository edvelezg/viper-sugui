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
//    ui->actionStart->setEnabled(false);
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
        QSound::play("sounds/alarm-clock1.wav");
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
    Qt::WindowFlags flags = this->windowFlags();
    flags ^= Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::FramelessWindowHint;
    if (checked)
    {
        this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint );
        this->show();
    }
    else
    {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }
}

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
