#include <QtGui>
#include <iostream>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    setupUi( this );

    createActions();
    createToolBars();

    QPushButton *convertButton =
            buttonBox->button(QDialogButtonBox::Ok);
    convertButton->setText(tr("&Correr"));

    connect(convertButton, SIGNAL(clicked()),
            this, SLOT(runSimulation()));
    
    convertButton->setEnabled(false);
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
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        sourceFile = sourceFileEdit->text();
    }
}


void MainWindow::newFile()
{
    setCurrentFile( "" );
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::runSimulation()
{
    textEdit->clear();

    QString sourceFile = sourceFileEdit->text();
    textEdit->append(sourceFile);

    // QFile file("script.sh");
    // if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    //     return;
	
    // Setting the Environment for Seismic Unix
    QStringList env = QProcess::systemEnvironment();

    env << "CWPROOT=/opt/SU";
    env << "PATH=$PATH:/opt/SU/bin";

    process.setEnvironment(env);
    process2.setEnvironment(env);

    // Setting the arguments for unif2.
    QStringList args;
	args << "n1=100" << "n2=100" << "method=spline" << "label1='Depth (m)'" << "label2='Distance (m)'";
    textEdit->append(args.at(0));
	process2.setStandardInputFile("model.out");
	process2.setStandardOutputFile("vel.out");
    process2.setWorkingDirectory( QDir::current().currentPath() );
	process2.start("unif2", args);
	process2.waitForFinished();
	args.clear();

    // Setting the arguments for ximage.
    args << "n1=100" << "n2=100" << "d1=5" << "d2=5" << "legend=1";
    textEdit->append(args.at(0));
	process.setStandardInputFile("vel.out");
    process.setWorkingDirectory( QDir::current().currentPath() );
    textEdit->append( QDir::current().currentPath() );

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


   distancia =  spinBox->text().toInt();
   profundidad =  spinBox_2->text().toInt();
   std::cout << distancia << " " << profundidad << endl;

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
//      recentFiles.removeAll(curFile);
//      recentFiles.prepend(curFile);
//      updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                                   .arg(tr("Simulacion")));
}

//void MainWindow::updateRecentFileActions()
//{
//    QMutableStringListIterator i(recentFiles);
//    while (i.hasNext()) {
//        if (!QFile::exists(i.next()))
//            i.remove();
//    }
//
//    for (int j = 0; j < MaxRecentFiles; ++j) {
//        if (j < recentFiles.count()) {
//            QString text = tr("&%1 %2")
//                           .arg(j + 1)
//                           .arg(strippedName(recentFiles[j]));
//            recentFileActions[j]->setText(text);
//            recentFileActions[j]->setData(recentFiles[j]);
//            recentFileActions[j]->setVisible(true);
//        } else {
//            recentFileActions[j]->setVisible(false);
//        }
//    }
//    separatorAction->setVisible(!recentFiles.isEmpty());
//}
//

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
