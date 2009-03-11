#include <QtGui>
#include "mainwindow.h"
#include "geometry.h"
#include "modelparams.h"
#include "velocitymodel.h"
#include "loadmodel.h"

MainWindow::MainWindow() {
    setupUi( this );   

    vm = new VelocityModel();
    dlgGeometry  = 0;
    dlgModParams = 0;
	dlgLoadModel = 0;
    createActions() ;
    createToolBars();

    setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentFile( "" );
}

void MainWindow::createActions()
{
    actionNew->setIcon(QIcon(":/images/new.png"));
    actionNew->setStatusTip(tr("Crear una nueva Simulacion"));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));

    actionSave->setIcon(QIcon(":/images/save.png"));
    actionSave->setStatusTip(tr("Guardar una Simulacion"));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));

    actionOpen->setIcon(QIcon(":/images/open.png"));
    actionOpen->setStatusTip(tr("Open an existing spreadsheet file"));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));

    actionSaveAs->setStatusTip(tr("Guardar la simulacion actual bajo un nuevo "
                                  "nombre"));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

    actionAbout->setStatusTip(tr("Sobre este programa"));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    actionRun->setStatusTip(tr("Correr una Simulacion"));
    connect(actionRun, SIGNAL(triggered()), this, SLOT(run()));

    actionLoadModel->setStatusTip(tr("Carcar Modelo de Velocidad"));
    connect(actionLoadModel, SIGNAL(triggered()), this, SLOT(loadModel()));

    connect(actionSizeSettings, SIGNAL(triggered()),
            this, SLOT(sizeSettings()));
    connect(actionModelParams, SIGNAL(triggered()),
            this, SLOT(modelParams()));
}


void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&Archivo"));
    fileToolBar->addAction(actionNew);
    fileToolBar->addAction(actionSave);
    fileToolBar->addAction(actionOpen);
}

void MainWindow::sizeSettings()
{
    textEdit->clear();
    if (!dlgGeometry) {
        dlgGeometry = new Geometry ( this );
    } else {
        dlgGeometry->show();
    }

    if (dlgGeometry->exec()) {
	    setWindowModified(true);

		vm->setWidth	 	( (dlgGeometry->sbWidth)->text()      	);
		vm->setHeight	 	( (dlgGeometry->sbHeight)->text()     	);
		vm->setWidthOff1 	( (dlgGeometry->sbWidthOff1)->text()  	);
		vm->setHeightOff1	( (dlgGeometry->sbHeightOff1)->text()  	);
		vm->setWidthOff2 	( (dlgGeometry->sbWidthOff2)->text() 	);
		vm->setHeightOff2	( (dlgGeometry->sbHeightOff2)->text() 	);
	                                                              	
		textEdit->append	( "width: "      + vm->getWidth	 	()	);
        textEdit->append	( "height: "     + vm->getHeight	()	); 
        textEdit->append	( "widthoff1: "  + vm->getWidthOff1 ()	); 
        textEdit->append	( "widthoff1: "  + vm->getHeightOff1()	); 
        textEdit->append	( "heightoff1: " + vm->getWidthOff2 ()	); 
        textEdit->append	( "heightoff2: " + vm->getHeightOff2()	); 
    }

    tabWidget->setCurrentIndex(1);
}

void MainWindow::loadModel()
{
    // textEdit->clear();
    if (!dlgLoadModel) {
        dlgLoadModel = new LoadModel( this );
    } else {
        dlgLoadModel->show();
    }

    if (dlgLoadModel->exec()) {
		textEdit->append("source file:" + dlgLoadModel->getModelFile());
		vm->setModelFile(dlgLoadModel->getModelFile());
    }

}

