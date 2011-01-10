#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QSound>
#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow)
{
    readSettings();
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    soundfile = new QSound("sounds/alarm-clock1.wav", this);
//    ui->actionStart->setEnabled(false);
    alwaysOnTop(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSettings()
{
    QSettings settings("Software Inc", "Timer");
    geometry = settings.value("geometry").toString();

    if  (!geometry.isEmpty()) {
        restoreGeometry(settings.value("geometry").toByteArray());
    }
}

void MainWindow::update(){
    QTime newTime( ui->timeEdit->time() );
    if (newTime == QTime(0, 0, 0))
    {
        soundfile->play();
        endTimer();
    }
    else
    {
        newTime = newTime.addSecs( -1 ); // doh
        ui->timeEdit->setTime( newTime );
    }
}

void MainWindow::alwaysOnTop(bool checked)
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
    if( !soundfile->isFinished() )
        soundfile->stop();

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
}

void MainWindow::writeSettings()
{
    QSettings settings("Software Inc", "Timer");
    settings.setValue("geometry", saveGeometry());
    qDebug() << settings.fileName();
}
