#include <QtGui>
#include <QWizard>
#include <QtDebug>

#include "editdialog.h"
#include "wizard.h"
#include "coordinatesetter.h"

EditDialog::EditDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(velocityButton, SIGNAL(clicked()), this, SLOT(setVelocities()));
    // wizard = 0;
	coordinateSetter = 0;
}

const QString EditDialog::modelName() const
{
    return nameEdit->text().replace("--","").trimmed();
}

void EditDialog::setModelName( const QString &modelName )
{
    nameEdit->setText( modelName );
}

const QString EditDialog::modelLocation() const
{
    return sourceFileEdit->text().replace("--","").trimmed();
}

void EditDialog::setModelLocation( const QString &modelLocation )
{
    sourceFileEdit->setText( modelLocation );
}

void EditDialog::setVelocities( const QStringList &vels ) {
	this->vels = vels;
}

void EditDialog::setVelocities()
{
	QFile file(modelLocation());
	
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
	//      QMessageBox::warning(this, tr("Simulation"),
	//                           tr("Cannot read file %1:\n%2.")
	//                           .arg(file.fileName())
	//                           .arg(file.errorString()));
	        return;
	}
	
	// Counts the number of layers
	QTextStream in(&file);

    int numLayers = 0;
    QString word;
    while (!in.atEnd()) {
        in >> word;
        if (word == "-99999") {
            ++numLayers;
        }
    }

    QList<double> coordinates;
	if (vels.size() == (numLayers - 1))
	{
		QString text;
		foreach( text, vels ) {
			coordinates << text.toDouble();
		}
	} 
	else {
    	double inc = 2500.0 / (numLayers - 1.0);

	    for (int i = 0; i < numLayers - 1 ; ++i ) {
	        coordinates << 1500.0 + i*inc;
	    }
	} 
	
	if (!coordinateSetter) {
        coordinateSetter = new CoordinateSetter(&coordinates, this);
    } else {
        coordinateSetter->show();
    }

    if (coordinateSetter->exec()) {
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		vels = coordinateSetter->velocities();
    } else {
	
	}
    
    // if (!wizard) {
    //        wizard = new ClassWizard( this );
    //    } else {
    //        wizard->show();
    //    }
    // 
    //    if ( wizard->exec() ) {
    //        QString numObjs = wizard->field("numObjs").toString();
    // 
    //        qDebug() << "number of objects: " << numObjs;
    // 
    //        int num = numObjs.toInt();
    //        for ( int i = 0; i < num; ++i ) {
    //            QString strVel = wizard->field("sbVel" + QString::number(i)).toString();
    //            qDebug() << "velocity "  + QString::number(i) << strVel;
    //            vels << strVel;
    //        }
    //        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    //    } else {
    // 		qDebug() << "cancelled action";
    //    }
}

void EditDialog::on_browseButton_clicked()
{
    QString initialName = sourceFileEdit->text();
    if (initialName.isEmpty())
        initialName = QDir::homePath();
    QString fileName =
    QFileDialog::getOpenFileName(this, tr("Choose File"),
                                 initialName,
                                 tr("Model Files (*.out)"));
    fileName = QDir::toNativeSeparators(fileName);
    if (!fileName.isEmpty()) {
        sourceFileEdit->setText(fileName);
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

const QStringList EditDialog::velocities() const {
    return vels;
}
