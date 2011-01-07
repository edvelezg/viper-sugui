#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <qDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(update()));
     timer->start(1000);
     setWindowTitle(tr("Analog Clock"));
     resize(200, 200);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    ui->timeEdit->setTime(QTime(0, 1, 2));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update(){
    QTime newTime( ui->timeEdit->time() );
      newTime = newTime.addSecs( -1 ); // doh
      ui->timeEdit->setTime( newTime );
//      QMessageBox::information(this, "Hello", newTime.toString());
}

void MainWindow::on_pushButton_clicked()
{
    timer->stop();
//    QString timeText = ui->timeEdit->text();
//    QStringList list = timeText.split(":");
//    int hour = list[0].toInt();
//    int min = list[1].toInt();
//    int sec = list[2].toInt();


//    QStringList::const_iterator constIterator;
//     for (constIterator = list.constBegin(); constIterator != list.constEnd();
//            ++constIterator)
//         qDebug() << (*constIterator).toLocal8Bit().constData() << " ";

//    int duration = 3600*hour + 60*min + sec;



//    MainWindow::msleep(1000);
//    ui->timeEdit->setTime(QTime(0, 1, 2));
//    MainWindow::msleep(1000);
//    ui->timeEdit->setTime(QTime(0, 2, 3));
//    MainWindow::msleep(1000);
//    ui->timeEdit->setTime(QTime(0, 1, 2));
//    MainWindow::msleep(1000);
//    ui->timeEdit->setTime(QTime(0, 2, 3));
//    timer->start(1000);

//    QMessageBox::information(this, "Hello", QString::number(duration));

}