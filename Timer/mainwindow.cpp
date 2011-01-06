#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <qDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString timeText = ui->timeEdit->text();
    QStringList list = timeText.split(":");
    int hour = list[0].toInt();
    int min = list[1].toInt();
    int sec = list[2].toInt();

//    QStringList::const_iterator constIterator;
//     for (constIterator = list.constBegin(); constIterator != list.constEnd();
//            ++constIterator)
//         qDebug() << (*constIterator).toLocal8Bit().constData() << " ";

    int duration = 3600*hour + 60*min + sec;

    MainWindow::msleep(1000);
    ui->timeEdit->setTime(QTime(0, 1, 2));
    MainWindow::msleep(1000);
    ui->timeEdit->setTime(QTime(0, 2, 3));
    MainWindow::msleep(1000);
    ui->timeEdit->setTime(QTime(0, 1, 2));
    MainWindow::msleep(1000);
    ui->timeEdit->setTime(QTime(0, 2, 3));
    QMessageBox::information(this, "Hello", QString::number(duration));

}
