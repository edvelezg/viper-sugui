#include <QtGui>
#include <QtDebug>
#include "mainwindow.h"
// #include <qmainwindow.h>
// #include <qwhatsthis.h>
// #include <qtoolbar.h>
// #include <qtoolbutton.h>

#include "canvaspicker.h"
#include "plot.h"
#include "wizard.h"

MainWindow::MainWindow() {
    setupUi( this );   

    createActions()    ;
    createToolBars()   ;

	QVector<int> numPoints(1);
	numPoints[0] = 15;
    plot = new Plot(1, numPoints, this);
   
    // The canvas picker handles all mouse and key
    // events on the plot canvas
   
    canvas = new CanvasPicker(plot);
	
	setCentralWidget(plot);

	// 	// #if QT_VERSION < 0x040000
	// 	//     a.setMainWidget(&mainWindow);
	// 	// #endif
	// 	} 

	resize(540, 400);
	
    connect(canvas, SIGNAL(modified()), this, SLOT(canvasModified()));
		
    setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentFile("");
}

void MainWindow::createActions()
{
    actionNew->setIcon(QIcon(":/images/new.png"));
    actionNew->setStatusTip(tr("Crear una nuevo modelo de velocidad"));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));

    actionSave->setIcon(QIcon(":/images/save.png"));
    actionSave->setStatusTip(tr("Guardar un modelo de velocidad"));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));

    actionOpen->setIcon(QIcon(":/images/open.png"));
    actionOpen->setStatusTip(tr("Abrir un modelo de velocidad"));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));

    actionSaveAs->setStatusTip(tr("Guardar el modelo de velocidad actual bajo un nuevo "
                                  "nombre"));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

    actionAbout->setStatusTip(tr("Sobre este programa"));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&Toolbar"));
    fileToolBar->addAction(actionNew);
    fileToolBar->addAction(actionSave);
    fileToolBar->addAction(actionOpen);
}

void MainWindow::newFile()
{
    if (okToContinue()) {
	
	    ClassWizard *wizard = new ClassWizard(this);
	    wizard->show();

		if ( wizard->exec() ) {

			QString numObjs = wizard->field("numObjs").toString();

			int num = numObjs.toInt();

			QVector<int> numPoints(num);
	        for ( int i = num-1; i >= 0; --i) {
	            QString strVel = wizard->field("sbVel" + QString::number(num-1-i)).toString();
				numPoints[i] = strVel.toInt();
	            // qDebug() << "velocity "  + QString::number(i) << strVel;
	        }

		    plot = new Plot(num, numPoints, this);

		    // The canvas picker handles all mouse and key
		    // events on the plot canvas

		    canvas = new CanvasPicker(plot);
		    connect(canvas, SIGNAL(modified()), this, SLOT(canvasModified()));
			setCentralWidget(plot);
			// #if QT_VERSION < 0x040000
			//     a.setMainWidget(&mainWindow);
			// #endif
			}
			
        setCurrentFile("");
//      textEdit->clear();
    }
}

void MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Abrir modelo de velocidad"), QDir::homePath(),
														tr("Model File (*.out);;All Files (*)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("ModelMaker"),
                                     tr("El modelo ha sido modificado.\n"
                                        "Desea guardar sus cambios?"),
                                     QMessageBox::Yes | QMessageBox::No
                                     | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this,
        tr("Guardar Simulacion"), QDir::homePath(),
		tr("Model File (*.out);;All Files (*)"));
    
	if (fileName.rightRef(4) != ".out") {
		fileName += ".out";
	}
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, tr("ModelMaker"),
                             tr("No puedo escribir el archivo %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }
    setCurrentFile( fileName );
	canvas->writeFile( fileName );

    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("ModelMaker"),
                             tr("No puedo escribir el archivo %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

	QTextStream in(&file);
	
	plot = new Plot(this);

	QVector<QPair<double, double> > myPoints;

	QString item  = in.readLine().simplified();
	QStringList parts = item.split(" ");
	while (parts[1] != "-99999") {
		item  = in.readLine().simplified();
		parts = item.split(" ");
	}
	
    while (!in.atEnd()) {
		QString item  = in.readLine().simplified();
	        QStringList parts = item.split(" ");
		if (parts[1] != "-99999")
		{
			myPoints.push_back(qMakePair(parts[0].toDouble(), parts[1].toDouble()));
		} else {
			if (!in.atEnd()) {
				plot->insertCurve( Qt::blue, myPoints );
				myPoints.clear();
			}
		}
    }
	
	plot->replot();
	canvas = new CanvasPicker(plot);
	setCentralWidget(plot);	

	connect(canvas, SIGNAL(modified()), this, SLOT(canvasModified()));
	
    setCurrentFile( fileName );
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Sin Titulo");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                   .arg(tr("Modelo de Velocidad")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Sobre ModelMaker"),
                       tr("<h2>ModelMaker</h2>"
                          "<p>Copyright &copy; 2009 Eduardo Gutarra."
                          "<p>ModelMaker es una pequeña aplicación que ayuda en la "
                          "elaboración de Modelos de velocidad para utilizarlos en "
                          "simulaciones de Ondas Sismicas P. "));
}

void MainWindow::canvasModified()
{
	setWindowModified(true);
}

