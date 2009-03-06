#include <QtGui>
#include "mainwindow.h"
#include "geometry.h"
#include "modelparams.h"
#include "velocitymodel.h"

MainWindow::MainWindow() {
    setupUi( this );   

    vm = new VelocityModel();
    dlgGeometry  = 0;
    dlgModParams = 0;
    createActions();
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
        QString width      = (dlgGeometry->sbWidth)->text();
        QString height     = (dlgGeometry->sbHeight)->text();
        QString widthoff1  = (dlgGeometry->sbWidthOff1)->text();
        QString widthoff2  = (dlgGeometry->sbWidthOff2)->text();
        QString heightoff1 = (dlgGeometry->sbHeightOff1)->text();
        QString heightoff2 = (dlgGeometry->sbHeightOff2)->text();
        textEdit->append( "width: "      + width );
        textEdit->append( "height: "     + height );
        textEdit->append( "widthoff1: "  + widthoff1 );
        textEdit->append( "widthoff1: "  + widthoff1 );
        textEdit->append( "heightoff1: " + heightoff1 );
        textEdit->append( "heightoff2: " + heightoff2 );
    }

}

void MainWindow::run()
{
	// Setting the Environment for Seismic Unix
	QStringList env = QProcess::systemEnvironment();
 	env << "CWPROOT=/opt/SU";
 	env << "PATH=$PATH:/opt/SU/bin";
 	ximage.setEnvironment(env);
	unif2.setEnvironment(env);
  	
    QStringList args;
	args 	<< "n1=" + vm->getN1()
	       	<< "n2=" + vm->getN2()
	        << "method=spline"
	        ;
	
	// Process ximage.
	unif2.setStandardInputFile("model.out");
	unif2.setStandardOutputFile("vel.out");
	unif2.setWorkingDirectory( QDir::current().currentPath() );
	unif2.start("unif2", args);
	unif2.waitForFinished();
	
	args.clear();
    
	args	<< "n1=" + vm->getN1() 
         	<< "n2=" + vm->getN2()
         	<< "d1=" + vm->getD1()
         	<< "d2=" + vm->getD2()
         	<< "legend=1"
        	;
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

    if (dlgModParams->exec()) {
        setWindowModified(true);
        QString vel1       = (dlgModParams->sbVel1)->text();
        QString vel2       = (dlgModParams->sbVel2)->text();
        QString vel3       = (dlgModParams->sbVel3)->text();
                           
        vm->setN1((dlgModParams->sbN1)->text());        
        vm->setN2((dlgModParams->sbN2)->text());        
        vm->setD1((dlgModParams->sbD1)->text());        
        vm->setD2((dlgModParams->sbD2)->text());        

        vm->setCmap((dlgModParams->cbColor)->currentText());        

        if((dlgModParams->chkLeyenda)->isChecked()) {
            vm->setLegend("1");        
        } else {
            vm->setLegend("0");        
        }

        vm->setTitulo((dlgModParams->leTitulo)->text());        

//      textEdit->append( "vel1:"    +   vm->getN1()       );
//      textEdit->append( "vel2:"    +   vm->getN2()       );
//      textEdit->append( "vel3:"    +   vm->getD1()       );
        textEdit->append( "n1:"      +   vm->getN1() );
        textEdit->append( "n2:"      +   vm->getN2() );
        textEdit->append( "d1:"      +   vm->getD1() );
        textEdit->append( "d2:"      +   vm->getD2() );
        textEdit->append( "cmap:"    +   vm->getCmap()   );
        textEdit->append( "legend:"  +   vm->getLegend()   );
        textEdit->append( "titulo:"  +   vm->getTitulo()   );

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

wvoid MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Simulation"), ".",
                                   tr("Simulation Viper(*.svp)"));
//      if (!fileName.isEmpty())
//          loadFile(fileName);
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
               "<p>Copyright &copy; 2009 EAFIT."
               "<p>Viper es una pequenha aplicacion que ayuda en la "
               "elaboracion de simulacion de propagacionde ondas sismicas"
               "utilizando un modelo de velocidad. "));
}
