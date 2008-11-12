#include <QtGui>

#include "convertdialog.h"
#include <cstdlib>

ConvertDialog::ConvertDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    QPushButton *convertButton =
            buttonBox->button(QDialogButtonBox::Ok);
    convertButton->setText(tr("&Run"));
    convertButton->setEnabled(true);

    connect(convertButton, SIGNAL(clicked()),
            this, SLOT(convertImage()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(&process, SIGNAL(readyReadStandardOutput()),
            this, SLOT(updateOutputTextEdit()));

	connect(&process, SIGNAL(readyReadStandardError()),
            this, SLOT(updateOutputTextEdit()));

    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(processError(QProcess::ProcessError)));
}

void ConvertDialog::on_browseButton_clicked()
{
    QString initialName = sourceFileEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("&Browse"),
                                         initialName);
    fileName = QDir::toNativeSeparators(fileName);
    if (!fileName.isEmpty()) {
        sourceFileEdit->setText(fileName);
//        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void ConvertDialog::convertImage()
{
    QString sourceFile = sourceFileEdit->text();
    targetFile = QFileInfo(sourceFile).path() + QDir::separator()
                 + QFileInfo(sourceFile).baseName() + "."
                 + targetFormatComboBox->currentText().toLower();
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    outputTextEdit->clear();

    QStringList args;
	args << "n1=100";
	args << "< /Users/Naix/Documents/Projects/seismic-unix/vel.out";

	//	if (enhanceCheckBox->isChecked())
//        args << "-enhance";
//    if (monochromeCheckBox->isChecked())
//        args << "-monochrome";
	
    //args << sourceFile << targetFile;
//	::system("xlogo -v");
//	::system("/opt/SU/bin/ximage n1=100");	
//	outputTextEdit->append( args.at(0));
	QStringList env = QProcess::systemEnvironment();
	process.setEnvironment(env);
	env << "CWPROOT=/opt/SU"; 
	env << "PATH=$PATH:/opt/SU/bin"; 
	::system("/opt/SU/bin/ximage n1=100 < /Users/Naix/Documents/Projects/seismic-unix/vel.out");
//	::system("/opt/SU/bin/ximage n1=100 < Documents/Projects/seismic-unix/vel.out");
//	process.start("/opt/SU/bin/ximage", args);
//	process.start("/opt/SU/bin/ximage n1=100 < /Users/Naix/Documents/Projects/seismic-unix/vel.out");
//	process.start("/opt/SU/bin/ximage n1=100 < /Users/Naix/Documents/Projects/seismic-unix/vel.out");
//	process.start("/opt/SU/src/demos/Synthetic/Finite_Difference/Sufdmod2/XDemo3");
}

void ConvertDialog::updateOutputTextEdit()
{
    QByteArray newData = process.readAllStandardError();
    QString text = outputTextEdit->toPlainText()
                   + QString::fromLocal8Bit(newData);
    outputTextEdit->setPlainText(text);
}

void ConvertDialog::processFinished(int exitCode,
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

void ConvertDialog::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        outputTextEdit->append(tr("Conversion program not found"));
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}
