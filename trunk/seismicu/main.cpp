#include <QApplication>
#include "seismicu.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SeismicU dialog;
    dialog.show();
    return app.exec();
}