void MainWindow::run()
{
	textEdit->clear();
	// Setting the Environment for Seismic Unix
	QStringList env = QProcess::systemEnvironment();
 	env << "CWPROOT=/opt/SU";
 	env << "PATH=$PATH:/opt/SU/bin";
 	ximage.setEnvironment(env);
	unif2.setEnvironment(env);

	// Process unif2.
    QStringList args;
	args 	<< "n1=" 		+ vm->getN1()
	       	<< "n2=" 		+ vm->getN2()
			<< "method=" 	+ vm->getMethod()
	        ;
	
	// print it out
	for ( QStringList::Iterator it = args.begin(); it != args.end(); ++it ) {
		textEdit->append(*it); 
	}

	unif2.setStandardInputFile(vm->getModelFile());
	unif2.setStandardOutputFile("vel.out");
	unif2.setWorkingDirectory( QDir::current().currentPath() );
	unif2.start("unif2", args);
	unif2.waitForFinished();
	
	args.clear();
	
    // Process ximage.
	args	<< "n1="	 	+ vm->getN1() 
         	<< "n2=" 		+ vm->getN2()
         	<< "d1=" 		+ vm->getD1()
         	<< "d2=" 		+ vm->getD2()
			<< "legend=" 	+ vm->getLegend()
			<< "cmap=" 		+ vm->getCmap()
		    << "label=" 	+ vm->getTitulo()
    	    << "method=" 	+ vm->getMethod()
    	    << "wbox=" 		+ vm->getWidth()
    	    << "hbox=" 		+ vm->getHeight()
    	    << "xbox=" 		+ vm->getWidthOff2()
    	    << "ybox=" 		+ vm->getHeightOff2()
			;
			
	for ( QStringList::Iterator it = args.begin(); it != args.end(); ++it ) {
        textEdit->append(*it); 
	}

	ximage.setStandardInputFile("vel.out");
	ximage.setWorkingDirectory( QDir::current().currentPath() );
	ximage.start("ximage", args);	
}

void MainWindow::modelParams()
{
    textEdit->clear();
	
    if (!dlgModParams) {
        dlgModParams = new ModelParams( this );
    } else {
        dlgModParams->show();
    }
	
	dlgModParams->getParams(*vm);
	
    if (dlgModParams->exec()) {
        setWindowModified(true);
		
		double distance  = (dlgModParams->sbDistance)->text().toDouble();
		double depth = (dlgModParams->sbDepth)->text().toDouble();
		
		QString d1 = QString::number((depth/100.0));
		QString d2 = QString::number((distance/100.0));
				
        vm->setN1("100");        
        vm->setN2("100");        
        vm->setD1(d1);        
        vm->setD2(d2);        
		
		if ((dlgModParams->cbColor)->currentText() == "Color") {
        	vm->setCmap("hue");        
		}	else {
			vm->setCmap("gray");
		}		

        if((dlgModParams->chkLeyenda)->isChecked()) {
            vm->setLegend("1");        
        } else {
            vm->setLegend("0");        
        }

		vm->setMethod((dlgModParams->cbMethod)->currentText());

        vm->setTitulo((dlgModParams->leTitulo)->text());        

        textEdit->append( "n1:"      +   vm->getN1() 		);
        textEdit->append( "n2:"      +   vm->getN2() 		);
        textEdit->append( "d1:"      +   vm->getD1() 		);
        textEdit->append( "d2:"      +   vm->getD2() 		);
        textEdit->append( "cmap:"    +   vm->getCmap()   	);
        textEdit->append( "legend:"  +   vm->getLegend()   	);
        textEdit->append( "titulo:"  +   vm->getTitulo()   	);

        tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::newFile()
{
    if (okToContinue()) {
        vm->clear();
        setCurrentFile("");
        textEdit->clear();
    }
}

void MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Simulation"), ".",
                                   tr("Simulation Viper(*.svp)"));
     if (!fileName.isEmpty())
         loadFile(fileName);
    }
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Viper Simulation"),
                        tr("Los parametros han sido modificados.\n"
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
                               tr("Guardar Simulacion"), ".",
                               tr("Simulaciones Viper (*.svp)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
   QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly)) {
       QMessageBox::warning(this, tr("Simulacion"),
                            tr("No puedo escribir el archivo %1:\n%2.")
                            .arg(file.fileName())
                            .arg(file.errorString()));
       return false;
   }
   setCurrentFile( fileName );
   vm->writeFile( fileName );

   return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly)) {
       QMessageBox::warning(this, tr("Simulacion"),
                            tr("No puedo escribir el archivo %1:\n%2.")
                            .arg(file.fileName())
                            .arg(file.errorString()));
       return false;
   }
   setCurrentFile( fileName );
   vm->readFile( fileName );

   return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
//      recentFiles.removeAll(curFile);
//      recentFiles.prepend(curFile);
//      updateRecentFileActions();
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("Simulacion")));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Sobre Viper SUGUI"),
            tr("<h2>Viper SuGui 1.1</h2>"
               "<p>Copyright &copy; 2009 Eduardo Gutarra."
               "<p>Viper es una pequeña aplicacion que ayuda en la "
               "elaboración de simulaciones de propagación de ondas sismicas"
               "utilizando un modelo de velocidad. "));
}
