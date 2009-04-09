#include <QtGui>
#include <QtDebug>
#include "mainwindow.h"
#include "geometry.h"
#include "modelparams.h"
#include "simulationmodel.h"
#include "loadmodel.h"
#include "simulparams.h"
#include "getenv.h"
#include "wizard.h"
#include "velocitymodel.h"
#include <cstdlib>

MainWindow::MainWindow() {
    setupUi( this );   

    model           	= new SimulationModel();
    dlgGeometry  	= 0;
    dlgModParams 	= 0;
	dlgLoadModel    = 0;
	dlgSimParams    = 0;
	dlgEnvironment  = 0;
	dlgList         = 0;
    createActions()    ;
    createToolBars()   ;

    readSettings();

    setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentFile( "" );

    connect(&suxwigb, SIGNAL(readyReadStandardError()),
            this, SLOT(updateOutputTextEdit()));
    connect(&suxwigb, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&suxwigb, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(processError(QProcess::ProcessError)));

}

bool MainWindow::wantsToShow() {
    return shows;
}

void MainWindow::readSettings()
{
    QSettings settings("Software Inc.", "Viper Sims");
    environment = settings.value("environment").toString();
 	
	if  (!environment.isEmpty()) {
		if (QFile::exists(environment + "/bin/ximage"	 ) &&
			QFile::exists(environment + "/bin/unif2"	 ) &&
			QFile::exists(environment + "/bin/sufdmod2"	 ) &&
			QFile::exists(environment + "/bin/suxmovie"	 )    ) {
			this->shows = true;		       
			return;
		}	
	}

	dlgEnvironment = new GetEnv(this);
	
	if (dlgEnvironment->exec()) {
		environment = dlgEnvironment->getEnvironment();
		textEdit->append( "environment: " + environment) ;
		if (QFile::exists(environment + "/bin/ximage"	 ) &&
			QFile::exists(environment + "/bin/unif2"	 ) &&
			QFile::exists(environment + "/bin/sufdmod2"	 ) &&
			QFile::exists(environment + "/bin/suxmovie"	 )    ) {
			this->shows = true;		       
		} else {
			QMessageBox::critical(this, tr("Simulacion"),
	                            tr("Los programas para generar la Simulación no fueron encontrados."));
			this->shows = false;
		}
	} else {
		this->shows = false;
	}
}

