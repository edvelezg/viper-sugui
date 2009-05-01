#include <QApplication>
#include "mainwindow.h"

int main (int argc, char *argv[])
{
    QApplication app( argc,  argv );
    MainWindow main;
    if( main.wantsToShow() ) {
        main.show();
    } else {
        return 0;
    }
    return app.exec();
}
