#include <QtGui>
#include "seismicu.h"

// 
//  seismicu.cpp
//  seismicu
//  
//  Created by Eduardo Gutarra on 2008-12-08.
//  Copyright 2008

SeismicU::SeismicU(QWidget *parent)
  : QDialog(parent)
{
    setupUi(this);

    QPushButton *convertButton =
    buttonBox->button(QDialogButtonBox::Ok);
    convertButton->setText(tr("&Run"));
    convertButton->setEnabled(true);
    outputTextEdit->append("Lol" );

    connect(convertButton, SIGNAL(clicked()),
            this, SLOT(runScript()));
    connect(&process, SIGNAL(readyReadStandardError()),
            this, SLOT(updateOutputTextEdit()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(processError(QProcess::ProcessError)));
}


void SeismicU::runScript()
{
    outputTextEdit->clear();

    QFile file("script.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QString text = spinBox->text();

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

    out << qPrintable(QString("xs=%1 zs=%2 hsz=50 vsx=250 verbose=2 ").arg(spinBox->text()).arg(spinBox_2->text())) << endl;

    out << "vsfile=\"vseis.out\" ssfile=\"sseis.out\" hsfile=\"hseis.out\"                          " << endl;
    out << "tmax=.4 mt=5 fpeak=35 fmax=40                                                           " << endl;
    out << "                                                                                        " << endl;
    out << "#makevel nx=$n2 nz=$n1 v000=3000 >vel.out                                               " << endl;
    out << "unif2 tfile=model.out                                                                   " << endl;
    out << "unif2 < model.out n1=$n1 n2=$n2 method=spline > vel.out                                 " << endl;
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
    outputTextEdit->append(args.at(0));
    process.setWorkingDirectory("/Users/Naix/Tmp/repotmp/seismicu");
    outputTextEdit->append(process.workingDirectory());

    // Setting the Environment for Seismic Unix
    QStringList env = QProcess::systemEnvironment();
    process.setEnvironment(env);

    env << "CWPROOT=/opt/SU";
    env << "PATH=$PATH:/opt/SU/bin";

    process.start("sh", args);
}


void SeismicU::processFinished(int exitCode,
                               QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        outputTextEdit->append(tr("Conversion program crashed"));
    } else if (exitCode != 0) {
        outputTextEdit->append(QString("Conversion failed, code=%1").arg(exitCode));
    } else {
        outputTextEdit->append(tr("File %1 created").arg(targetFile));
    }
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}


void SeismicU::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        outputTextEdit->append(tr("Conversion program not found"));
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}


void SeismicU::updateOutputTextEdit()
{
    QByteArray newData = process.readAllStandardError();
    QString text = outputTextEdit->toPlainText()
                   + QString::fromLocal8Bit(newData);
    outputTextEdit->setPlainText(text);
}