void MainWindow::writeSettings()
{
    QSettings settings("Software Inc.", "Viper Sims");
    settings.setValue("environment", environment);
	qDebug() << settings.fileName();
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

    actionPreview->setStatusTip(tr("Vista Previa del Modelo de Velocidad"));
    connect(actionPreview, SIGNAL(triggered()), this, SLOT(preview()));

    actionSimParams->setStatusTip(tr("Cambiar Parametros de Simulacion"));
    connect(actionSimParams, SIGNAL(triggered()), this, SLOT(simParams()));

    actionViewTraces->setStatusTip( tr("Ver Registro" ) );
    connect(actionViewTraces, SIGNAL(triggered()), this, SLOT(viewTraces()));

    connect(actionSizeSettings, SIGNAL(triggered()),
            this, SLOT(sizeSettings()));
    connect(actionModelParams, SIGNAL(triggered()),
            this, SLOT(modelParams()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
     	writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&Archivo"));
    fileToolBar->addAction(actionNew);
    fileToolBar->addAction(actionSave);
    fileToolBar->addAction(actionOpen);
}

void MainWindow::simParams()
{
    textEdit->clear();
    if (!dlgSimParams) {
        dlgSimParams = new SimulParams ( this );
    } else {
        dlgSimParams->show();
    }

	dlgSimParams->getParams(*model);

    if (dlgSimParams->exec()) {

        setWindowModified(true);
        model->setDistance    ( (dlgSimParams->sbDistance)->text()     );
        model->setDepth       ( (dlgSimParams->sbDepth)->text()        );
        model->setFpeak       ( (dlgSimParams->sbFpeak)->text()        );
        model->setFmax        ( (dlgSimParams->sbFmax)->text()         );
        model->setWindowTitle ( (dlgSimParams->leWindowTitle)->text()  );
        model->setTitle       ( (dlgSimParams->leTitle)->text()        );
        model->setTMax        ( (dlgSimParams->sbTMax)->text()         );

        if((dlgSimParams->chkLoop)->isChecked()) {
            model->setLoop("1");        
        } else {
            model->setLoop("0");        
        }

        model->setClip        ( (dlgSimParams->sbClip)->text()         ) ;

        textEdit->append( "distance   :" + model->getDistance    () ) ;
        textEdit->append( "depth      :" + model->getDepth       () ) ;
        textEdit->append( "fpeak      :" + model->getFpeak       () ) ;
        textEdit->append( "fmax       :" + model->getFmax        () ) ;
        textEdit->append( "windowTitle:" + model->getWindowTitle () ) ;
        textEdit->append( "title      :" + model->getTitle       () ) ;
        textEdit->append( "tMax       :" + model->getTMax        () ) ;
        textEdit->append( "clip       :" + model->getClip        () ) ;

    }
    tabWidget->setCurrentIndex(0);
}

void MainWindow::sizeSettings()
{
    textEdit->clear();
    if (!dlgGeometry) {
        dlgGeometry = new Geometry ( this );
    } else {
        dlgGeometry->show();
    }

	dlgGeometry->getParams(*model);

    if (dlgGeometry->exec()) {
	    setWindowModified(true);

		model->setWidth	 	( (dlgGeometry->sbWidth)->text()      	);
		model->setHeight	 	( (dlgGeometry->sbHeight)->text()     	);
		model->setWidth_2	 	( (dlgGeometry->sbWidth_2)->text()      );
		model->setHeight_2	 	( (dlgGeometry->sbHeight_2)->text()    	);
		model->setWidthOff1 	( (dlgGeometry->sbWidthOff1)->text()  	);
		model->setHeightOff1	( (dlgGeometry->sbHeightOff1)->text()  	);
		model->setWidthOff2 	( (dlgGeometry->sbWidthOff2)->text() 	);
		model->setHeightOff2	( (dlgGeometry->sbHeightOff2)->text() 	);
	                                                              	
		textEdit->append	( "Model width: "      + model->getWidth	 	()	);
        textEdit->append	( "Model height: "     + model->getHeight	()	); 
		textEdit->append	( "Movie width: "      + model->getWidth	 	()	);
        textEdit->append	( "Movie height: "     + model->getHeight	()	); 
        textEdit->append	( "Cord. X Modelo: " + model->getWidthOff2 ()	); 
        textEdit->append	( "Cord. Y Modelo: " + model->getHeightOff2()	); 
        textEdit->append	( "Cord. X Pelicula: "  + model->getWidthOff1 ()	); 
        textEdit->append	( "Cord. Y Pelicula: "  + model->getHeightOff1()	); 
    }

    tabWidget->setCurrentIndex(0);
}

void MainWindow::modelParams()
{
    textEdit_2->clear();
	
    if (!dlgModParams) {
        dlgModParams = new ModelParams( this );
    } else {
        dlgModParams->show();
    }
	
	dlgModParams->getParams(*model);
	
    if (dlgModParams->exec()) {
        setWindowModified(true);
		
		double distance  = (dlgModParams->sbDistance)->text().toDouble();
		double depth = (dlgModParams->sbDepth)->text().toDouble();
		
		QString d1 = QString::number((depth/100.0));
		QString d2 = QString::number((distance/100.0));
				
        model->setN1("100");        
        model->setN2("100");        
        model->setD1(d1);        
        model->setD2(d2);        
		
		if ((dlgModParams->cbColor)->currentText() == "Color") {
        	model->setCmap("hue");        
		}	else {
			model->setCmap("gray");
		}		

        if((dlgModParams->chkLeyenda)->isChecked()) {
            model->setLegend("1");        
        } else {
            model->setLegend("0");        
        }

		model->setMethod((dlgModParams->cbMethod)->currentText());

        model->setTitulo((dlgModParams->leTitulo)->text());        

        textEdit_2->append( "n1:"      +   model->getN1() 		);
        textEdit_2->append( "n2:"      +   model->getN2() 		);
        textEdit_2->append( "d1:"      +   model->getD1() 		);
        textEdit_2->append( "d2:"      +   model->getD2() 		);
        textEdit_2->append( "cmap:"    +   model->getCmap()   	);
        textEdit_2->append( "legend:"  +   model->getLegend()   	);
        textEdit_2->append( "titulo:"  +   model->getTitulo()   	);

        tabWidget->setCurrentIndex(1);
     }
}

void MainWindow::loadModel()
{
    textEdit_2->clear();
    if (!dlgList) {
        dlgList = new ListDialog( this );
    } else {
        dlgList->show();
    }

	dlgList->setVelocities(model->getVelocities());

      if( dlgList->exec() ) {
          setWindowModified(true);
          model->setModelFile(dlgList->currentLocation());
          textEdit_2->append("Archivo de Modelo: " + model->getModelFile());

          QString all = dlgList->velocities().join(",");
		  if (all != "")
          	model->setVelocities( "v00=" + all );

          qDebug() << model->getVelocities();

          tabWidget->setCurrentIndex(1);
      }
}

void MainWindow::viewTraces()
{
	textEdit_2->clear();
	// Setting the Environment for Seismic Unix
	QStringList env = QProcess::systemEnvironment();
 	env << "CWPROOT=" + environment;
 	suxwigb.setEnvironment(env);
	
	QStringList args;
	
	args	<< "clip=1.0"	 	
    		;
	
	suxwigb.setStandardInputFile("hseis.out");
	suxwigb.setWorkingDirectory( QDir::current().currentPath() );
	suxwigb.start("suxwigb", args);	
}

void MainWindow::run()
{
	textEdit->clear();
	// Setting the Environment for Seismic Unix
	QString sysPath = ::getenv("PATH");
	sysPath = sysPath + ":" + environment + "/bin";
//  ::setenv("PATH", sysPath.toStdString().c_str(), 1);
	QStringList env = QProcess::systemEnvironment();
	env << "CWPROOT=" + environment;
 	// env = "PATH=" + environment;
 	// env << "PATH=$PATH:" + environment + "/bin";
	// env.replaceInStrings(QRegExp("^PATH=(.*)", Qt::CaseInsensitive), "PATH=/1:/bin:" + environment + "/bin");
 	ximage.setEnvironment(env);
	unif2.setEnvironment(env);
 	sufdmod2.setEnvironment(env);
	suxmovie.setEnvironment(env);
	suxwigb.setEnvironment(env);
	
	QStringList showme = ximage.environment();

	for ( QStringList::Iterator it = showme.begin(); it != showme.end(); ++it ) {
		qDebug() << *it;
	}
	
    QStringList args;
    if (model->getModelFile() == "") {
        args << "tfile=model.out";
        model->setModelFile("model.out");
		unif2.start("unif2", args);
		unif2.waitForFinished();
		args.clear();
    }

	// Process unif2.
	args 	<< "n1=" 		+ model->getN1()
	       	<< "n2=" 		+ model->getN2()
	     	<< model->getVelocities()
			<< "method=" 	+ model->getMethod()
	        ;
	
	textEdit_2->append("Archivo de Modelo: " + model->getModelFile());
	textEdit_2->append("Velocidades del Modelo: " + model->getVelocities());
    
	// print it out
    
	unif2.setStandardInputFile(model->getModelFile());
	unif2.setStandardOutputFile("vel.out");
	unif2.setWorkingDirectory( QDir::current().currentPath() );
	unif2.start("unif2", args);
	unif2.waitForStarted();
	// unif2.waitForFinished();
	
	args.clear();
	
    // Process ximage.
	args	<< "n1="	 	+ model->getN1() 
         	<< "n2=" 		+ model->getN2()
         	<< "d1=" 		+ model->getD1()
         	<< "d2=" 		+ model->getD2()
			<< "legend=" 	+ model->getLegend()
			<< "cmap=" 		+ model->getCmap()
		    << "title=" 	+ model->getTitulo()
    	    << "method=" 	+ model->getMethod()
    	    << "wbox=" 		+ model->getWidth()
    	    << "hbox=" 		+ model->getHeight()
    	    << "xbox=" 		+ model->getWidthOff2()
    	    << "ybox=" 		+ model->getHeightOff2()
			;
			
	for ( QStringList::Iterator it = args.begin(); it != args.end(); ++it ) {
        textEdit->append(*it); 
	}

	ximage.setStandardInputFile("vel.out");
	ximage.setWorkingDirectory( QDir::current().currentPath() );
	ximage.start("ximage", args);	
	ximage.waitForStarted();
	
	args.clear();
	
	// Process sufdmod2. 
	
    QStringList argsSufdmod2;
	
	args	<< "nz="	 	                    + model->getN1() 
         	<< "nx=" 		                    + model->getN2()
         	<< "dz=" 		                    + model->getD1()
         	<< "dx=" 		                    + model->getD2()
			<< "fpeak=" 	                    + model->getFpeak()
			<< "fmax=" 	                        + model->getFmax()
		    << "xs=" 	                        + model->getDistance()
    	    << "zs=" 		                    + model->getDepth()
    	    << "hsz=60" 	
    	    << "vsx=250" 	
    	    << "hsfile=hseis.out" 	
    	    << "vsfile=vseis.out" 	
    	    << "ssfile=sseis.out" 	
    	    << "tmax="                          + model->getTMax() 	
    	    << "abs=1,1,1,1"
    	    << "mt=5" 		
    	    << "verbose=2"  
			;
	
	sufdmod2.setStandardInputFile("vel.out");
	sufdmod2.setWorkingDirectory( QDir::current().currentPath() );
	sufdmod2.setStandardOutputProcess(&suxmovie);
	// sufdmod2.waitForFinished();


	QStringList argsMovie;
	// Process suxmovie. 

	argsMovie	<< "clip=1.0"	 	
				<< "title='"                                    + model->getTitle()        + "'"
				<< "windowtitle='" 	                            + model->getWindowTitle()  + "'"
				<< "label1='Profundidad (m)'" 	
				<< "label2='Distancia (m)'" 	
				<< "n1="	 	                                + model->getN1() 
				<< "n2=" 		                                + model->getN2()
	         	<< "d1=" 		                                + model->getD1()
	         	<< "d2=" 		                                + model->getD2()
			    << "f1=0.0" 	                                
			    << "f2=0.0" 	                                
	    	    << "loop=" 	                                    + model->getLoop()
	    	    << "cmap=gray" 	
	    	    << "-geometry"
				<< model->getWidth_2()	+ "x" + model->getHeight_2() + "+" + model->getWidthOff1() + "+" + model->getHeightOff1()
				;

	suxmovie.setWorkingDirectory( QDir::current().currentPath() );

	sufdmod2.start("sufdmod2", args);	
	suxmovie.start("suxmovie", argsMovie);	
}

void MainWindow::preview()
{
	textEdit->clear();
	// Setting the Environment for Seismic Unix
	QStringList env = QProcess::systemEnvironment();
 	env << "CWPROOT=/opt/SU";
 	env << "PATH=$PATH:/opt/SU/bin";
 	ximage.setEnvironment(env);
	unif2.setEnvironment(env);
 	sufdmod2.setEnvironment(env);
	suxmovie.setEnvironment(env);


    QStringList args;
    if (model->getModelFile() == "") {
        args << "tfile=model.out";
        model->setModelFile("model.out");
		unif2.start("unif2", args);
		unif2.waitForFinished();
		args.clear();
    }

	// Process unif2.
	args 	<< "n1=" 		+ model->getN1()
	       	<< "n2=" 		+ model->getN2()
			<< "method=" 	+ model->getMethod()
	        ;
	
	// print it out
	for ( QStringList::Iterator it = args.begin(); it != args.end(); ++it ) {
		textEdit->append(*it); 
	}

    
	unif2.setStandardInputFile(model->getModelFile());
	unif2.setStandardOutputFile("vel.out");
	unif2.setWorkingDirectory( QDir::current().currentPath() );
	unif2.start("unif2", args);
	// unif2.waitForFinished();
	
	args.clear();
	
    // Process ximage.
	args	<< "n1="	 	+ model->getN1() 
         	<< "n2=" 		+ model->getN2()
         	<< "d1=" 		+ model->getD1()
         	<< "d2=" 		+ model->getD2()
			<< "legend=" 	+ model->getLegend()
			<< "cmap=" 		+ model->getCmap()
		    << "title=" 	+ model->getTitulo()
    	    << "method=" 	+ model->getMethod()
    	    << "wbox=" 		+ model->getWidth()
    	    << "hbox=" 		+ model->getHeight()
    	    << "xbox=" 		+ model->getWidthOff2()
    	    << "ybox=" 		+ model->getHeightOff2()
			;
			
	for ( QStringList::Iterator it = args.begin(); it != args.end(); ++it ) {
        textEdit->append(*it); 
	}

	ximage.setStandardInputFile("vel.out");
	ximage.setWorkingDirectory( QDir::current().currentPath() );
	ximage.start("ximage", args);	
}

void MainWindow::newFile()
{
    if (okToContinue()) {
        model->clear();
        setCurrentFile("");
        textEdit->clear();
    }
}

void MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Simulation"), "",
                                   tr("Viper Simulation File(*.vsf)"));
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
        tr("Guardar Simulacion"), "",
		tr("Viper Simulation File (*.vsf);;"));
    
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
   model->writeFile( fileName );

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
   model->readFile( fileName );

   return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
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
               "elaboracion de simulaciones de propagacion de Ondas Sismicas P"
               "utilizando un modelo de velocidad. "));
}

void MainWindow::processFinished(int exitCode,
                                    QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        textEdit->append(tr("Conversion program crashed"));
    } else if (exitCode != 0) {
        textEdit->append(tr("Suxwigb failed"));
	    textEdit->append(suxwigb.errorString());
    } else {
        textEdit->append(tr("File %1 created").arg(curFile));
    }
}

void MainWindow::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        textEdit->append(tr("Conversion program not found"));
    }
}

void MainWindow::updateOutputTextEdit()
{
    QByteArray newData = suxwigb.readAllStandardError();
    QString text = textEdit->toPlainText()
                   + QString::fromLocal8Bit(newData);
    textEdit->append(text);
}

