#include <QtGui>
#include <iostream>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    setupUi( this );

    createActions();
    createToolBars();

    QPushButton *simulateButton =
            propagationModelButtonBox->button(QDialogButtonBox::Ok);
    simulateButton->setText(tr("&Correr"));

    QPushButton *ximageButton =
            velocityModelButtonBox->button(QDialogButtonBox::Ok);
    ximageButton->setText(tr("&Ver Modelo"));

    connect(simulateButton, SIGNAL(clicked()),
            this, SLOT(runSimulation()));

    connect(ximageButton, SIGNAL(clicked()),
            this, SLOT(runXimage()));

    connect(actionPropagationModel, SIGNAL( triggered() ),  this,  SLOT( definePropagationModel() ));

    connect(actionVelocityModel, SIGNAL( triggered() ),  this,  SLOT( defineVelocityModel() ));
    
    simulateButton->setEnabled(false);
}

void MainWindow::createActions()
{
    actionNew->setIcon(QIcon(":/images/new.png"));
    actionNew->setShortcut(QKeySequence::New);
    actionNew->setStatusTip(tr("Crear una nueva Simulacion"));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));

    actionSave->setIcon(QIcon(":/images/save.png"));
    actionSave->setShortcut(QKeySequence::Save);
    actionSave->setStatusTip(tr("Guardar una Simulacion"));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&Archivo"));
    fileToolBar->addAction(actionNew);
    fileToolBar->addAction(actionSave);
}

void MainWindow::on_browseButton_clicked()
{
    QString initialName = sourceFileEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Choose File"),
                                         initialName);
    fileName = QDir::toNativeSeparators(fileName);
    if (!fileName.isEmpty()) {
        sourceFileEdit->setText(fileName);
        propagationModelButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        sourceFile = sourceFileEdit->text();
    }
}


void MainWindow::newFile()
{
    setCurrentFile( "" );
}


void MainWindow::defineVelocityModel()
{
    stackedWidget->setCurrentIndex(1);
}


void MainWindow::definePropagationModel()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::runSimulation()
{
	propagationModelTextEdit->clear();

    QFile file("script.sh");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

        // Stuff to write to the script
    QTextStream out(&file);
    out << "# /bin/sh                                                                               " << endl;
    out << "WIDTH=450                                                                               " << endl;
    out << "HEIGHT=450                                                                              " << endl;
    out << "WIDTHOFF1=10                                                                            " << endl;
    out << "WIDTHOFF2=500                                                                           " << endl;
    out << "HEIGHTOFF1=50                                                                           " << endl;
    out << "HEIGHTOFF2=530                                                                          " << endl;
    out << "                                                                                        " << endl;
    out << "n1=100 d1=5 f1=0.0 label1=\"Profundidad (km)\"                                                " << endl;
    out << "n2=100 d2=5 f2=0.0 label2=\"Distancia (km)\"                                             " << endl;

        out << qPrintable(QString("xs=%1 zs=%2 hsz=50 vsx=250 verbose=2 ")
                                                                                                                .arg(xsSpinBox->text()).arg(zsSpinBox->text())) << endl;

    out << "vsfile=\"vseis.out\" ssfile=\"sseis.out\" hsfile=\"hseis.out\"                          " << endl;
    out << "tmax=.4 mt=5 fpeak=35 fmax=40                                                           " << endl;
    out << "                                                                                        " << endl;
    out << "makevel nx=$n2 nz=$n1 v000=3000 >vel.out                                                " << endl;
    out << qPrintable(QString("unif2 < %1 n1=$n1 n2=$n2 method=spline > vel.out  ")
                                                                                                                .arg(sourceFile)) << endl;
    out << "unif2 < model.out n1=$n1 n2=$n2 method=spline > vel.out                                                                     " << endl;
    out << "                                                                                        " << endl;
    out << "ximage < vel.out wbox=$WIDTH hbox=$HEIGHT xbox=$WIDTHOFF2 title=\"Wavespeed Profile\" \\" << endl;
    out << " n1=$n1 n2=$n2 legend=1 lheight=150 units=\"wave speed\" &                                                          " << endl;
    out << "                                                                                        " << endl;
    out << "sufdmod2 <vel.out nz=$n1 dz=$d1 nx=$n2 dx=$d2 verbose=1                               \\" << endl;
    out << "    fpeak=$fpeak fmax=$fmax                                                           \\" << endl;
    out << "    xs=$xs zs=$zs hsz=$hsz vsx=$vsx hsfile=$hsfile                                    \\" << endl;
    out << "    vsfile=$vsfile ssfile=$ssfile verbose=$verbose                                    \\" << endl;
    out << "    tmax=$tmax abs=1,1,1,1 mt=$mt |                                                     " << endl;
    out << "suxmovie  clip=1.0                                                                    \\" << endl;
    out << "    title=\"Acoustic Finite-Differencing\"                                            \\" << endl;
    out << "    windowtitle=\"Movie\"                                                             \\" << endl;
    out << "    label1=\"$label1\" label2=\"$label2\"                                             \\" << endl;
    out << "    n1=$n1 d1=$d1 f1=$f1 n2=$n2 d2=$d2 f2=$f2                                         \\" << endl;
    out << "    cmap=gray loop=1                                                                  \\" << endl;
    out << "    -geometry ${WIDTH}x${HEIGHT}+${WIDTHOFF1}+${HEIGHTOFF1} &                           " << endl;
    out << "                                                                                        " << endl;
    out << "exit 0                                                                                  " << endl;

    // Setting the arguments for sh.
    QStringList args;
    args << "script.sh";
    propagationModelTextEdit->append(args.at(0));
    process.setWorkingDirectory( QDir::current().currentPath() );
    propagationModelTextEdit->append( QDir::current().currentPath() );

    // Setting the Environment for Seismic Unix
    QStringList env = QProcess::systemEnvironment();
    process.setEnvironment(env);

    env << "CWPROOT=/opt/SU";
    env << "PATH=$PATH:/opt/SU/bin";

    process.start("sh", args);
}


