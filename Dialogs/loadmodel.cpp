#include <QtGui>

#include "loadmodel.h"

LoadModel::LoadModel(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(propagationModelButtonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(propagationModelButtonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton *simulateButton =
            propagationModelButtonBox->button(QDialogButtonBox::Ok);
    simulateButton->setText(tr("&Correr"));

    connect(simulateButton, SIGNAL(clicked()), this, SLOT(runSimulation()));
}

void LoadModel::on_browseButton_clicked()
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
        modelFile = sourceFileEdit->text();
    }
}

QString LoadModel::getModelFile() {
	return modelFile;
}

void LoadModel::runSimulation()
{
	// propagationModelTextEdit->clear();
	// 
	//     QFile file("script.sh");
	//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	//         return;
	// 
	//         // Stuff to write to the script
	//     QTextStream out(&file);
	//     out << "# /bin/sh                                                                               " << endl;
	//     out << "WIDTH=450                                                                               " << endl;
	//     out << "HEIGHT=450                                                                              " << endl;
	//     out << "WIDTHOFF1=10                                                                            " << endl;
	//     out << "WIDTHOFF2=500                                                                           " << endl;
	//     out << "HEIGHTOFF1=50                                                                           " << endl;
	//     out << "HEIGHTOFF2=530                                                                          " << endl;
	//     out << "                                                                                        " << endl;
	//     out << "n1=100 d1=5 f1=0.0 label1=\"Profundidad (km)\"                                                " << endl;
	//     out << "n2=100 d2=5 f2=0.0 label2=\"Distancia (km)\"                                             " << endl;
	//         out << qPrintable(QString("xs=%1 zs=%2 hsz=50 vsx=250 verbose=2 ").arg(xsSpinBox->text()).arg(zsSpinBox->text())) << endl;
	// 
	//     out << "vsfile=\"vseis.out\" ssfile=\"sseis.out\" hsfile=\"hseis.out\"                          " << endl;
	//     out << "tmax=.4 mt=5 fpeak=35 fmax=40                                                           " << endl;
	//     out << "                                                                                        " << endl;
	//     out << "makevel nx=$n2 nz=$n1 v000=3000 >vel.out                                                " << endl;
	//     out << qPrintable(QString("unif2 < %1 n1=$n1 n2=$n2 method=spline > vel.out  ")
	//                                                                                                                 .arg(modelFile)) << endl;
	//     out << "unif2 < model.out n1=$n1 n2=$n2 method=spline > vel.out                                                                     " << endl;
	//     out << "                                                                                        " << endl;
	//     out << "ximage < vel.out wbox=$WIDTH hbox=$HEIGHT xbox=$WIDTHOFF2 title=\"Wavespeed Profile\" \\" << endl;
	//     out << " n1=$n1 n2=$n2 legend=1 lheight=150 units=\"wave speed\" &                                                          " << endl;
	//     out << "                                                                                        " << endl;
	//     out << "sufdmod2 <vel.out nz=$n1 dz=$d1 nx=$n2 dx=$d2 verbose=1                               \\" << endl;
	//     out << "    fpeak=$fpeak fmax=$fmax                                                           \\" << endl;
	//     out << "    xs=$xs zs=$zs hsz=$hsz vsx=$vsx hsfile=$hsfile                                    \\" << endl;
	//     out << "    vsfile=$vsfile ssfile=$ssfile verbose=$verbose                                    \\" << endl;
	//     out << "    tmax=$tmax abs=1,1,1,1 mt=$mt |                                                     " << endl;
	//     out << "suxmovie  clip=1.0                                                                    \\" << endl;
	//     out << "    title=\"Acoustic Finite-Differencing\"                                            \\" << endl;
	//     out << "    windowtitle=\"Movie\"                                                             \\" << endl;
	//     out << "    label1=\"$label1\" label2=\"$label2\"                                             \\" << endl;
	//     out << "    n1=$n1 d1=$d1 f1=$f1 n2=$n2 d2=$d2 f2=$f2                                         \\" << endl;
	//     out << "    cmap=gray loop=1                                                                  \\" << endl;
	//     out << "    -geometry ${WIDTH}x${HEIGHT}+${WIDTHOFF1}+${HEIGHTOFF1} &                           " << endl;
	//     out << "                                                                                        " << endl;
	//     out << "exit 0                                                                                  " << endl;
	// 
	//     // Setting the arguments for sh.
	//     QStringList args;
	//     args << "script.sh";
	//     propagationModelTextEdit->append(args.at(0));
	//     ximage.setWorkingDirectory( QDir::current().currentPath() );
	//     propagationModelTextEdit->append( QDir::current().currentPath() );
	// 
	//     // Setting the Environment for Seismic Unix
	//     QStringList env = QProcess::systemEnvironment();
	//     ximage.setEnvironment(env);
	// 
	//     env << "CWPROOT=/opt/SU";
	//     env << "PATH=$PATH:/opt/SU/bin";
	// 
	//     ximage.start("sh", args);
}

