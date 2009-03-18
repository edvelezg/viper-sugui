#include <QApplication>
#include "mainwindow.h"
#include <QtDebug>

int main (int argc, char *argv[])
{
    QApplication app( argc,  argv );
    MainWindow main;
    main.show();
	qDebug() << "Think of the lonely doh";
    return app.exec();
}