void MainWindow::runXimage()
{
  	// Setting the Environment for Seismic Unix
	QStringList env = QProcess::systemEnvironment();
 	env << "CWPROOT=/opt/SU";
 	env << "PATH=$PATH:/opt/SU/bin";
 	process.setEnvironment(env);
	process2.setEnvironment(env);

	
    velocityModelTextEdit->clear();

    QString n1 = n1SpinBox->text();
    QString n2 = n2SpinBox->text();
    QString d1 = d1SpinBox->text();
    QString d2 = d2SpinBox->text();

//  QString sourceFile = sourceFileEdit->text();
	// Setting arguments for unif2
    velocityModelTextEdit->append("unif2");
    QStringList args;
    args 	<< "n1=" + n1 
	       	<< "n2=" + n2
	        << "method=spline"
	        ;

    for ( QStringList::Iterator it = args.begin(); it != args.end(); ++it ) {
        velocityModelTextEdit->append(*it);
    }

	// Process unif2.
	process2.setStandardInputFile("model.out");
	process2.setStandardOutputFile("vel.out");
	process2.setWorkingDirectory( QDir::current().currentPath() );
	process2.start("unif2", args);
	process2.waitForFinished();

    args.clear();
	
	// Setting arguments for ximage
    velocityModelTextEdit->append("ximage");
    args	<< "n1=" + n1 
         	<< "n2=" + n2
         	<< "d1=" + d1
         	<< "d2=" + d2
         	<< "legend=1"
        	;

    for ( QStringList::Iterator it = args.begin(); it != args.end(); ++it ) {
        velocityModelTextEdit->append(*it);
    }

	process.setStandardInputFile("vel.out");
	process.setWorkingDirectory( QDir::current().currentPath() );
	process.start("ximage", args);

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


   distancia =  xsSpinBox->text().toInt();
   profundidad =  zsSpinBox->text().toInt();

   QTextStream out(&file);

   out << qPrintable(QString("MODEL=%1").arg(sourceFile)) << endl;
   out << qPrintable(QString("DISTANCIA=%1").arg(distancia)) << endl;
   out << qPrintable(QString("PROFUNDIDAD=%1").arg(profundidad)) << endl;

   setCurrentFile( fileName );

   return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile.isEmpty()) {
        shownName = strippedName(curFile);
        recentFiles.removeAll(curFile);
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
