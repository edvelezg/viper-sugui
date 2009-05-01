#include <QtGui>
#include <qmainwindow.h>
#include <qwhatsthis.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>

#include "canvaspicker.h"
#include "plot.h"
#include "wizard.h"

int main (int argc, char **argv)
{
    QApplication a(argc, argv);

    QMainWindow mainWindow;

    QToolBar *toolBar = new QToolBar(&mainWindow);
#if QT_VERSION >= 0x040000
    QAction *action = QWhatsThis::createAction(toolBar);

	QAction *actionSave = new  QAction(&mainWindow);
    actionSave->setIcon(QIcon(":/images/save.png"));
    actionSave->setStatusTip("Guardar una Simulacion");

    toolBar->addAction(action);
    toolBar->addAction(actionSave);
    mainWindow.addToolBar(toolBar);

#else
 	(void)QWhatsThis::whatsThisButton(toolBar);
#endif

    ClassWizard *wizard = new ClassWizard(&mainWindow);
    wizard->show();
	
	if ( wizard->exec() ) {
		
		QString numObjs = wizard->field("numObjs").toString();
				
		int num = numObjs.toInt();
		
		QVector<int> numPoints(num);
        for ( int i = 0; i < num; ++i ) {
            QString strVel = wizard->field("sbVel" + QString::number(i)).toString();
			numPoints[i] = strVel.toInt();
            // qDebug() << "velocity "  + QString::number(i) << strVel;
        }
                
	    Plot *plot = new Plot(num, numPoints, &mainWindow);
   
	    // The canvas picker handles all mouse and key
	    // events on the plot canvas
   
	    CanvasPicker *canvas = new CanvasPicker(plot);
	    a.connect(actionSave, SIGNAL(triggered()), canvas, SLOT(save()));

	    mainWindow.setCentralWidget(plot);
	#if QT_VERSION < 0x040000
	    a.setMainWidget(&mainWindow);
	#endif
	} else {
		qDebug() << "Cancelled Crap";
	}
    // The scale picker translates mouse clicks
    // int o clicked() signals


    mainWindow.resize(540, 400);
    mainWindow.show();
    int rv = a.exec();
    return rv;
}
