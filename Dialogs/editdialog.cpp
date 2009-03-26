#include <QtGui>
#include <QWizard>
#include <QtDebug>

#include "editdialog.h"
#include "wizard.h"

EditDialog::EditDialog( QWidget *parent ) : QDialog( parent )
{
    setupUi( this );
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(velocityButton, SIGNAL(clicked()), this, SLOT(setVelocities()));
    wizard = 0;
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

void EditDialog::setVelocities()
{
    if (!wizard) {
        wizard = new ClassWizard( this );
    } else {
        wizard->show();
    }

    if ( wizard->exec() ) {
        QString numObjs = wizard->field("numObjs").toString();

        qDebug() << "number of objects: " << numObjs;

        int num = numObjs.toInt();
        for ( int i = 0; i < num; ++i ) {
            QString strVel = wizard->field("sbVel" + QString::number(i)).toString();
            qDebug() << "velocity "  + QString::number(i) << strVel;
            vels << strVel;
        }
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
		qDebug() << "cancelled action";
    }
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
