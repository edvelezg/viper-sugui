#include <QApplication>
#include "mainwindow.h"
#include <QtDebug>

int main (int argc, char *argv[])
{
    QApplication app( argc,  argv );
    MainWindow main;
    if( main.wantsToShow() ) {
        main.show();
        qDebug() << "Program starts!";
    } else {
        return 0;
    }
    return app.exec();
}
