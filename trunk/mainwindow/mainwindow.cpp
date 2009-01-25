#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    setupUi( this );
    createActions();
    createToolBars();

    QPushButton *convertButton =
            buttonBox->button(QDialogButtonBox::Ok);
    convertButton->setText(tr("&Convert"));

    connect(convertButton, SIGNAL(clicked()),
            this, SLOT(convertImage()));
}

void MainWindow::createActions()
{
    action_New->setIcon(QIcon(":/images/new.png"));
    action_New->setShortcut(QKeySequence::New);
    action_New->setStatusTip(tr("Create a new * file"));
    connect(action_New, SIGNAL(triggered()), this, SLOT(newFile()));

    actionSave->setIcon(QIcon(":/images/save.png"));
    actionSave->setShortcut(QKeySequence::Save);
    actionSave->setStatusTip(tr("Save a * file"));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(action_New);
    fileToolBar->addAction(actionSave);
}

void MainWindow::newFile()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::saveFile()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::convertImage()
{
    textEdit->clear();
    QFile file("script.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
	
	// Stuff to write to the script
    QTextStream out(&file);
    out << "# /bin/sh                                                                               " << endl;
    out << "# Sufdmod2 --- example script for sufdmod2                                              " << endl;
    out << "# finite-difference modeling: part 1, the movie                                         " << endl;
    out << "# Author: John Stockwell                                                                " << endl;
    out << "WIDTH=450                                                                               " << endl;
    out << "HEIGHT=450                                                                              " << endl;
    out << "WIDTHOFF1=10                                                                            " << endl;
    out << "WIDTHOFF2=500                                                                           " << endl;
    out << "HEIGHTOFF1=50                                                                           " << endl;
    out << "HEIGHTOFF2=530                                                                          " << endl;
    out << "                                                                                        " << endl;
    out << "n1=100 d1=5 f1=0.0 label1=\"Depth (km)\"                                                " << endl;
    out << "n2=100 d2=5 f2=0.0 label2=\"Distance (km)\"                                             " << endl;

	out << qPrintable(QString("xs=%1 zs=%2 hsz=50 vsx=250 verbose=2 ")
														.arg(spinBox->text()).arg(spinBox_2->text())) << endl;
														
    out << "vsfile=\"vseis.out\" ssfile=\"sseis.out\" hsfile=\"hseis.out\"                          " << endl;
    out << "tmax=.4 mt=5 fpeak=35 fmax=40                                                           " << endl;
    out << "                                                                                        " << endl;
    out << "makevel nx=$n2 nz=$n1 v000=3000 >vel.out                                                " << endl;
    out << "unif2 tfile=model.out                                                                   " << endl;
    out << "unif2 < model.out n1=$n1 n2=$n2 method=spline > vel.out								 	" << endl;
    out << "                                                                                        " << endl;
    out << "ximage < vel.out wbox=$WIDTH hbox=$HEIGHT xbox=$WIDTHOFF2 title=\"Wavespeed Profile\" \\" << endl;
    out << " n1=$n1 n2=$n2 legend=1 lheight=150 units=\"wave speed\" &								" << endl;
    out << "                                                                                        " << endl;
    out << "sufdmod2 <vel.out nz=$n1 dz=$d1 nx=$n2 dx=$d2 verbose=1                               \\" << endl;
    out << "	fpeak=$fpeak fmax=$fmax                                                           \\" << endl;
    out << "	xs=$xs zs=$zs hsz=$hsz vsx=$vsx hsfile=$hsfile                                    \\" << endl;
    out << "	vsfile=$vsfile ssfile=$ssfile verbose=$verbose                                    \\" << endl;
    out << "	tmax=$tmax abs=1,1,1,1 mt=$mt |                                                     " << endl;
    out << "suxmovie  clip=1.0                                                                    \\" << endl;
    out << "	title=\"Acoustic Finite-Differencing\"                                            \\" << endl;
    out << "	windowtitle=\"Movie\"                                                             \\" << endl;
    out << "	label1=\"$label1\" label2=\"$label2\"                                             \\" << endl;
    out << "	n1=$n1 d1=$d1 f1=$f1 n2=$n2 d2=$d2 f2=$f2                                         \\" << endl;
    out << "	cmap=gray loop=1                                                                  \\" << endl;
    out << "	-geometry ${WIDTH}x${HEIGHT}+${WIDTHOFF1}+${HEIGHTOFF1} &                           " << endl;
    out << "                                                                                        " << endl;
    out << "exit 0                                                                                  " << endl;

    // Setting the arguments for sh.
    QStringList args;
    args << "script.txt";
    textEdit->append(args.at(0));
    process.setWorkingDirectory( QDir::current().currentPath() );
    textEdit->append( QDir::current().currentPath() );

    // Setting the Environment for Seismic Unix
    QStringList env = QProcess::systemEnvironment();
    process.setEnvironment(env);

    env << "CWPROOT=/opt/SU";
    env << "PATH=$PATH:/opt/SU/bin";

    process.start("sh", args);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
}
